#include "../intf/instrLL.h"
#include "../intf/instrTable.h"
#include "../intf/tokenLL.h"

#include "../intf/token.h"
#include "../intf/tokenQueue.h"

// #include "../intf/lexer.h"

#include "../intf/assem.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char *get_token_name(TokenType type) {
  switch (type) {
  case TOKEN_EOF:
    return "TOKEN_EOF";
  case TOKEN_EOL:
    return "TOKEN_EOL";
  case TOKEN_MNEMONIC:
    return "TOKEN_MNEMONIC";
  case TOKEN_REGISTER:
    return "TOKEN_REGISTER";
  case TOKEN_LABEL:
    return "TOKEN_LABEL";
  case TOKEN_COMMA:
    return "TOKEN_COMMA";
  case TOKEN_NUMBER:
    return "TOKEN_NUMBER";
  case TOKEN_COMMENT:
    return "TOKEN_COMMENT";
  case TOKEN_UNKNOWN:
    return "TOKEN_UNKNOWN";
  case TOKEN_ERROR:
    return "TOKEN_ERROR";
  default:
    return "UNDEFINED_TOKEN";
  }
}

int calcInstrSize(OperandType type) {
  switch (type) {
  // 1 byte opcode only or lower nibble operand
  case OP_NONE:
  case OP_REG:
  case OP_REG_N0:
  case OP_PORT_OUT:
  case OP_PORT_INP:
    return 1;

  // 2 bytes, opcode + 8 bits
  case OP_IMM:
  case OP_ADDR8:
    return 2;

  // 3 bytes, opcode + 16-bits
  case OP_ADDR16:
    return 3;

  // Something went wrong lol
  default:
    return 0;
  }
}

void populate(queue *tokens, tokenNode **linkedTokens) {
  while (!isEmpty(tokens)) {
    Token front = dequeue(tokens);

    insertTK(linkedTokens, front);
  }

  return;
}

void printTokens(tokenNode **myNode) {
  tokenNode *curr = *myNode;
  Token currToken = curr->data;
  printf("%s : %s\n", get_token_name(currToken.type), currToken.text);

  while (curr->next != NULL) {
    curr = curr->next;
    currToken = curr->data;
    printf("%s : %s\n", get_token_name(currToken.type), currToken.text);
  }
}

void printLabels(labelNode *myNode) {
  labelNode *curr = myNode;
  while (curr != NULL) {
    printf("Label name: %s\n Address: 0x%x\n\n", curr->data.name,
           curr->data.address);

    curr = curr->next;
  }

  return;
}

InstructionInfo *findInstr(char *mnemonic) {
  for (int i = 0; i < OPCODE_TABLE_SIZE; i++) {
    if (strcasecmp(mnemonic, opcode_table[i].mnemonic) == 0) {
      return &opcode_table[i]; // Return a pointer to the matched struct
    }
  }

  // If the loop finishes without returning, the mnemonic isn't in the table
  return NULL;
}

label *findLabel(labelNode *myNode, char *myLabel) {
  labelNode *curr = myNode;
  while (curr != NULL) {
    // If we find a match, return a pointer to that specific label's data
    if (strcasecmp(myLabel, curr->data.name) == 0) {
      return &(curr->data);
    }

    // Move to the next node
    curr = curr->next;
  }

  // If we get here, the label wasn't in the list
  return NULL;
}

void pass1(tokenNode **myNode, labelNode **symbolTable) {
  uint16_t pc = 0x0000;
  tokenNode *curr = *myNode;
  Token currToken = curr->data;

  int mnemonic_count = 0;
  int label_count = 0;

  while (curr != NULL && curr->data.type != TOKEN_EOF) {
    currToken = curr->data;

    // Assume 'currToken' is fetched from your lexer/queue
    switch (currToken.type) {
    case TOKEN_LABEL:
      printf("label processing: %s\n", currToken.text);

      if (findLabel(*symbolTable, currToken.text) != NULL) {
        printf("Duplicate label definition '%s'\n", currToken.text);
        break;
      }

      // Pass 1: Add current_token.text and current_pc to the Symbol Table
      // put together a new label
      label newLabel;
      strcpy(newLabel.name, currToken.text);

      // dont store the colons in the label name
      size_t len = strlen(newLabel.name);
      if (len > 0 && newLabel.name[len - 1] == ':') {
        newLabel.name[len - 1] = '\0';
      }

      if (curr->next != NULL && curr->next->data.type == TOKEN_MNEMONIC &&
          strcasecmp(curr->next->data.text, "EQU") == 0) {

        // Move past the label and past the 'EQU' token
        curr = curr->next; // Now on 'EQU'
        curr = curr->next; // Now theoretically on the number token

        if (curr != NULL && curr->data.type == TOKEN_NUMBER) {
          // process EQU directive
          newLabel.address = (uint16_t)curr->data.value;
          printf("EQU directive: %s set to 0x%04X\n", newLabel.name,
                 newLabel.address);

          while (curr->next != NULL && curr->next->data.type != TOKEN_EOL) {
            curr = curr->next;
          }

        } else {
          printf(
              "Expected number after EQU, moving on but things will break\n");
          newLabel.address = 0; // that isn't supposed to happen and probably
                                // will break things if it does
        }
      } else {
        // Standard label, bind to current PC
        newLabel.address = pc;
      }

      // add the label to the symbol table
      insertLabel(symbolTable, newLabel);
      label_count++;
      break;

    case TOKEN_MNEMONIC:
      // Pass 1 & 2: Look up the mnemonic in opcode_table.
      // Get expected operands, calculate PC increment, and generate code (Pass
      // 2)

      printf("mnemonic processing: %s\n", currToken.text);

      if (strcasecmp(currToken.text, "ORG") == 0) {
        if (curr->next != NULL && curr->next->data.type == TOKEN_NUMBER) {
          curr = curr->next;
          pc = (uint16_t)curr->data.value;
          printf("PC updated to: %04X\n", pc);
        } else {
          printf("something went wrong with the ORG\n");
        }
        break;
      }

      // i kind of didn't want to add this yet but whatever
      if (strcasecmp(currToken.text, "DB") == 0) {
        int byte_count = 0;
        while (curr->next != NULL && curr->next->data.type != TOKEN_EOL &&
               curr->next->data.type != TOKEN_EOF &&
               curr->next->data.type != TOKEN_COMMENT) {

          curr = curr->next;

          // things can go wrong here if there are labels that are more than one
          // byte
          if (curr->data.type == TOKEN_NUMBER ||
              curr->data.type == TOKEN_LABEL ||
              curr->data.type == TOKEN_MNEMONIC) {
            byte_count++;
          }
        }
        pc += byte_count;
        printf("DB: + %d bytes. PC 0x%x\n", byte_count, pc);
        break;
      }

      InstructionInfo *info = findInstr(currToken.text);

      if (info != NULL) {
        // Increment the PC by the size of the valid instr
        pc += calcInstrSize(info->type);

      } else {
        // Handle error: Unknown mnemonic
        printf("bad mnemonic or something I cant handle yet\n");
      }

      // obliterate operands, we can deal with that later
      while (curr->next != NULL && curr->next->data.type != TOKEN_EOL &&
             curr->next->data.type != TOKEN_COMMENT &&
             curr->next->data.type != TOKEN_EOF) {
        curr = curr->next;
      }

      // debug
      mnemonic_count++;

      break;

    case TOKEN_COMMENT:
      // printf("comment, time to skip\n");
      while (curr->next != NULL && (curr->next->data).type != TOKEN_EOL &&
             (curr->next->data).type != TOKEN_EOF) {
        printf("skip\n");
        curr = curr->next;
        currToken = curr->data;
      }

      break;

    case TOKEN_EOL:
      printf("eol token\n");
      break;

    case TOKEN_EOF:
      // Reached the end of the file. Break out of the parsing loop.
      break;

    case TOKEN_UNKNOWN:
    case TOKEN_ERROR:
      // Throw a syntax error, print line number, and halt assembly.
      printf("Unexpected token '%s', killing\n", currToken.text);
      exit(-1);
      break;

    case TOKEN_REGISTER:
      printf("register\n");
      break;

    case TOKEN_COMMA:
      printf("comma\n");
      break;

    case TOKEN_NUMBER:
      printf("number\n");
      break;

    default:
      // Handles cases where a line starts weirdly, like starting with a comma.
      printf("Parse Error: Invalid start of statement.\n");
      break;
    }

    curr = curr->next;
  }

  printLabels(*symbolTable);
  printf("label count: %d, mnemonic count: %d\n", label_count, mnemonic_count);
  printf("final pc: %x\n", pc);
}

void pass2(tokenNode **myNode, labelNode **symbolTable) {
  // here we actually build the binary
  // we use an output buffer to make this easy
  // The symbol table we built earlier is also used in this process to translate
  // labels to memory addresses

  uint16_t pc = 0x0000;
  uint16_t minPc = 0xFFFF;
  uint16_t maxPc = 0x0000;

  uint8_t binBuf[65536];

  tokenNode *curr = *myNode;
  Token currToken;

  while (curr != NULL && curr->data.type != TOKEN_EOF) {
    currToken = curr->data;

    switch (currToken.type) {
    case TOKEN_LABEL:
      // until further notice I don't think anything needs to be done here
      break;
    case TOKEN_MNEMONIC:
      // ORG directive
      if (strcasecmp(currToken.text, "ORG") == 0) {
        printf("ORG directive\n");
        if (curr->next != NULL && curr->next->data.type == TOKEN_NUMBER) {
          curr = curr->next;
          pc = (uint16_t)curr->data.value;
          printf("Set to 0x%x\n", pc);
        }
        break;
      }

      // i know this sounds weird, but EQU directive
      // Technically there is already a label for it
      // BUT the directive itself is handled as a mnemonic so we need to skip
      // over it
      if (strcasecmp(currToken.text, "EQU") == 0) {
        printf("EQU directive (do nothing useful except skip ahead)\n");
        if (curr->next != NULL && curr->next->data.type == TOKEN_NUMBER) {
          curr = curr->next;
        }
        break;
      }

      // handle DB directive
      // works with just a byte or the lower 8 bits of a label so far
      // in the future maybe we will do strings
      if (strcasecmp(currToken.text, "DB") == 0) {
        printf("DB directive\n");
        while (curr->next != NULL && curr->next->data.type != TOKEN_EOL &&
               curr->next->data.type != TOKEN_EOF &&
               curr->next->data.type != TOKEN_COMMENT) {
          curr = curr->next;

          if (curr->data.type == TOKEN_NUMBER) {
            if (pc < minPc) {
              minPc = pc;
            }

            binBuf[pc] = (uint8_t)(curr->data.value & 0xFF);
            pc++;

            if (pc > maxPc) {
              maxPc = pc;
            }

          } else if (curr->data.type == TOKEN_LABEL ||
                     curr->data.type == TOKEN_MNEMONIC) {
            // Make use of lower 8 bits of a label using bitwise and
            // ill think about doing strings at another time
            label *myLabel = findLabel(*symbolTable, curr->data.text);
            if (pc < minPc) {
              minPc = pc;
            }

            if (myLabel) {
              binBuf[pc] = (uint8_t)(myLabel->address & 0xFF);
            } else {
              binBuf[pc] = 0x00;
            }
            pc++;

            if (pc > maxPc) {
              maxPc = pc;
            }
          }
        }
        break;
      }

      // Now that we have dealt with the other stuff, we either have an
      // instruction or some garbage
      InstructionInfo *info = findInstr(currToken.text);
      printf("%s", currToken.text);
      if (info != NULL) {
        if (pc < minPc)
          minPc = pc;

        // Make our opcode byte
        uint8_t opcode;
        if (info->n != -1) {
          opcode = (info->i << 4) | (info->n & 0x0F);
        } else {
          // We handle the lower nibble later since this instruction needs it
          opcode = (info->i << 4);
        }

        binBuf[pc] = opcode;
        pc++;

        // Is there something coming up?
        // Go to it because we need to build the opcode/operands if so
        if (curr->next != NULL && curr->next->data.type != TOKEN_EOL &&
            curr->next->data.type != TOKEN_COMMENT) {
          curr = curr->next;
        } else {
          // I saw nothing, ill just move on then
          if (pc > maxPc) {
            maxPc = pc;
          }

          break;
        }

        // Our instruction info came with a piece of information for free
        // Now we get to use this information to determine what to do with the
        // token(s) after
        switch (info->type) {
        case OP_REG:
        case OP_REG_N0:
          if (curr->data.type == TOKEN_REGISTER) {
            // not sure how well this actually works for registers, investigate
            int regVal = (int)strtol(curr->data.text + 1, NULL, 0);
            binBuf[pc - 1] |= (regVal & 0x0F);
          }
          break;
        case OP_PORT_OUT:
        case OP_PORT_INP:
          if (curr->data.type == TOKEN_NUMBER) {
            binBuf[pc - 1] |= (curr->data.value & 0x07);
          }
          break;
        // 2 bytes
        case OP_IMM:
        case OP_ADDR8:
          if (curr->data.type == TOKEN_NUMBER) {
            binBuf[pc] = (uint8_t)(curr->data.value & 0xFF);
            pc++;
          } else if (curr->data.type == TOKEN_LABEL ||
                     curr->data.type == TOKEN_MNEMONIC) {
            label *myLabel = findLabel(*symbolTable, curr->data.text);
            if (myLabel) {
              binBuf[pc] = (uint8_t)(myLabel->address & 0xFF);
            } else {
              printf("Bad label, things will go wrong. 0x00 at 0x%x\n", pc);
              binBuf[pc] = 0x00;
            }
            pc++;
          }
          break;
          // 3 bytes (evil)
        case OP_ADDR16:
          // so it doesnt explode when you try to compile on a slightly older
          // system
          while (0) {
          }

          uint16_t addr = 0x0000;
          if (curr->data.type == TOKEN_NUMBER) {
            addr = (uint16_t)curr->data.value;
          } else if (curr->data.type == TOKEN_LABEL ||
                     curr->data.type == TOKEN_MNEMONIC) {
            label *myLabel = findLabel(*symbolTable, curr->data.text);
            if (myLabel) {
              addr = myLabel->address;
            }
          }

          // endianness wackiness when dealing with a 3 byte instruction
          binBuf[pc] = (uint8_t)((addr >> 8) & 0xFF);
          pc++;
          binBuf[pc] = (uint8_t)(addr & 0xFF);
          pc++;
          break;

        default:
          break;
        }

        if (pc > maxPc) {
          maxPc = pc;
        }

      } else {
        printf("Unknown mnemonic '%s' at PC %04X, aborting\n", currToken.text,
               pc);
        exit(-1);
      }

      // obliterate the rest of the garbage
      while (curr->next != NULL && curr->next->data.type != TOKEN_EOL &&
             curr->next->data.type != TOKEN_COMMENT &&
             curr->next->data.type != TOKEN_EOF) {
        curr = curr->next;
      }

      break;

    // We ignore other tokens, they are handled with the mnemonic case
    case TOKEN_COMMENT:
    case TOKEN_EOL:
    case TOKEN_EOF:
    case TOKEN_UNKNOWN:
    case TOKEN_ERROR:
    case TOKEN_REGISTER:
    case TOKEN_COMMA:
    case TOKEN_NUMBER:
      break;
    }

    curr = curr->next;
  }

  if (maxPc <= minPc) {
    printf("Did you actually write a program? Nothing written.\n");
    return;
  }

  // Handle the actual file output
  FILE *binary = fopen("output.bin", "wb");

  // make sure the file opened properly, then output from the memory addresses
  // that were actually used
  if (binary != NULL) {

    fwrite(&binBuf[minPc], 1, maxPc - minPc, binary);
    fclose(binary);
    printf("Wrote %d bytes to output.bin\n", maxPc - minPc);
  } else {
    printf("The output binary file could not be created. Aborting.\n");
    exit(-1);
  }
}

void assem(queue *tokens) {

  // move what is in the queue into the linked list
  tokenNode *linkedTokens = NULL;
  populate(tokens, &linkedTokens);

  // print tokens (i mean this can be commented out but for now its debug
  printTokens(&linkedTokens);

  // We should have a linked list full of tokens
  //  But if not, this will tell us (until its removed)
  if (linkedTokens == NULL) {
    printf("Somehow we got to assembling, but the queue is empty\n");
    return;
  } else {
    printf("The queue is not empty, we can move on\n");
  }

  // Initialize symbol table;
  labelNode *symbolTable = NULL;

  printf("Did absolutely nothing yet but I am making it look like I did "
         "something\n");

  // figure out symbol addresses, etc all that good stuff
  pass1(&linkedTokens, &symbolTable);

  // actually figure out putting together the binary piece
  // probably need to set up output buffer to a file
  pass2(&linkedTokens, &symbolTable);

  return;
}

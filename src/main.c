#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int opcode;
	const char *mnemonic;
} Instruction;

struct Node {
	Instruction instr;
	struct Node* left;
	struct Node* right;
	int height;
};

int getHeight(struct Node* n) {
	if (n == NULL) {
		return 0;
	}
	return n->height;
}

struct Node* createNode(Instruction instr) {
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	node->instr = instr;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return node;
}


int max(int a, int b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

int getBalanceFactor(struct Node* n) {
	if (n == NULL) {
		return 0;
	}
	return getHeight(n->left) - getHeight(n->right);
}

//Right rotation
struct Node* rRotate(struct Node* y) {
	struct Node* x = y->left;
	struct Node* T2 = x->right;

	x->right = y;
	y->left = T2;
	
	y->height = max(getHeight(y->left), getHeight(y->right))+1;
	x->height = max(getHeight(x->left), getHeight(x->right))+1;

	return x;
}

//Left Rotation
struct Node* lRotate(struct Node* x) {
	struct Node* y = x->right;
	struct Node* T2 = y->left;

	y->left = x;
	x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right))+1;
        y->height = max(getHeight(y->left), getHeight(y->right))+1;

	return y;
}

struct Node* insert(struct Node* node, Instruction instr) {
    if (node == NULL) {
        return createNode(instr);
    }

    if (instr.opcode < node->instr.opcode) {
        node->left = insert(node->left, instr);
    } else if (instr.opcode > node->instr.opcode) {
        node->right = insert(node->right, instr);
    } else {
        return node;
    }

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalanceFactor(node);

    // Left Left
    if (balance > 1 && instr.opcode < node->left->instr.opcode)
        return rRotate(node);

    // Right Right
    if (balance < -1 && instr.opcode > node->right->instr.opcode)
        return lRotate(node);

    // Left Right
    if (balance > 1 && instr.opcode > node->left->instr.opcode) {
        node->left = lRotate(node->left);
        return rRotate(node);
    }

    // Right Left
    if (balance < -1 && instr.opcode < node->right->instr.opcode) {
        node->right = rRotate(node->right);
        return lRotate(node);
    }

    return node;
}

Instruction* findByOpcode(struct Node* root, int opcode) {
	if (!root) {
		return NULL;
	}
	if (opcode == root->instr.opcode) {
		return &root->instr;
	} else if (opcode < root->instr.opcode) {
		return findByOpcode(root->left, opcode);
	} else {
		return findByOpcode(root->right, opcode);
	}
}

Instruction* findByMnemonic(struct Node* root, const char* mnemonic) {
	if (!root) {
		return NULL;
	}

	struct Node* queue[256];
	int front = 0;
	int back = 0;

	queue[back++] = root;

	while (front < back) {
		struct Node* curr = queue[front++];

		if (strcmp(curr->instr.mnemonic, mnemonic) == 0) {
			return &curr->instr;
		}

		if (curr->left) {
			queue[back++] = curr->left;
		}

		if (curr->right) {
			queue[back++] = curr->right;
		}
	}
	return NULL;
}

//void inOrder(struct Node* root) {
//	if (root != NULL) {
//		inOrder(root->left);
//		printf("%d ", root->key);
//		inOrder(root->right);
//	}
//}

int main() {
    struct Node* root = NULL;
    
	//Testing vals
	Instruction insts[] = {
        {0x00, "IDL"},
        {0x01, "LDN"},
        {0x30, "BR"},
        {0xF8, "LDI"},
        {0xA0, "PLO"}
    };
    
    for (int i = 0; i < 5; i++) {
        root = insert(root, insts[i]);
    }
    
    // User input section
    char input[50];
    printf("Enter opcode or mnemonic: ");
    
    if (scanf("%s", input) != 1) {
        printf("Error reading input\n");
        return 1;
    }
    
    // Check if input is a hex opcode
    if (input[0] == '0' && (input[1] == 'x' || input[1] == 'X')) {
        // Parse as hexadecimal opcode
        int opcode;
        if (sscanf(input, "%x", &opcode) == 1) {
            Instruction* result = findByOpcode(root, opcode);
            if (result) {
                printf("Opcode 0x%02X -> Mnemonic: %s\n", result->opcode, result->mnemonic);
            } else {
                printf("Opcode 0x%02X not found\n", opcode);
            }
        } else {
            printf("Invalid opcode format\n");
        }
    } else {
        Instruction* result = findByMnemonic(root, input);
        if (result) {
            printf("Mnemonic %s -> Opcode: 0x%02X\n", result->mnemonic, result->opcode);
        } else {
            printf("Mnemonic '%s' not found\n", input);
        }
    }
    
    return 0;
}
	

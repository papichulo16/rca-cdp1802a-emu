#include <stdio.h>
#include <stdlib.h>
#include "../intf/token.h"

#include "../intf/tokenQueue.h"
// specific
#include "scanner.h"

#include "../intf/lexer.h"

#include "../intf/asm.h"

#include "../intf/assem.h"
struct Node
{ 
instr = String;
i = unsigned int;
n = int;
byte = unsigned int;
struct Node* next;
}
add_Node(String instruction,int i_value,int n_value){
int byte_v = 0;
    if(n_value == -1){
    byte_v = 2
}

    Node = (struct Node *)malloc(sizeof(byte_v));
new_Node->instru = instruction;
new_node -> i = i_value;
new_node -> n = n_value;
new_node -> byte = byte_v
if (*root == NULL){
    *root = new_nde;
}
else{
    Node* cur = *root;
    while(cur != NULL){
        if(cur->next == NULL){
            curr->next = new_node;
        }

        cur = cur-> next;
        
    }
}
return 0;
}
add_Node("IDL",0,0);
add_Node("LDN",0,-1);
add_Node("INC",1,-1)
add_Node("DEC",2,-1)
add_Node("Short Branch",3,-1)
add_Node("LDA",4,-1)
add_Node("STR",5,-1)
add_Node("IRX",6,0)
add_Node("OUT_1",6,1)
add_Node("OUT_2",6,2)
add_Node("OUT_3",6,3)
add_Node("OUT_4",6,4)
add_Node("OUT_5",6,5)
add_Node("OUT_6",6,6)
add_Node("OUT_7",6,7)
add_Node("INP_1",6,9);
add_Node("INP_2",6,0xA);
add_Node("INP_3",6,0xB);
add_Node("INP_4",6,0xC);
add_Node("INP_5",6,0xD);
add_Node("INP_6",6,0xE);
add_Node("INP_7",6,0xF);
add_Node("RET",7,0);
add_Node("DIS",7,1);
add_Node("LDXA",7,2);
add_Node("STXD",7,3);
add_Node("ADC",7,4);
add_Node("SDB",7,5);
add_Node("SHRC",7,6);
add_Node("SMB",7,7);
add_Node("SAV",7,8);

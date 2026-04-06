#include <stdio.h>
#include <stdlib.h>

#include "../intf/token.h"
#include "../intf/tokenLL.h"


void insertTK(tokenNode **head, Token toInsert){

  tokenNode *inserted = (tokenNode *)malloc(sizeof(tokenNode));

  inserted->data=toInsert;
  inserted->next=NULL;



  if(*head == NULL){
    *head=inserted;
    return;
  }

  tokenNode *curr=*head;


  while(curr->next != NULL){
    curr=curr->next;
  }

  curr->next=inserted;

  return;

}



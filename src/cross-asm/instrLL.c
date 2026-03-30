#include <stdio.h>
#include <stdlib.h>

#include "../intf/instrLL.h"


void insertLabel(labelNode **head, label toInsert){

  labelNode *inserted = (labelNode *)malloc(sizeof(labelNode));

  inserted->data=toInsert;
  inserted->next=NULL;



  if(*head == NULL){
    *head=inserted;
    return;
  }

  labelNode *curr=*head;


  while(curr->next != NULL){
    curr=curr->next;
  }

  curr->next=inserted;

  return;

}



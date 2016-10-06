#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../headers/index.h"
#include "../headers/index_impl.h"
#include "../headers/defn_impl.h"
#include "../headers/entryFunctions.h"
#include "../headers/entryList.h"
#include "../headers/payloadFunctions.h"
#include "../headers/queryIDFunctions.h"
#include "../headers/queryInfo.h"

/*Allocate space for each EntryListNode in Payload*/
void PAYLOAD_Allocate(Payload *payload){
    int i;
    for(i=0;i<3;i++){
        if((payload->threshold[i] = malloc(sizeof(struct EntryListNode))) == NULL){
           perror("Memory allocation of threshold");
           return;
        }
    }
       
}

/*Swap payload second to payload first*/
void PAYLOAD_Swap(Payload *first, Payload *second){
    
    Payload *temp=malloc(sizeof(struct Payload));
    int i;
    for(i=0;i<3;i++)
       /*set to null*/
       temp->threshold[i] = NULL;
    
    PAYLOAD_CopyToNode(temp,first);
    PAYLOAD_Free(first);
    for(i=0;i<3;i++)
       /*set to null*/
       first->threshold[i] = NULL;
    
    PAYLOAD_CopyToNode(first,second);
    PAYLOAD_Free(second);
    for(i=0;i<3;i++)
       /*set to null*/
       second->threshold[i] = NULL;
    
    PAYLOAD_CopyToNode(second,temp);
    
     PAYLOAD_Free(temp);
     free(temp);
}

#pragma GCC diagnostic warning "-w"
/*Copy payload from source to destination*/
void PAYLOAD_CopyToNode(Payload *dest,Payload *src){
    
    
    struct EntryListNode *tempdest,*tempsrc;

    int i;
    for(i=0;i<3;i++){
        /*copy payload with threshold 0*/
        if(src->threshold[i] != NULL){
                tempsrc = src->threshold[i];
                tempdest = dest->threshold[i];
                /*while the payload list not empty*/
                while(tempsrc->next != NULL){
                /*place the payload entry to the tempdestination*/
                        tempdest = ENTRYLIST_Insert(tempdest,tempsrc->entry,QUERYINFO_Allocate,QUERYINFO_CreateNode,QUERYINFO_Free);
                        tempsrc = tempsrc->next;
            
                }
                /*place the last node to the destnation payload*/
                tempdest = ENTRYLIST_Insert(tempdest,tempsrc->entry,QUERYINFO_Allocate,QUERYINFO_CreateNode,QUERYINFO_Free);
                /*move to the beginning of the temp destination*/
                while(tempdest->prev!=NULL){
                        tempdest = tempdest->prev;
                }
                /*assng the list to the destination*/
                dest->threshold[i] = tempdest;
        }
    }
}

/*free struct payload*/
void PAYLOAD_Free(Payload *payload){
    
    int i;  
    /*for every threshold in the payload struct*/
    for(i=0;i<3;i++){
        if(payload->threshold[i]!=NULL){
            /*free the list*/
           ENTRYLIST_Free(payload->threshold[i]);
        }
    }
    
}
/*Update the dest Payload with elements from the src*/
void PAYLOAD_Update(Payload *dest, Payload *src){

    int i;
    struct EntryListNode *tempdest;
    
    for(i=0;i<3;i++){
        /*if the source has elements in position i*/
        if(src->threshold[i] != NULL){
                /*if the position i in destination is empty*/
                if(dest->threshold[i] == NULL){
                        /*copy to that position*/
                        PAYLOAD_CopyToNode(dest,src);
            
                }
                else{
                        /*if there are already elements in that position*/
                        tempdest = dest->threshold[i];
                        /*get to the end of the elements*/
                        while(tempdest->next != NULL){
                                tempdest = tempdest->next;
                        }
                        /*add the node at the end*/
                        ENTRYLIST_Insert(tempdest ,src->threshold[i]->entry,QUERYINFO_Allocate,QUERYINFO_CreateNode,QUERYINFO_Free);
                }
        }
    }
    
}

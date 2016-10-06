#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../headers/index.h"
#include "../headers/index_impl.h"
#include "../headers/defn_impl.h"
#include "../headers/entryFunctions.h"
#include "../headers/entryList.h"
#include "../headers/wordFunctions.h"
#include "../headers/payloadFunctions.h"


void *ENTRYLISTNODE_Allocate(void){
    /* allocate memory for the node */
	void *temp;
	if((temp = malloc(sizeof(struct EntryListNode))) == NULL){
		fprintf(stderr, "Problem in memory allocation.\n");
		return NULL;
	}

	return temp;

}

int ENTRYLISTNODE_CreateNode(struct EntryListNode *dest, Entry *src){
    
    int i;
    dest->entry = malloc(sizeof(Entry));
    ((Entry*)(dest->entry))->word = WORD_Allocate();
    /*Create the node's entry using the src*/
    CreateWord(src->word->str, ((Entry*)(dest->entry))->word);
    
    /*allocate space for the paylaod struct*/
    ((Entry*)(dest->entry))->payload = malloc(sizeof(Payload));
    for(i=0;i<3;i++)
        /*set every list to NULL*/
        ((Entry*)(dest->entry))->payload->threshold[i] = NULL;
    /*Copy every entry from payload lists to destination*/
    PAYLOAD_CopyToNode(((Entry*)(dest->entry))->payload,src->payload);
    return 0;	

}

void ENTRYLISTNODE_Free(struct EntryListNode *node){
    
    if(node != NULL){
        /*free the str*/
        WORD_Free(((Entry*)(node->entry))->word);
        /*then free the word*/
	free(((Entry*)(node->entry))->word);
        
        /*free the payload lists*/
        if(((Entry*)(node->entry))->payload != NULL){
                PAYLOAD_Free(((Entry*)(node->entry))->payload);
                 /*then free the payload*/
                free(((Entry*)(node->entry))->payload);
        }
        /*free the entry*/
        free(node->entry);
        //free(node);
    }
    
    return;
    
}

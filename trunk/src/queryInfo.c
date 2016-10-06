#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../headers/core.h"
#include "../headers/queryInfo.h"
#include "../headers/queryIDFunctions.h"
#include "../headers/index.h"
#include "../headers/index_impl.h"

/*allocate space for the struct*/
void *QUERYINFO_Allocate(void){
        /* allocate memory for the node */
	void *temp;
	if((temp = malloc(sizeof(struct EntryListNode))) == NULL){
		fprintf(stderr, "Problem in memory allocation.\n");
		return NULL;
	}

	return temp;

}
 
/*Create the node's entry using the src*/ 
int QUERYINFO_CreateNode(struct EntryListNode *dest, struct QueryInfo *src){

    /*malloc size for the entry*/
    dest->entry = malloc(sizeof(struct QueryInfo));
    /*malloc size for the query id*/
    ((struct QueryInfo*)(dest->entry))->id = malloc(sizeof(QueryID));
    /*copy the id to the entry*/
    QUERYID_CreateNode(((struct QueryInfo*)(dest->entry))->id,src->id);
    /*assign the other members of the struct as well*/
    ((struct QueryInfo*)(dest->entry))->numOfQuery = src->numOfQuery;
    ((struct QueryInfo*)(dest->entry))->indexInQuery = src->indexInQuery;
 
    return 0;	

}


void QUERYINFO_Free(struct EntryListNode *node){
    
    if(node != NULL){
        /*free the id*/
        QUERYID_Free(((struct QueryInfo*)(node->entry))->id);
        free(((struct QueryInfo*)(node->entry)));
        /*free the entry*/
   //     free(node->entry);
    }
    return;
    
}

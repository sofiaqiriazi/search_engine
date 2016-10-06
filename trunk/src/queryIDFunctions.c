#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../headers/core.h"
#include "../headers/queryIDFunctions.h"
#include "../headers/index.h"
#include "../headers/index_impl.h"

/*Allocate space for the query id struct*/
void *QUERYID_Allocate(void){
        /* allocate memory for the node */
	void *temp;
	if((temp = malloc(sizeof(struct EntryListNode))) == NULL){
		fprintf(stderr, "Problem in memory allocation.\n");
		return NULL;
	}

	return temp;

}
/*copy from src to dest the id*/
int QUERYID_CreateNode(QueryID *dest, QueryID *src){
        /*Create the node's entry using the src*/
    memcpy(dest,src,sizeof(QueryID));
    
    return 0;	

}
/*free the id from the node*/
void QUERYID_Free(QueryID *id){
    
    if(id != NULL){
        /*free the entry*/
        free(id);
    }
    return;
    
}


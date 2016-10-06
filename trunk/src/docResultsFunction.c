#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../headers/core.h"
#include "../headers/queryIDFunctions.h"
#include "../headers/docResultsFunction.h"
#include "../headers/index.h"
#include "../headers/index_impl.h"


void *DOCRESULTS_Allocate(void){
        /* allocate memory for the node */
	void *temp;
	if((temp = malloc(sizeof(struct EntryListNode))) == NULL){
		fprintf(stderr, "Problem in memory allocation.\n");
		return NULL;
	}

	return temp;

}

/*Copy from 2nd argument to the 1st argument the results */
int DOCRESULTS_CreateNode(struct EntryListNode *dest, docResults *src){
	int i;
	dest->entry = malloc(sizeof( docResults  ) );
	((docResults *)(dest->entry) )->size = src->size;
	((docResults *)(dest->entry) )->res = malloc(sizeof( QueryID )* src->size );
	for( i = 0 ; i < src->size ; i++)
		//memcpy( ((docResults *)(dest->entry) )->res[i]  , src->res[i]  , sizeof( QueryID  )   );
		((docResults *)(dest->entry) )->res[i] = src->res[i];
	((docResults *)(dest->entry) )->docId =  src->docId;
}
/*Free the node's results*/
void DOCRESULTS_Free(docResults* dest ){
	free( dest->res  );
	free( dest  );
}




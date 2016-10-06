#ifndef DOCRESULTSFUNCTION_H_AGKETZG3
#define DOCRESULTSFUNCTION_H_AGKETZG3

typedef struct _docResults{
	QueryID *res;
	DocID docId;
	int size;
}docResults;	

/*Allocate space for QueryID*/
void *DOCRESULTS_Allocate(void);
/*Copy from 2nd argument to the 1st argument the id*/
int DOCRESULTS_CreateNode(struct EntryListNode*, docResults*);
/*Free the node's id*/
void DOCRESULTS_Free(docResults*);

#endif /* end of include guard: DOCRESULTSFUNCTION_H_AGKETZG3 */


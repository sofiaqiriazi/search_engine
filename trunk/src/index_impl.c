#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../headers/core.h"
#include "../headers/index.h"
#include "../headers/defn_impl.h"
#include "../headers/index_impl.h"
#include "../headers/wordFunctions.h"
#include "../headers/entryFunctions.h"
#include "../headers/entryList.h"
#include "../headers/indexFunctions.h"
#include "../headers/payloadFunctions.h"
#include "../headers/queryInfo.h"

#pragma GCC diagnostic warning "-w"
enum ErrorCode CreateEntry(const Word* word, Entry* entry) {
        int i;
        /*allocate space for the word*/
	entry->word = malloc(sizeof(Word));
	/*copy the contents to the entry*/
	CreateWord(word->str, entry->word); 
        /*allocate space for the payload lists*/
	entry->payload = malloc(sizeof(Payload));
        for(i=0;i<3;i++)
                /*set to null*/
                entry->payload->threshold[i] = NULL;
	return EC_SUCCESS;
}

enum ErrorCode AddEntry(EntryList* entryList, const Entry* entry) {

	struct EntryListNode *lastNode;
	entryList->size++;
	/*If the entry is the first to be added in the list*/
	if(entryList->start == NULL){
		/*Insert the entry and return it as a lastnode*/
		lastNode = ENTRYLIST_Insert(entryList->end, entry, ENTRYLISTNODE_Allocate, ENTRYLISTNODE_CreateNode, ENTRYLISTNODE_Free);
		entryList->start = (struct EntryListNode*)lastNode;
		entryList->end = (struct EntryListNode*)lastNode;               /*make start,end,current point at the entry*/
		entryList->currentEntry = (struct EntryListNode*)lastNode;

	}
	else{
		/*Insert the entry and return it as a lastnode*/
		lastNode = ENTRYLIST_Insert(entryList->end, entry, ENTRYLISTNODE_Allocate, ENTRYLISTNODE_CreateNode, ENTRYLISTNODE_Free);
		/*this is now the last node of the list and the end points to it*/
		entryList->end = (struct EntryListNode*)lastNode;
	}



	return EC_SUCCESS;
}

enum ErrorCode DestroyEntry(Entry *entry) {
	/* If the entry word is not empty*/
	if(entry->word != NULL){
		/*Free the string if the word*/
		WORD_Free(entry->word);
		/*Free the word struct*/
		free(entry->word);
	}
	/* If the entry payload is not empty*/
	if(entry->payload != NULL){
                /*Free the payload lists*/
                PAYLOAD_Free(entry->payload);
                /*free the payload struct*/
	        free(entry->payload);
	   }

	return EC_SUCCESS;
}

enum ErrorCode CreateEntryList(EntryList* entryList) {
	/*make the start,end and current to point at NULL*/
	entryList->size = 0;
	entryList->start = NULL;
	entryList->end = NULL;
	entryList->currentEntry = NULL;

	return EC_SUCCESS;
}

Entry* GetLast(EntryList* entryList) {
	/*If empty return NULL*/
	if(entryList->start == NULL){
		return NULL;
	}
	/*make the current to point to the node after the starting node */
	/*return the starting node*/
	return entryList->end->entry;
}


Entry* GetFirst(EntryList* entryList) {
	/*If empty return NULL*/
	if(entryList->start == NULL){
		return NULL;
	}
	/*make the current to point to the node after the starting node */
	entryList->currentEntry = entryList->start->next;
	/*return the starting node*/
	return entryList->start->entry;
}

struct EntryListNode * GetFirstNode(EntryList* entryList) {
	/*If empty return NULL*/
	if(entryList->start == NULL){
		return NULL;
	}
	/*make the current to point to the node after the starting node */
	entryList->currentEntry = entryList->start->next;
	/*return the starting node*/
	return entryList->start;
}

Entry* GetNext(EntryList* entryList) {
	/*If you are already at the end of the list or the current points to NULL*/
	if(entryList->currentEntry == entryList->end || entryList->currentEntry == NULL ){
		/*return the last node entry*/
		return entryList->end->entry;
	}
	/*make the current to point to the node after it*/
	entryList->currentEntry = entryList->currentEntry->next;
	/*return the previous position entry that the current pointed to*/
	return entryList->currentEntry->prev->entry;
}


struct EntryListNode* GetNextNode(EntryList* entryList) {
	/*If you are already at the end of the list or the current points to NULL*/
	if(entryList->currentEntry == entryList->end || entryList->currentEntry == NULL ){
		/*return the last node */
		return entryList->end;
	}
	/*make the current to point to the node after it*/
	entryList->currentEntry = entryList->currentEntry->next;
	/*return the previous position  that the current pointed to*/        
	return entryList->currentEntry->prev;
}

enum ErrorCode DestroyEntryList(EntryList* entryList) {
	/*call the function to free the entryListNodes by passing the first node of the list*/
	ENTRYLIST_Free(entryList->start);
	entryList->start = NULL;
	entryList->end = NULL;

	return EC_SUCCESS;
}

unsigned int GetNumberOfEntries(const EntryList* entryList) {
	return entryList->size;
	unsigned int numOfEntries = 0;
	struct EntryListNode *temp;
	/*If the list is empty*/
	if(entryList->start == NULL){
		return 0;
	}

	/*point to the start*/
	temp = entryList->start;
	/*if not null*/
	while(temp->next!= NULL){
		/*increase the number of entries*/
		numOfEntries++;
		/*point to the next*/
		temp = temp->next;
	}
	/*increase by one for the last node*/
	return ++numOfEntries;

}
unsigned int GetNumberOfEntriesOtouN(const EntryList* entryList) {
	unsigned int numOfEntries = 0;
	struct EntryListNode *temp;
	/*If the list is empty*/
	if(entryList->start == NULL){
		return 0;
	}

	/*point to the start*/
	temp = entryList->start;
	/*if not null*/
	while(temp->next!= NULL){
		/*increase the number of entries*/
		numOfEntries++;
		/*point to the next*/
		temp = temp->next;
	}
	/*increase by one for the last node*/
	return ++numOfEntries;

}

void SwapEntries(struct Entry *nodeA, struct Entry *nodeB){

	struct Entry *temp = malloc(sizeof(struct Entry));                                                         /*create temp entry*/

	/*If the nodes are not the same*/
	if(nodeA != nodeB){
		/*swap their word structs*/
		CreateEntry(nodeA->word, temp);                                      /*save the nodeA's word in the temp entry*/
		WORD_Cpy(nodeA->word, nodeB->word);                              /*save the nodeB's word in the nodeA entry*/
		WORD_Cpy(nodeB->word, temp->word);                                      /*save the temp word in the nodeB entry*/
		PAYLOAD_Swap(nodeA->payload, nodeB->payload);                           /*swap their payloads*/
                DestroyEntry(temp);                                             
                
	}
        
	free(temp);
	return EC_SUCCESS;                          

}

struct EntryList* GetEntryList(struct EntryListNode *start,struct EntryListNode *end){

	struct EntryList *subList = malloc(sizeof(struct EntryList));                                                  
	struct EntryListNode *temp = start;
	subList->start = start;                                                     /*set the start to the entryListNode start*/
	subList->end = end;                                                         /*set the end to the entryListNode end*/
	subList->currentEntry = subList->start;                                     /*set the currentEntry to the entryListNode start*/
	int len = 1;
	while( temp != end  ){
		len++;
		temp = temp->next;
	}
	subList->size = len;
	return subList;
}

//void ResetEntryList(EntryList *entries)
//{
//entries->currentEntry = entries->start;
//}

enum ErrorCode BuildEntryIndex(EntryList* entries,
		enum MatchType type,
		Index* index) {
	EntryList *less,*more;
	int VPPos;
	Entry *temp;

	index->type = type;
	temp = FindVP( entries , index->type , &VPPos );
	/*find vp*/
	index->vantagePoint = malloc(sizeof(Entry));
	CreateEntry( temp->word , index->vantagePoint );
	PAYLOAD_CopyToNode( index->vantagePoint->payload ,temp->payload	);
	DestroyEntry(temp);
	free(temp);
	//printf("Entries: %d\n", GetNumberOfEntries(entries));
	QuickSelectInit(entries,index->vantagePoint, type , &less, &more, &(index->median));

	/*
	 * recursion...
	 */
	/*bulid left child*/
	if(GetNumberOfEntries(less)>0){
		index->left = malloc(sizeof(Index));
		if(index->left == NULL)return EC_FAIL;
		BuildEntryIndex(less,type,index->left);
	}
	else{
		index->left = NULL;
	}
	/*build right child*/
	if(GetNumberOfEntries(more)>0){
		index->right = malloc(sizeof(Index));
		if(index->right == NULL)return EC_FAIL;
		BuildEntryIndex(more,type,index->right);
	}
	else{
		index->right = NULL;
	}

	DestroyEntryList(less);
	DestroyEntryList(more);
	free(less);
	free(more);
	return EC_SUCCESS;
}

enum ErrorCode DestroyEntryIndex(Index* index) {

	if(index==NULL)
		printf("op\n");
	if( index->left != NULL ){
		DestroyEntryIndex(index->left);
	}

	if( index->right != NULL ){
		DestroyEntryIndex(index->right);
	}

	DestroyEntry(index->vantagePoint);
	free(index->vantagePoint);
	free(index->left);
	free(index->right);

	return EC_SUCCESS;
}


enum ErrorCode LookupEntryIndex(const Word* word,
		int threshold,
		const Index* index,
		EntryList* result) {
	if (index == NULL)
		return EC_SUCCESS;
	int dist=Distance(word,index->vantagePoint->word,index->type);
	/* if word is close enough add it to the results */
	if( dist <= threshold && dist >=0){
		struct QueryInfo *qi;
		struct EntryListNode *eln;
		int i;
		AddEntry(result , index->vantagePoint);
		( (Entry*)(result->end->entry) )->payload->dist = dist;
		//for( i = 0 ; i < 3 ; i++){
			//eln = index->vantagePoint->payload->threshold[i];
			//printf("threshold%d:",i);
			//while( eln!= NULL)
			//{
				//qi =(struct QueryInfo*)(eln->entry);
				//printf("%d ,%d " , qi->numOfQuery , qi->indexInQuery);
				//eln = eln->next;
			//}
			//printf("||||||\n");
		//}
		//printf("%s d=%d\n\n", index->vantagePoint->word->str , dist  );
	}

	/* if it is inside the circle (with threshold relaxation), search left */
	if( dist <= threshold + index->median )
		LookupEntryIndex(word,threshold,index->left,result);

	/* if it is outside the circle, search right  */
	if( index->median <= threshold + dist )
		LookupEntryIndex(word,threshold,index->right,result);


	return EC_SUCCESS;
}





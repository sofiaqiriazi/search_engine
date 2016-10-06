#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../headers/entryList.h"
#include "../headers/index.h"
#include "../headers/index_impl.h"
#include "../headers/entryFunctions.h"

void *ENTRYLIST_Initialize(void *(*AllocateStructureSpace)(), int (*InsertStructure)(void *, void *), void (*FreeStructure)(void *))  
{
	struct EntryListNode *node;
	/*check if the allocation for the EntryListNode is successful*/
	if((node = (struct EntryListNode*) AllocateStructureSpace())== NULL){
		fprintf(stderr, "Problem in entry list allocation.\n");
		return NULL;
	}

	/*make the new nodes prev and next to point to null*/
	node->next = NULL;
	node->prev = NULL;

	node->AllocateStructureSpace = AllocateStructureSpace;                  
	node->InsertStructure = InsertStructure;                                /*Assign the functions*/
	node->FreeStructure = FreeStructure;                                    

	return node;
}

void ENTRYLIST_Free(struct EntryListNode *node)
{
	/*if the node is empty return*/
	if(node == NULL){
		return;
	}

	/*for each node until the last node*/
	while(node->next != NULL){
		/*point to the next node*/
		node = node->next;
		node->FreeStructure(node->prev);
		free(node->prev);
	}

	/*free the last node*/
	node->FreeStructure(node);    
	free(node);
	return;
}

void *ENTRYLIST_Insert(struct EntryListNode *node, void *data, void *(*AllocateStructureSpace)(), int (*InsertStructure)(), void (*FreeStructure)())
{
	struct EntryListNode *nodeToAdd;

	/*if the node points to NULL*/
	if( node == NULL ){
		/*create a new node*/
		node = (struct EntryListNode* )ENTRYLIST_Initialize( AllocateStructureSpace, InsertStructure, FreeStructure);
		/*make the new node to contain the data given*/
		(node->InsertStructure)( node, data);
		return node;
	}
	/*create a new node*/
	nodeToAdd = (struct EntryListNode* )ENTRYLIST_Initialize( AllocateStructureSpace, InsertStructure, FreeStructure);
	/*make sure the Insert was correct*/
	if( ((nodeToAdd->InsertStructure)( nodeToAdd, data)) == -1 ){
		fprintf(stderr, "Problem in entry list allocation.\n");
		return NULL;        
	}

	/*make the node(the entryList->end)'s next to point to the new node */
	node->next = nodeToAdd;
	/*make the node's to add prev to point to the ending node*/
	nodeToAdd->prev = node;
	/*make the next to point to NULL*/
	nodeToAdd->next = NULL;

	return nodeToAdd;
}

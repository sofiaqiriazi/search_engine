/* 
 * File:   entryList.h
 * Author: sofia
 *
 * Created on January 28, 2014, 12:50 PM
 */

#ifndef ENTRYLIST_H
#define	ENTRYLIST_H

struct EntryListNode{
        void *entry;
	struct EntryListNode *next;
	struct EntryListNode *prev;
	/* the function that allocates the space for a new data NODE 
	 * RETURNS a pointer to it
	 * 	   NULL in case of failure */
	void *(*AllocateStructureSpace)();		
	/* the function that inserts the data to the pointer of the node
	 * 1st argument: a pointer to the structure that we want to insert the data
	 * 2nd argument: a pointer to the data
	 * RETURNS: 1 in case of success, 
	 * 	    -1 in case of failure */
	int (*InsertStructure)();		
	/* the function that deletes the data from a pointer of the bucket
	 * 1st argument: a pointer to the node of the structure */
	void (*FreeStructure)();			
};

/*Initialize the node to be place in the entrylist by pointing to NULL and assigning the proper functions*/
void *ENTRYLIST_Initialize(void *(*AllocateStructureSpace)(), int (*InsertStructure)(void *, void *), void (*FreeStructure)(void *));
/*Free the entrylist*/
void ENTRYLIST_Free(struct EntryListNode *);
/*Insert the new node to the list after coping the data to the node*/
void *ENTRYLIST_Insert(struct EntryListNode *, void *,void *(*AllocateStructureSpace)(), int (*InsertStructure)(), void (*FreeStructure)());

#endif	/* ENTRYLIST_H */

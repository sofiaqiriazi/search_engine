/* 
 * File:   entryFunctions.h
 * Author: sofia
 *
 * Created on January 28, 2014, 10:51 AM
 */

#ifndef ENTRYFUNCTIONS_H
#define	ENTRYFUNCTIONS_H



/*Allocate space for the first arguments data*/
void *ENTRYLISTNODE_Allocate(void);
/*Copy from 2nd argument to the 1st argument the data*/
int ENTRYLISTNODE_CreateNode(struct EntryListNode *, Entry*);
/*Free the node*/
void ENTRYLISTNODE_Free(struct EntryListNode *);
/*hash of entry*/
uint32_t EntryHash(Entry *);




#endif	/* ENTRYFUNCTIONS_H */


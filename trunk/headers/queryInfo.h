/* 
 * File:   QueryInfo.h
 * Author: sofia
 *
 * Created on March 2, 2014, 7:07 PM
 */

#ifndef QUERYINFO_H
#define	QUERYINFO_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "entryList.h"
#include "core.h"
    
struct QueryInfo{
        QueryID *id;
        int numOfQuery;
        int indexInQuery;
};

   
/*Allocate space for the node*/
void *QUERYINFO_Allocate(void);
/*Copy to the node the struct QueryInfo*/
int QUERYINFO_CreateNode(struct EntryListNode *,struct QueryInfo*);
/*Free the node*/
void QUERYINFO_Free(struct EntryListNode *);

#ifdef	__cplusplus
}
#endif

#endif	/* QUERYINFO_H */


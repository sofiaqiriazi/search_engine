/* 
 * File:   QueryIDFunctions.h
 * Author: sofia
 *
 * Created on February 22, 2014, 3:05 PM
 */


#ifndef QUERYIDFUNCTIONS_H
#define	QUERYIDFUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "entryList.h"
    
/*Allocate space for QueryID*/
void *QUERYID_Allocate(void);
/*Copy from 2nd argument to the 1st argument the id*/
int QUERYID_CreateNode(QueryID *, QueryID *);
/*Free the node's id*/
void QUERYID_Free(QueryID *);


#ifdef	__cplusplus
}
#endif

#endif	/* QUERYIDFUNCTIONS_H */


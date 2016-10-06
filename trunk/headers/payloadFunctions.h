/* 
 * File:   payloadFunctions.h
 * Author: sofia
 *
 * Created on February 22, 2014, 4:34 PM
 */

#ifndef PAYLOADFUNCTIONS_H
#define	PAYLOADFUNCTIONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "index.h"

    
/*Allocate space for each EntryListNode in Payload*/
void PAYLOAD_Allocate(Payload *);

/*Swap the payloads*/
void PAYLOAD_Swap(Payload *, Payload *);

/*copy payload lists from source to destination*/
void PAYLOAD_CopyToNode(Payload *dest,Payload *src);

/*free struct payload*/
void PAYLOAD_Free(Payload *);

/*Update the payload lists with the new payloads from src*/
void PAYLOAD_Update(Payload *dest,Payload *src);

#ifdef	__cplusplus
}
#endif

#endif	/* PAYLOADFUNCTIONS_H */


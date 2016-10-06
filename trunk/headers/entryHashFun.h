
#ifndef ENTRYHASHFUNCTIONS_H
#define	ENTRYHASHFUNCTIONS_H


void *ENTRY_Allocate(void);

int ENTRY_Cpy(Entry *dest,const Entry *src);

void ENTRY_Free(Entry *e);

int ENTRY_Cmp(Entry *e1, Entry *e2);

void ENTRY_Update(Entry *dest,Entry *src);

#endif

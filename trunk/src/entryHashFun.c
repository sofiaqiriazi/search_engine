#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../headers/index_impl.h"
#include "../headers/defn_impl.h"
#include "../headers/wordFunctions.h"
#include "../headers/payloadFunctions.h"

uint32_t EntryHash(Entry *entry)
{
        return FNV32(entry->word->str);
} 


void *ENTRY_Allocate(void)
{
        void *temp = malloc(sizeof(Entry));
        return temp;
}

int ENTRY_Cpy(Entry *dest,const Entry *src)
{
        CreateEntry(src->word, dest);
        PAYLOAD_CopyToNode(dest->payload,src->payload);
        return 0;
}

void ENTRY_Free(Entry *e)
{
	DestroyEntry(e);
        return;
}

int ENTRY_Cmp(Entry *e1, Entry *e2)
{
        return strcmp(e1->word->str, e2->word->str);
}

void ENTRY_Update(Entry *dest,Entry *src)
{
    //update payload
    PAYLOAD_Update(dest->payload,src->payload);
    return;
}
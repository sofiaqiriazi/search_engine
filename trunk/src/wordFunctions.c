#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../headers/defn.h"
#include "../headers/defn_impl.h"
#include "../headers/wordFunctions.h"

uint32_t FNV32(const char *s)
{
	uint32_t hash = FNV_OFFSET_32, i;
	for(i = 0; i < strlen(s); i++) {
		hash = hash ^ (s[i]); // xor next byte into the bottom of the hash
		hash = hash * FNV_PRIME_32; // Multiply by prime number found to work well
	}
	return hash;
} 


uint32_t wordHash(Word *word)
{
	return FNV32(word->str);
}


void *WORD_Allocate(void)
{
	void *temp = malloc(sizeof(Word));
	return temp;
}

int WORD_Cpy(Word *dest,const Word *src)
{
//	WORD_Free(dest);
	CreateWord(src->str, dest); 
	return 0;	
}

void WORD_Free(Word *word)
{
    free(word->str);
	word->str = NULL;
	return;	
}

int WORD_Cmp(Word *word1, Word *word2)
{
	return strcmp(word1->str, word2->str);
}

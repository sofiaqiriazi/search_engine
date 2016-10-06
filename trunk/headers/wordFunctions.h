#ifndef WORDFUNCTIONS_H
#define WORDFUNCTIONS_H

#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U

void *WORD_Allocate(void);
int WORD_Cpy(Word *,const Word *);
void WORD_Free(Word *);
int WORD_Cmp(Word *, Word *);
uint32_t FNV32(const char *s);
uint32_t wordHash(Word *word);
#endif

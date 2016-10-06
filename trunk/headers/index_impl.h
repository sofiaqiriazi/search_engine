#ifndef INDEX_IMPL_H
#define	INDEX_IMPL_H

#include "index.h"
#include "entryList.h"

struct Index {
  enum MatchType type;
  Entry *vantagePoint;
  Index *left;
  Index *right;
  int median;
};

struct EntryList {
    unsigned int size;
    struct EntryListNode *start;
    struct EntryListNode *end;
    struct EntryListNode *currentEntry; 
};

#endif	/* INDEX_IMPL_H */

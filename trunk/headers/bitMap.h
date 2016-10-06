#ifndef _BitMap_H
#define _BitMap_H

typedef struct bitMap{
	unsigned int size;
	unsigned int numOfElements;
	int *posMap;
	char* bitArray; 
}bitMap;



void InitBitMap(EntryList *, bitMap *map);
void SetElementsBit(bitMap *map, int , int );
void CheckBitMap(bitMap *map , results * );
void DestroyBitMap( bitMap *map  );
void SetBitsFromList( bitMap * , EntryList * );

#endif



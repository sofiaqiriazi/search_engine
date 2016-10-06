#include <stdio.h>
#include <stdlib.h>
#include "../headers/index_impl.h"
#include "../headers/defn_impl.h"
#include "../headers/entryList.h"
#include "../headers/docResultsFunction.h"
#include "../headers/queryInfo.h"
#include "../headers/entryList.h"
#include "../headers/cache.h"
#include "../headers/thread.h"

#include "../headers/bitMap.h"
int CheckBitsFromTo(bitMap *map ,int from ,int to  );


//typedef enum { false, true } bool;

/* return the ceiling of x/(2^y)  */
#define CEILING(x,y)  ( ( (x) >> (y) ) + ( ( (x)&( (1>>(y)) - 1  ) ) != 0 ) )
/* set the y bit of array x to 1 */
#define SETBIT( x, y  )  ( x[ (y)>>3 ] |= ( 1 << ( (y)%8  ) ) )

#define SETBITSFROMTO( ch , from , to  )	\
	ch = 1 <<( to - from );			\
	ch -= 1;				\
	ch <<= from;				\
	//ch |= from==to;
	



void printCharBits(char c)
{
	unsigned char *b;
        unsigned char byte;
	int j,t;	
	b = (unsigned char*)& (c );
	for (j=0,t=0;j<=7;j++,t++) {
		byte = b[0] & (1<<j);
		byte >>= j;
		printf("%u", byte);
 	}
	printf("\n");

}


void printBitArray(bitMap *map)
{
	int i,j,p=0,t;
	unsigned char *b;
        unsigned char byte;

	printf("----------");
	for( i = 0  ; i < (map->size ) ; i++) {
		
		b = (unsigned char*)& (map->bitArray[i] );
		for (j=0,t=0;j<=7;j++,t++) {
			if(map->posMap[p] == i*8+t)printf("\n%d|",p++);
			byte = b[0] & (1<<j);
			byte >>= j;
			printf("%u", byte);
		}
	}

	printf("\n%d----------\n",map->numOfElements);

}

void InitBitMap(EntryList *queries,bitMap *map ) {
	int i, t, numOfWords,prevCount;
	Entry *curr;
	map->numOfElements = GetNumberOfEntries(queries);
	map->posMap = malloc( sizeof(int) * ( map->numOfElements + 1 ));

	curr = GetFirst(queries);
	numOfWords = 0;
	for ( i = 0 ; i < map->numOfElements ; i++ ) {
		map->posMap[i] = numOfWords;
		prevCount = numOfWords;
		t = 0;
		while( curr->word->str[t] != '\0' )
			if( curr->word->str[t++] == ' ' )
				numOfWords++;
		//printf("%d\n",numOfWords- prevCount);
		//need it for tiny/small not for medium
		//numOfWords++;
		//printf("curr %d=%s\n",numOfWords - prevCount,curr->word->str);		
		
		curr = GetNext( queries );
	}
	map->posMap[i] = numOfWords;


	//map->size = ( (numOfWords - 1) >> 3) + ( ( (counter - 1)&(7) ) != 0 );
	map->size = CEILING(numOfWords - 1 , 3);
	map->bitArray = malloc( sizeof(char) * map->size );
	
	for ( i = 0 ; i < map->size ; i++  )
		map->bitArray[i] = 0;
	
	//printf("bits=%d\n",map->size);
	//printBitArray( map );
}

void SetElementsBit(bitMap *map, int row , int col) {
	int actualPos = map->posMap[ row  ] + col;
	//printf("%d,%d\n", pos.row , pos.col  );

	//map->bitArray[ actualPos>>3 ] |= 1 << ( actualPos%8 );
	SETBIT( map->bitArray , actualPos  );
	//printBitArray( map );
}

void SetBitsFromList( bitMap *map , EntryList *list  ){
	int i , j;
	int co =0;
	Entry *e = GetFirst( list );
	struct EntryListNode *curr;
	struct QueryInfo *qInfo;
	for( i = 0 ; i < GetNumberOfEntries( list ) ; i++ ){
		for( j = (e->payload->dist == 0 ? 0 : e->payload->dist-1  ); j < 3  ; j++ ){
			curr = e->payload->threshold[j];
			while( curr !=NULL ){
				co++;
				qInfo =(struct QueryInfo *)(curr->entry);
				//printf("%d,%d\n",qInfo->numOfQuery , qInfo->indexInQuery );
				SetElementsBit( map , qInfo->numOfQuery , qInfo->indexInQuery  );
				curr = curr->next;
			}
		}
		e = GetNext( list );
	}
	//printf("count:%d:\n",co);
}



void CheckBitMap(bitMap *map , results  *r )
{
	int i;
	int from;
	int to;
	int count;
	int offset =1;
	r->resCount= 0;
	for( i = 0 ; i < map->numOfElements ; i++ ) {
		from = map->posMap[i];
		to =  map->posMap[i+1];
		if( 1 == CheckBitsFromTo( map , from , to ) ){
			r->resCount++;
			//printf("%d\t",i+offset);
		}
	}
	r->res = malloc( sizeof(QueryID) * r->resCount  );
	//printf("%d\n",r->resCount );
	for( i = 0 , count =0 ; i < map->numOfElements ; i++ ) {
		from = map->posMap[i];
		to =  map->posMap[i+1];
		if( 1 == CheckBitsFromTo( map , from , to ) )
			r->res[count++] = i + offset;
	}
	//printf("\n---------------------\n");
}

int CheckBitsFromTo(bitMap *map ,int from ,int to  )
{
	int i;
	char c1,c2;
	int index;
	int flag = 1;
	
	index = from;
	for ( i = from ; ( i >> 3 ) < ( ( to - 1 ) >> 3 ) ; i+=8 ) {
		SETBITSFROMTO( c1 , index%8 , 8 );
		c2 = map->bitArray[ i >> 3 ];
		flag &= ( ( c1 & c2 ) == c1 );
		index = 0;
	}
	
	SETBITSFROMTO( c1 , index%8 , to%8 );
	c2 = map->bitArray[ i >> 3 ];
	flag &= ( ( c1 & c2 ) == c1 );
	return flag;
}

/***** debug version  ************
int CheckBitsFromTo(bitMap *map ,int from ,int to  )
{
	int i;
	char c1,c2;
	int index;
	int flag = 1;

	index = from;
	printf("start:%d,end:%d\n",from,to);
	for ( i = from ; ( i >> 3 ) < ( ( to - 1 ) >> 3 ) ; i+=8 ) {
	//while( ( index >> 3 ) < ( to >> 3 ) ) {
		SETBITSFROMTO( c1 , index%8 , 8 );
		printf("from=%d,to=%d,i=%d\n",index%8,8,i);
		c2 = map->bitArray[ i >> 3 ];
		flag &= ( ( c1 & c2 ) == c1 );
	//printCharBits(c1);
	//printCharBits(c2);
		index = 0;
	}
	//printf( "actual  :%d,%d\n", ( index%8  ), ( to%8  )) ;
	SETBITSFROMTO( c1 , index%8 , to%8 );
	c2 = map->bitArray[ i >> 3 ];
		//printf("index::::%d\n",i>>3);
	flag &= ( ( c1 & c2 ) == c1 );
	//printCharBits(c1);
	//printCharBits(c2);
	//printf("---- flag = %d -----\n",flag);
	return flag;

}


*/

void DestroyBitMap( bitMap *map ) {
	//printBitArray( map );
	free( map->posMap );
	free( map->bitArray );
}


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "../headers/arrayBucket.h"
#include "../headers/defn.h"
#include "../headers/defn_impl.h"
#include "../headers/index_impl.h"

void *ARRAY_Initialize(size_t size, void *(*AllocateStructureSpace)(), int (*InsertStructure)(void *, void *), void (*FreeStructure)(void *), int (*Compare)(void *, void *),void (*Update)(void *,void *))  
{
	struct arrayBucket *bucket;
	if((bucket = malloc(sizeof(struct arrayBucket))) == NULL){
		fprintf(stderr, "Problem in array allocation.\n");
		return NULL;
	}
	bucket->size = 512;							/*				       */
	bucket->lastElement = -1;						/* obvious initializations are obvious */
	bucket->AllocateStructureSpace = AllocateStructureSpace;		/*				       */
	bucket->InsertStructure = InsertStructure;				/*				       */
	bucket->FreeStructure = FreeStructure;					/*				       */
	bucket->Compare = Compare;
        bucket->Update = Update;

	if((bucket->array = malloc(size*sizeof(void *))) == NULL){
		fprintf(stderr, "Problem in array allocation.\n");
		return NULL;
	}

	return bucket;
}

void ARRAY_Free(struct arrayBucket *bucket)
{
	int i;
	for(i = 0 ; i < bucket->lastElement + 1 ; i++){
		(*bucket->FreeStructure)(bucket->array[i]);	/* let the appropriate function handle the freeing of the internal structure */
		free(bucket->array[i]);
	}
	free(bucket->array);
}

int ARRAY_FindDataInBucket(struct arrayBucket *bucket, void *data)
{
        int binaryElementFirst = 0, binaryElementLast = bucket->lastElement, binaryElement;
        int cmpResult;
        if((*bucket->Compare)(data,bucket->array[0]) < 0){ /* CASE value should go first in order */
                return 0;
        }
        else if((*bucket->Compare)(data,bucket->array[bucket->lastElement]) > 0){ /* CASE value should go last in order */
                return bucket->lastElement+1;
        }
        else{                           /* else initiate binary search algorithm */
                binaryElement = (binaryElementFirst + binaryElementLast)/2;
                while((cmpResult = (*bucket->Compare)(data,bucket->array[binaryElement])) != 0 && binaryElementFirst <= binaryElementLast){
                        if(cmpResult > 0){
                                binaryElementFirst = binaryElement+1;
                        }
                        else{
                                binaryElementLast = binaryElement-1;
                        }
                        binaryElement = (binaryElementFirst + binaryElementLast)/2;
                	if(cmpResult == 0){
				//printf("%d\n",binaryElement );
                        	return -binaryElement-1;/* if ellement is found retrun a neg num */
			}
                }
                if(cmpResult == 0){
			//printf("%d\n",binaryElement );
                        return -binaryElement-1;/* if ellement is found retrun a neg num */
		}
        }
        return binaryElement + 1;

}

int AllocateMoreSpace( struct arrayBucket *bucket )
{
	void *temp;
	if((temp = malloc(bucket->size)) == NULL){      /* create backup table */
		fprintf(stderr, "Could not allocate memory for backup array.\n");
		return -1;
	}
	if((memcpy(temp, bucket->array, bucket->size)) == NULL){ /* copy table to backup table */
		fprintf(stderr, "Could not copy data to backup array.\n");
		return -1;
	}
	if((bucket->array = malloc(bucket->size*2)) == NULL){   /* double the previous array */
		fprintf(stderr, "Could not double the memory of the array.\n");
		return -1;
	}
	if((memcpy(bucket->array, temp, bucket->size)) == NULL){ /* copy the previous array to the new one */
		fprintf(stderr, "Could not copy data to new array.\n");
		return -1;
	}
	bucket->size *= 2;
	free(temp);
	return 0;
}

int ARRAY_Insert(struct arrayBucket *bucket, void *data)
{
	int binaryElement,test; 
	int prevIter = -1;
	
	if(bucket->lastElement < 0){ 			/* in case bucket is empty */
		if((bucket->array[bucket->lastElement+1] = (*bucket->AllocateStructureSpace)()) == NULL){ /* create it */
			fprintf(stderr, "Could not allocate memory for structure.\n");
			return -1;
		}
		bucket->lastElement++;
		if((*bucket->InsertStructure)(bucket->array[bucket->lastElement], data) == -1){		/* the functions that inserts the data to the bucket */
			fprintf(stderr, "Could not insert structure into the hash.\n");
			return -1;
		}	
		return 1;
	}	

	else{
		binaryElement = ARRAY_FindDataInBucket( bucket , data  );
		if ( binaryElement < 0 )
			return 0;
	}
	if(bucket->size == (size_t)bucket->lastElement+1){ 	/* in case bucket is full */
		test = AllocateMoreSpace(bucket);
		assert( test == 0 );
	}
	for(prevIter = bucket->lastElement ; prevIter >= binaryElement ; prevIter--)	/* shift values after the position by 1 */
		bucket->array[prevIter + 1] = bucket->array[prevIter];
	if((bucket->array[binaryElement] = (*bucket->AllocateStructureSpace)()) == NULL){
		fprintf(stderr, "Could not allocate memory for structure.\n");
		return -1;
	}

	if((*bucket->InsertStructure)(bucket->array[binaryElement], data) == -1){		/* the functions that inserts the data to the bucket */
		fprintf(stderr, "Could not insert structure into the hash.\n");
		return -1;
	}	
	bucket->lastElement++;
	return 1;
	
}

void* ARRAY_Search(struct arrayBucket *bucket, void *data)
{
	int binaryElement;
	binaryElement =-1-ARRAY_FindDataInBucket( bucket , data  );
	if ( binaryElement < 0 )
		return NULL;
	return bucket->array[binaryElement];
}

void ARRAY_Update(struct arrayBucket *bucket, void *data, int t){

    (*bucket->Update)(bucket->array[t],data);
    return;
}

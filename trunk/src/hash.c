#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/hash.h"
#include "../headers/defn.h"
#include "../headers/defn_impl.h"
#include "../headers/arrayBucket.h"
#include "../headers/index.h"


//#include "../headers/index.h"
//#include "../headers/arrayBucket.h"


int HASH_Initialize(
		 struct hashTable *ht,
		 unsigned int numberOfElements,
		 uint32_t (*HashFunction)(),
		 void *(*AllocateStructureSpace)(), 
		 int (*InsertStructure)(),

		 int (*SearchStructure)( ),

		 void (*FreeStructure)(),
                 void (*UpdateStructure)(),
		 void *(*AllocateValueSpace)(),
		 int (*InsertValue)(),
		 void (*FreeValue)(),
		 int (*Compare)(),
                 void (*Update)()
		)
{
	int i, errorFlag = 0;

	ht->sequenceNumber = INITIAL_BUCKET_NUMBER;				/*				       */
	ht->numberOfBuckets = CENTERED_DECAGONAL_PRIME(ht->sequenceNumber);	/*				       */
	ht->HashFunction = HashFunction;					/* obvious initializations are obvious */
	ht->AllocateStructureSpace = AllocateStructureSpace;			/*				       */
	ht->InsertStructure = InsertStructure;					/*				       */

	ht->SearchStructure = SearchStructure;

	ht->FreeStructure = FreeStructure;					/*				       */
        ht->UpdateStructure = UpdateStructure;
	ht->AllocateValueSpace = AllocateValueSpace;		/* the function that allocates the space for a new bucket */
	ht->InsertValue = InsertValue;		/* the functions that inserts the data to the bucket */
	ht->FreeValue = FreeValue;			/* the functions that deletes a bucket */
	ht->Compare = Compare;
        ht->Update = Update;
        
	while(	   ht->numberOfBuckets < numberOfElements 
		&& ht->sequenceNumber < MAXIMUM_BUCKET_NUMBER
	   ){ /* find the smallest number of buckets that could hold all the values with no collisions (in case of one and only hash this is subject to change) */
		ht->sequenceNumber++;									
		ht->numberOfBuckets = CENTERED_DECAGONAL_PRIME(ht->sequenceNumber);
	}

	while(errorFlag == 0){ 	/* In case of low memory, retry with less buckets */
		errorFlag = 1;
		if((ht->buckets = malloc(ht->numberOfBuckets*sizeof(void *))) == NULL){
			ht->sequenceNumber--;
			ht->numberOfBuckets = CENTERED_DECAGONAL_PRIME(ht->sequenceNumber);
			fprintf(stderr, "Problem in hash table allocation.\nRetrying with value %zu.\n", ht->numberOfBuckets);
			errorFlag = 0;
		}
	}

	for(i = 0 ; i < ht->numberOfBuckets ; i++)
		ht->buckets[i] = NULL;
	return 0;
}

void HASH_Free(struct hashTable *ht)
{
	int i;
	for(i = 0 ; i < ht->numberOfBuckets ; i++){
		if(ht->buckets[i] != NULL){
			(*ht->FreeStructure)(ht->buckets[i]);	/* let the appropriate function handle the freeing of the internal structure */
			free(ht->buckets[i]);
			ht->buckets[i] = NULL;
		}
	}
	free(ht->buckets);
}

int HASH_Insert(struct hashTable *ht, void *data)
{
	int bucketId = (*ht->HashFunction)(data)%ht->numberOfBuckets;
	if(ht->buckets[bucketId] == NULL){
		if((ht->buckets[bucketId] = (*ht->AllocateStructureSpace)(512, ht->AllocateValueSpace, ht->InsertValue, ht->FreeValue, ht->Compare,ht->Update)) == NULL){	/* let the appropriate function handle the allocation of the internal structure */
			fprintf(stderr, "Could not allocate space for hash bucket.\n");
			return -1;
		}
		return (*ht->InsertStructure)(ht->buckets[bucketId], data);	/* let the appropriate function handle the insertion in the internal structure */
	}
	void *te;
	return (*ht->InsertStructure)(ht->buckets[bucketId], data);/* let the appropriate function handle the insertion in the internal structure */
}

int HASH_FindIndex( struct hashTable *ht, void *data )
{
	int bucketId = (*ht->HashFunction)(data)%ht->numberOfBuckets;
	if(ht->buckets[bucketId] == NULL )
		return 250;
	return  (*ht->SearchStructure)(ht->buckets[bucketId],data);
}

void HASH_Update(struct hashTable *ht,void *data,int t){
        
        int bucketId = (*ht->HashFunction)(data)%ht->numberOfBuckets;
   //     printf("word from bucket is %s\n",((Entry*)(((struct arrayBucket *)(ht->buckets[bucketId]))->array[t]))->word->str );
   //     printf("id from bucket is %u\n",*(QueryID*)(((Entry*)(((struct arrayBucket *)(ht->buckets[bucketId]))->array[t]))->payload->threshold[0]->entry) );
        (ht->UpdateStructure)(ht->buckets[bucketId],data,t);
        
        return;
        
    
}

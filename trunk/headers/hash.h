#ifndef HASH_H
#define HASH_H


#define INITIAL_BUCKET_NUMBER 5
#define MAXIMUM_BUCKET_NUMBER 40 
#define CENTERED_DECAGONAL_PRIME(n) 5*(n*n-n)+1

//#include <stdint.h>

struct hashTable{
	void **buckets;					/* the array that holds the buckets */
	size_t numberOfBuckets;				/* the number of buckets */
	unsigned short int sequenceNumber;		/* the position of the prime number within the sequence */
	/* the hash function */
	uint32_t (*HashFunction)();		
	/* the function that allocates the space for a new bucket 
	 * RETURNS a pointer to it
	 * 	   NULL in case of failure */
	void *(*AllocateStructureSpace)();		
	/* the function that inserts the data to the bucket
	 * 1st argument: a pointer to the structure that we want to insert the data
	 * 2nd argument: a pointer to the data
	 * RETURNS: 1 in case of success, 
	 * 	    0 if the value is already in the structure
	 * 	    -1 in case of failure */
	int (*InsertStructure)();		

	int (*SearchStructure)();
	/* the function that deletes a bucket 
	 * 1st argument: a pointer to the structure */
	void (*FreeStructure)();				
	/* the function that allocates the space for a new data cell 
	 * RETURNS a pointer to it
	 * 	   NULL in case of failure */
        void (*UpdateStructure)();
 
	void *(*AllocateValueSpace)();
	/* the function that inserts the data to the pointer of the bucket
	 * 1st argument: a pointer to the structure that we want to insert the data
	 * 2nd argument: a pointer to the data
	 * RETURNS: 1 in case of success, 
	 * 	    -1 in case of failure */
	int (*InsertValue)();
	/* the function that deletes the data from a pointer of the bucket
	 * 1st argument: a pointer to the structure */
	void (*FreeValue)();
	/* the function that compares 2 values
	 * RETURNS: 1 in case first value is bigger
	 * 	    0 in case values are equal
	 * 	    -1 in case second value is bigger */ 
	int (*Compare)();
        
        
        void (*Update)();
	
};

uint32_t FNV32(const char *);       		/* the hash function */
int HASH_f(struct hashTable *, const char *);	/* the function that returns the bucket */
int HASH_Initialize(	struct hashTable *,
			unsigned int,
			uint32_t (*HashFunction)(),
			void *(*AllocateStructureSpace)(),
			int (*CopyStructure)(),
			int (*SearchStructure)(),
			void (*FreeStructure)(),
                        void (*UpdateStructure)(),
			void *(*AllocateValueSpace)(),
			int (*InsertValue)(),
			void (*FreeValue)(),
			int (*Compare)(),
                        void (*Update)()
 			);
void HASH_Free(struct hashTable *); /* delete the hash structure */
int HASH_Insert(struct hashTable *, void *); /* Functions that handles the basic structure of a data insertions into the hash */
int HASH_FindIndex(struct hashTable *, void *);
void HASH_Update(struct hashTable *,void *,int t);

#endif

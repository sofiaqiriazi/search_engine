#ifndef DOCUMENTHASH_H
#define DOCUMENTHASH_H

struct arrayBucket{
	size_t size;
	int lastElement;
	void **array;
	/* the function that allocates the space for a new data cell 
	 * RETURNS a pointer to it
	 * 	   NULL in case of failure */
	void *(*AllocateStructureSpace)();		
	/* the function that inserts the data to the pointer of the bucket
	 * 1st argument: a pointer to the structure that we want to insert the data
	 * 2nd argument: a pointer to the data
	 * RETURNS: 1 in case of success, 
	 * 	    -1 in case of failure */
	int (*InsertStructure)(void *, void *);		
	/* the function that deletes the data from a pointer of the bucket
	 * 1st argument: a pointer to the structure */
	void (*FreeStructure)(void *);			
	/* the function that compares 2 values
	 * RETURNS: 1 in case first value is bigger
	 * 	    0 in case values are equal
	 * 	    -1 in case second value is bigger */ 
	int (*Compare)(void *, void *);

        void (*Update)(void *,void *);
};

void *ARRAY_Initialize(size_t , void *(*AllocateStructureSpace)(), int (*InsertStructure)(void *, void *), void (*FreeStructure)(void *), int (*Compare)(void *, void *),void (*Update)(void *,void *));
void ARRAY_Free(struct arrayBucket *);
int ARRAY_Insert(struct arrayBucket *, void *);
void ARRAY_Update(struct arrayBucket *, void *, int );
void *ARRAY_Search(struct arrayBucket *, void *);

int ARRAY_FindDataInBucket(struct arrayBucket *, void *);
int AllocateMoreSpace( struct arrayBucket * );

#endif

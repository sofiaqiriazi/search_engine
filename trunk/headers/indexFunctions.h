#ifndef _INDEX_FUNCTIONS_H
#define	_INDEX_FUNCTIONS_H

/* returns an array of with random unique indexes
 * 1st argument: the number of the indexes
 * 2nd argumnet: the set [0, 2nd arg) that the indexes must lie in
 * RETURNS: an array with those indexes */ 
int *RandomIndexes(int , size_t );

/* returns which entry has less MatchType distance with the guide word
 * 1st argument: the guide word
 * 2nd argument: the first word
 * 3rd argument: the second word
 * 4th argument: type of distance
 * RETURNS: -1 if the distance of first with guide is less or equal than the distance of second with guide
 *           1 otherwise */
int EntriesCompare(Entry *, Entry *, Entry *, enum MatchType);

/* partitions the space with respect of the guide element
 * 1st argument: array that is partitioned
 * 2nd argument: index of the pivot
 * 3rd argument: beginning of the space
 * 4th argument: end of the space
 * 5th argument: guide element
 * 6th argument: type of metric
 * 7th argument: a function that swaps 2 elements
 * RETURNS: the final index of the guide element
*/ 
int partition(void **, int , int, int, void *, enum MatchType, void (*swap)());

/* the QuickSelect algorithm 
 * 1st argument: array to be partitioned
 * 2nd argument: index of the element we are searching
 * 3rd argument: end of the array
 * 4th argument: guide element
 * 5th argument: type of metric
 * 6th argument: a functions that swaps 2 elements */
void QuickSelect(void **, int, int, void *, enum MatchType, void (*swap)());

/* some initializations before the Quick Select, plus finds the radius r and 
 * creates to lists, one with the elements inside the radius and one with 
 * the elements on the outside of the radius
 *
 * 1st argument: list of entries
 * 2nd argument: guide element
 * 3rd argument: type of metric
 * 4th argument: here the elements that are inside the radius will be stored
 * 5th argument: here the elements that are outside the radius will be stored
 * 6th argument: here we store the radius r
 * RETURNS: EC_SUCCESS in case of success
 *    	    EC_FAIL in case of failure */
enum ErrorCode QuickSelectInit(EntryList* , struct Entry * , enum MatchType, EntryList **, EntryList **, int *);
/*
 * return the entry that corresponce to the vantage point 
 *
 * @param[in] entries
 *   The set of entries in which the index will be built
 *
 * @param[in] type
 *   The matching type of the index as specified in MatchType enumeration
 *
 * @param[out] i
 *   Returns the posision of the VP
 */
Entry* FindVP(EntryList* entries , enum MatchType type , int* i );



/*
 * returns the distance between w1 and w2 with "type" metric
 */
int Distance(const Word *w1,const Word *w2 , enum MatchType type );

/*
 * returns a representative sample of the list
 */
EntryList* GetSample(EntryList* entries);

#endif


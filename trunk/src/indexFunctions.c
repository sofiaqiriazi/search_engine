#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include "../headers/core.h"
#include "../headers/index.h"
#include "../headers/index_impl.h"
#include "../headers/defn_impl.h"
#include "../headers/index_impl.h"
#include "../headers/indexFunctions.h"

int *RandomIndexes(int quantity, size_t size)
{
	int *numbers;
	int index, currQuantity = 0;
	int i, repetitionFlag;	

	if((numbers = malloc(sizeof(int)*quantity)) == NULL){
		fprintf(stderr, "Problem in memory allocation\n");
		return NULL;
	}

	while(currQuantity < quantity){			/* while you are searching for more indexes */
		index = rand()%size;			/* get a random index */
		repetitionFlag = 0;
		for(i = 0 ; i < currQuantity ; i++){    /* check if that index is already picked */
			if(numbers[i] == index){        
				repetitionFlag = 1;
				break;
			}
		}	
		if(!repetitionFlag)                     /* if not, add it */
			numbers[currQuantity++] = index;

	}	

	return numbers;	

}

int EntriesCompare(Entry *guide, Entry *first, Entry *second, enum MatchType type)
{
	if(Distance(guide->word, first->word, type) <= Distance(guide->word, second->word, type))
		return -1;
	return 1;
}

int partition(void **array, int pivotIndex, int left, int right, void *guide, enum MatchType type, void (*swap)())
{
	int i, storeIndex = left;
	void *pivotValue = NULL;
	swap(array[pivotIndex], array[right]);           /* put the pivot element at the last place */
	pivotValue = array[right];
	for(i = left ; i < right ; i++){                 /* for each other element in the array */
		if(EntriesCompare(guide, array[i], pivotValue, type) < 0){    /* if it comes behind in order than element storeIndex */
			swap(array[storeIndex], array[i]);                    /* swap it */
			storeIndex++;
		}
	}
	swap(array[storeIndex], array[right]);                                /* put the last element in place */
	return storeIndex;
}

void QuickSelect(void **array,int k, int right, void *guide, enum MatchType type, void (*swap)()){

	int pivotIndex = 0;
	int left = 0;
	
	if(left == right)
		return;
	while(1){                                     
		pivotIndex = left;
		pivotIndex = partition(array, pivotIndex, left, right, guide, type, swap);	
		if(k == pivotIndex){                   /* until you find the kth element partition and iterate */
			return;	
		}
		else if(k < pivotIndex){
			right = pivotIndex - 1;
		}
		else{
			left = pivotIndex + 1;
		}
	}

}

enum ErrorCode QuickSelectInit(EntryList* entries, Entry *guide, enum MatchType type, EntryList **left, EntryList **right, int *r)
{
	int numberOfEntries, i;
	struct Entry **entriesInList;	/* this is an array that holds pointers to the list entries for random access in quickselect */
	EntryList *tempList;
	struct EntryListNode *start[2], *end[2];
	Entry *temp;
	numberOfEntries = GetNumberOfEntries(entries);
	
	if(numberOfEntries == 0)
		return EC_FAIL;


	if( (tempList = malloc(sizeof(EntryList))) == NULL ){
		fprintf(stderr, "Problem in array allocation for quickselect.\n");
		return EC_FAIL;
	}



	CreateEntryList(tempList);		/* create the copy list */
	temp = GetFirst(entries);
								 
	if(Equal(temp->word, guide->word) != 0)			 /*						*/ 
		AddEntry(tempList, temp);			 /* 						*/
								 /* add all the elements from the entries list	*/
	for(i = 1 ; i < numberOfEntries ; i++){			 /* to the tempList except the guide element	*/
		temp = GetNext(entries);			 /*						*/
		if(Equal(temp->word, guide->word) != 0){	 /*						*/
			AddEntry(tempList, temp);		 /*						*/
		}						 /*						*/
	}							 /*						*/
	numberOfEntries = GetNumberOfEntries(tempList);
	
	if(numberOfEntries == 0){				/* if there is no element in tempList */
		*r = 0;

		if( (*left = malloc(sizeof(EntryList))) == NULL ){
			fprintf(stderr, "Problem in array allocation for quickselect.\n");
			return EC_FAIL;
		}
		if( (*right = malloc(sizeof(EntryList))) == NULL ){
			fprintf(stderr, "Problem in array allocation for quickselect.\n");
			return EC_FAIL;
		}
		CreateEntryList(*left);                        /* return empty left and right with radius equal to 0 */
		CreateEntryList(*right);
		free(tempList);
		return EC_SUCCESS;
	}

	if((entriesInList = malloc(sizeof(struct Entry *)*(numberOfEntries))) == NULL){ /* else create the array that we will use */
		fprintf(stderr, "Problem in array allocation for quickselect.\n");    /* in quick select */
		return EC_FAIL;
	}
	for(i = 0 ; i < numberOfEntries ; i++){ 	/* initialize it with the data */
		entriesInList[i] = GetNext(tempList);
	}

	if(numberOfEntries == 1){			/* if there is only on element in tempList */ 
		*r = Distance(guide->word, entriesInList[0]->word, type);	/* r is the distance of guide from the element */
		
		if( (*right = malloc(sizeof(EntryList))) == NULL ){
			fprintf(stderr, "Problem in array allocation for quickselect.\n");
			return EC_FAIL;
		}
		*left = GetEntryList(GetFirstNode(tempList), GetFirstNode(tempList)); /* and r is on the left */
		CreateEntryList(*right); /* while the right list is empty */
		free(entriesInList);
		free(tempList);
		return EC_SUCCESS;
	}
	*r = (numberOfEntries-1)/2;     /* here r is temporarily used as an index for the quick select element */
	QuickSelect((void **)entriesInList, *r, numberOfEntries - 1, guide, type, SwapEntries);
	start[0] = GetFirstNode(tempList);	/* after quick select is finished and the array is "sorted" we begin creating our left list */	
	if(*r == 0){
		end[0] = GetFirstNode(tempList);
	}
	else{
		for(i = 1 ; i < *r ; i++) 
			GetNext(tempList);

		end[0] = GetNextNode(tempList);
	}
	*left = GetEntryList(start[0], end[0]);
	start[1] = GetNextNode(tempList);	/* when we reach element r we begin our right list */
	end[0]->next = NULL;
	for(; i < numberOfEntries ; i++) 
		GetNext(tempList);
	end[1] = GetNextNode(tempList);
	*right = GetEntryList(start[1], end[1]);
	start[1]->prev = NULL;
	*r = Distance(guide->word, entriesInList[*r]->word, type); /* we calculate the radius r */
	free(entriesInList);
	free(tempList);
	return EC_SUCCESS;
}

Entry* FindVP(EntryList* entries , enum MatchType type ,int *trash )
{
/*
 *	we are going to find a good VP by calculating the spread of the points 
 *	(from the prospecive of the potetional VP) minus its median distance 
 *	from the points so that the VP is closer to the "edge" (corners of the
 *  collective shape of the points)
 *
 */
	int numOfEntriesP,numOfEntriesD;
	int i,j;
	EntryList *samplesD;
	Entry *node;
	EntryList *samplesP;
	EntryList *first,*sec;
	Entry *bestPoint = NULL;
	int mu;
	int spread,temp;
	int best_spread = -1;

	srand(time(NULL));
	samplesP = GetSample(entries);
	numOfEntriesP = GetNumberOfEntries(samplesP);

	if((bestPoint = malloc(sizeof(Entry))) == NULL){
		fprintf(stderr, "problem in memory allocation.\n");
		return NULL;
	}
	for(i=0;i<numOfEntriesP;i++){
		samplesD = GetSample(entries);/* get a sample for the potential VP  */
		numOfEntriesD = GetNumberOfEntries(samplesD);        
		node = (struct Entry *)(samplesP->currentEntry->entry);


		QuickSelectInit(samplesD,samplesP->currentEntry->entry,type, &first,&sec,&mu);

		spread=0;/* calculate the spread */
		temp = Distance(node->word,(GetFirst(samplesD))->word,type);
		spread += temp*temp;
		for(j=1;j<numOfEntriesD;j++){
			temp = Distance(node->word,((struct Entry *)(samplesD->currentEntry->entry))->word,type);
			spread += temp*temp;
			GetNext(samplesD);
		}
		spread /=numOfEntriesD ;
	        /* \calculate the spread */

		if(spread > best_spread){/* choose the greater spread  */
			if(best_spread != -1)	
				DestroyEntry(bestPoint);
			best_spread = spread;
			CreateEntry(((Entry *)samplesP->currentEntry->entry)->word, bestPoint);
			PAYLOAD_CopyToNode( bestPoint->payload ,
					( (Entry *)samplesP->currentEntry->entry)->payload 
					);
		}
		
		/* Clean up */
		DestroyEntryList(samplesD);
		DestroyEntryList(first);
		DestroyEntryList(sec);
		free(first);
		free(sec);
		free(samplesD);
		GetNext(samplesP);


	}   
	DestroyEntryList(samplesP);
	free(samplesP);
	return bestPoint;
	//        *trash=0;
	//	return GetFirst(entries);

	/**/
}

EntryList* GetSample(EntryList* entries)
/*
 *  retuen a 5% sample  of <entries>
 */
{
	int sizeOfEntries = GetNumberOfEntries(entries);
	int sizeOfSample = sizeOfEntries*0.05;
	int *samplePos,i,j;
	Entry *curr;
	EntryList *samples;

	if( sizeOfSample < 1)/* return at least one point */
		sizeOfSample = 1;

	samplePos = RandomIndexes(sizeOfSample, sizeOfEntries);/* get distinct positions */  
	samples = malloc( sizeof(EntryList) );
	CreateEntryList(samples);

		curr = GetFirst(entries);            
	for(i=0;i<sizeOfSample;i++){/* for every sample we want */

		//for(j=0;j<samplePos[i];j++){/* add to results at the coresponfing pos */
		//}

		AddEntry(samples , curr);
			curr = GetNext(entries);
	}

	free(samplePos);
	return samples;

}


int Distance(const Word *w1,const Word *w2 , enum MatchType type )
{
	switch ( type ) {
		case MT_HAMMING_DIST:
			return HammingDistance(w1,w2);
			break;
		case MT_EDIT_DIST:
			return EditDistance(w1,w2);
			break;
		case MT_EXACT_MATCH:
			return Equal(w1,w2);
			break;
		default:
			fprintf(stderr, "Unknown metric\n");
			return -2;
			break;
	}

	return 1;
}


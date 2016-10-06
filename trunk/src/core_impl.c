#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../headers/core.h"
#include "../headers/defn_impl.h"
#include "../headers/index_impl.h"
#include "../headers/hash.h"
#include "../headers/arrayBucket.h"
#include "../headers/wordFunctions.h"
#include "../headers/entryFunctions.h"
#include "../headers/entryList.h"
#include "../headers/entryHashFun.h"
#include "../headers/queryIDFunctions.h"
#include "../headers/payloadFunctions.h"
#include "../headers/docResultsFunction.h"
#include "../headers/queryInfo.h"
#include "../headers/cache.h"
#include "../headers/thread.h"

#include "../headers/bitMap.h"
#define NUM_DIFF_LEN_WORDS MAX_WORD_LENGTH - MIN_WORD_LENGTH
/*set up the necessary index for the exact matching type*/
enum ErrorCode SetUpExact();
/*set up the necessary index for the Hamming matching type*/
enum ErrorCode SetUpHamming();
/*set up the necessary index for the Edit matching type*/
enum ErrorCode SetUpEdit();


enum ErrorCode SetUpIndex( EntryList * );
/*given the MatchType add the entry to the proper list*/
enum ErrorCode AddToIndexes(Entry *,enum MatchType );
/*Initialize the Entrylist*/
void initHash(EntryList *);
/*search the queries and return the results */
void SearchQuerys( EntryList * , Index * , Document * , int , cache*);
void SearchQuerysHamming( EntryList list[][NUM_DIFF_LEN_WORDS], Index **index, Document *, int ,cache *);
enum ErrorCode MatchDocumentAct(DocID doc_id,const char* doc_str,cache *mnimi);

void PrintTree( Index *tree ,int len   ){
	int i;
	if (tree == NULL)
		return;
	//for (i = 0; i < len; ++i)
		//printf("-");
	//printf("%s\t\t%d\n", tree->vantagePoint->word->str , len);
	printf("%d\t", len);
	PrintTree( tree->right , len+1);
	PrintTree( tree->left , len +1 );


}

void PrintPayloads(EntryList * list){
		Entry *e;
		int i,j;
		struct QueryInfo *qi;
		struct EntryListNode *eln;
		e = GetFirst( list );
		for( i =0 ; i< GetNumberOfEntries( list  ) ; i++){
			eln = e->payload->threshold[0];
			printf("\nthreshold 0:\t");
			printf("%s:\n",e->word->str);
			while (eln != NULL){
				qi =(struct QueryInfo*)(eln->entry);
				printf("%d ,%d " , qi->numOfQuery , qi->indexInQuery);
				eln = eln->next;
			}
			printf("\ntelos\n");
			e = GetNext (list );
		}
		printf("-------------------\n");
}


typedef struct hashTable hashTable;

typedef struct indexes{ 
       	hashTable ht;                                                           /*hashtable to host entries and remove duplicates*/
	EntryList ExactList;                                                    /*keeps the entrys from queries with Hamm_dist*/
	EntryList HammingList[ NUM_DIFF_LEN_WORDS ];                                                  /*keeps the entrys from queries with Edit_dist*/
	EntryList EditList;                                                     /*keeps the entrys from queries with Exact_dist*/
	EntryList QueryInfo;
        Index *exactIndex;
        Index *hammingIndex[ NUM_DIFF_LEN_WORDS   ];
        Index *editIndex;
	struct EntryListNode *docs_results;
}indexes;

indexes ix;
pthread_mutex_t mutBm= PTHREAD_MUTEX_INITIALIZER;

enum ErrorCode InitializeIndex() {
	int i;
        CreateEntryList(&(ix.ExactList) );                                      
	for ( i = 0; i < NUM_DIFF_LEN_WORDS; ++i)
		CreateEntryList(&(ix.HammingList[i]) );                                    /*Create lists*/
	CreateEntryList(&(ix.EditList) );
	CreateEntryList(&(ix.QueryInfo) );
	InitThreads();
	return EC_SUCCESS;
}

enum ErrorCode DestroyIndex() {
	int i;
	DestroyEntryList( &(ix.ExactList)  );
	DestroyEntryList( &(ix.EditList)  );
	for ( i = 0; i < NUM_DIFF_LEN_WORDS; ++i)
		DestroyEntryList( &(ix.HammingList[i])  );
	DestroyEntryList( &(ix.QueryInfo)  );
										                if( ix.exactIndex != NULL){
												DestroyEntryIndex(ix.exactIndex);
	free(ix.exactIndex);
	}
	for(  i = 0; i < NUM_DIFF_LEN_WORDS; ++i)
		if( ix.hammingIndex[i] != NULL){
			DestroyEntryIndex(ix.hammingIndex[i]);
		free(ix.hammingIndex[i]);
	}
	if( ix.editIndex != NULL){
		DestroyEntryIndex(ix.editIndex);
		free(ix.editIndex);
	}


	return EC_SUCCESS;
               
}

enum ErrorCode StartQuery(QueryID             query_id,
                          const char*         query_str,
                          enum MatchType      match_type,
                          unsigned int        match_dist) {
        
	Word word;
	Entry entry;                                                                /*sub structs consist the entry*/
        struct QueryInfo *queryinfos;
       
        int indexInQuery=0;                                                     /*the index of a word in the query*/
	static int numOfQuery = 0;                                              /*the number of query as it arrives*/
       
        int i = 0,j = 0;
        

	while( query_str[j] != '\0' ) {
                
        	i += CreateWord2( query_str + i , &word ) +1;
		CreateEntry(&word , &entry );                                          /*create a new entry*/
	
                queryinfos = malloc(sizeof(struct QueryInfo));                          /*allocate memory for the struct*/
        
		queryinfos->id = malloc(sizeof(QueryID));
                                                                
		memcpy(queryinfos->id,&query_id,sizeof(QueryID));               /*copy the query id inside the struct*/
		queryinfos->indexInQuery = indexInQuery;                        /*copy the index of the word in the query*/
		queryinfos->numOfQuery = numOfQuery;                            /*copy the current index number*/
	
		indexInQuery++;                                                 /*increase the number by one for the next struct*/
		
		/*insert in the entry's payload lists the struct containing the infos*/
		entry.payload->threshold[(match_dist == 0 ? 0 : match_dist-1)] = ENTRYLIST_Insert(entry.payload->threshold[ (match_dist == 0 ? 0 : match_dist-1) ] ,queryinfos,QUERYINFO_Allocate,QUERYINFO_CreateNode,QUERYINFO_Free);
		
		//printf("the word is %s and the",entry.word->str);
		//printf("ID :%u \n",*(QueryID*)(((struct QueryInfo*)(entry.payload->threshold[match_dist]->entry))->id) );
		AddToIndexes(&entry , match_type );                             /*add the entry to the appropriate list*/
		
		DestroyEntry(&entry);                                          /*destroy the entry*/
		
		QUERYID_Free(queryinfos->id);
		free(queryinfos);
		WORD_Free( &word );                                                /*free the word*/
		j = i-1;
	}

	numOfQuery++;                                                           /*increase the number of the query*/
        
	CreateWord(query_str , &word );
	CreateEntry(&word , &entry );
        
	AddEntry( &(ix.QueryInfo) ,  &entry );
	DestroyEntry(&entry);
	WORD_Free( &word );
                       
	return EC_SUCCESS;
}

enum ErrorCode AddToIndexes(Entry *entry,enum MatchType match_type) {
	switch ( match_type ) {
                case MT_HAMMING_DIST:
//                        printf("Insert word %s in Hamming List %d\n",e->word->str,e->payload->threshold[]);
			AddEntry( &(ix.HammingList[ entry->word->size -  MIN_WORD_LENGTH ] ),entry);
                        break;
                case MT_EDIT_DIST: 
//                        printf("Insert word %s in Edit List\n",e->word->str);
			AddEntry( &(ix.EditList),entry);
                        break;
                case MT_EXACT_MATCH:
//                        printf("Insert word %s in Exact List\n",e->word->str);
			AddEntry( &(ix.ExactList),entry);
                        break;
                default:
                        fprintf(stderr, "Unknown metric\n");
                        return -2;
                        break;
        }
	
	return EC_SUCCESS;
}


enum ErrorCode MatchDocument(DocID         doc_id,
                             const char*   doc_str) {
	int i;
	static flag = 1;
	if(flag == 1 ) {
		//set up indexes
		
		//SetUpExact();
		//SetUpHamming();
		//SetUpEdit();
		SetUpIndex( &(ix.EditList)  );
		for ( i = 0; i < NUM_DIFF_LEN_WORDS; ++i)
			SetUpIndex( &(ix.HammingList[i]) );
		SetUpIndex( &(ix.ExactList) );
		flag = 0;
                
                ix.exactIndex = malloc(sizeof(Index));
                if(GetNumberOfEntries(&(ix.ExactList))!=0)
                BuildEntryIndex( &(ix.ExactList) , MT_EXACT_MATCH , ix.exactIndex );

                //DestroyEntryIndex(ix.exactIndex );
                //DestroyEntryList(&(ix.ExactList));
                //free(ix.exactIndex
		for (i = 0; i < NUM_DIFF_LEN_WORDS; ++i)
			if(GetNumberOfEntries( &(ix.HammingList[i]))!=0){
				ix.hammingIndex[i] = malloc(sizeof(Index));
				BuildEntryIndex( &(ix.HammingList[i]),MT_HAMMING_DIST , ix.hammingIndex[i] );
			}
		if(GetNumberOfEntries( &(ix.EditList))!=0){
			ix.editIndex = malloc(sizeof(Index));
			BuildEntryIndex( &(ix.EditList) , MT_EDIT_DIST , ix.editIndex );
		}
		else
			ix.editIndex = NULL;
                
                
		//PrintTree( (ix.hammingIndex), 0 );
		printf("go\n");
	}
	args_job *a = malloc(sizeof(args_job) );
	a->id = doc_id;
	a->doc_str = malloc(sizeof(char) * (strlen(doc_str)+1) );
	strcpy(a->doc_str , doc_str);
	NewJob((void*)a);
	
}


enum ErrorCode MatchDocumentAct(DocID         doc_id,
                             const char*   doc_str,
			     cache *mnimi) {
	bitMap bm;
	int i;
	pthread_t id =  pthread_self();
	printf("(arxh tou %d thread: %0x\n",doc_id ,id );

	pthread_mutex_lock( &mutBm );
	InitBitMap(&(ix.QueryInfo),&bm);
	pthread_mutex_unlock( &mutBm );
	Document *doc;
	doc = malloc(sizeof(Document));
	CreateDocument(doc_id,doc_str,doc);
	RemoveDuplicates(doc);

	//printf("search\n");
	EntryList resultsExact , resultsHamming[NUM_DIFF_LEN_WORDS] , resultsEdit;
	SearchQuerys(&resultsEdit , ix.editIndex , doc , 3, mnimi );
	SearchQuerysHamming(&resultsHamming , ix.hammingIndex , doc , 3,mnimi );
	SearchQuerys(&resultsExact , ix.exactIndex , doc , 0 ,mnimi);
	
	//printf("match\n");
	SetBitsFromList( &bm , &resultsEdit );
	for ( i = 0; i < NUM_DIFF_LEN_WORDS; ++i)
		SetBitsFromList( &bm , &resultsHamming[i] );
	SetBitsFromList( &bm , &resultsExact  );
	
	pthread_mutex_lock( &res );
	if( fR == NULL  ){
		fR = malloc(sizeof( results ) );
		lR = fR;
	}
	else {
		lR->next = malloc(sizeof( results ) );
		lR = lR->next;
	}
	CheckBitMap( &bm , lR );
	lR->id = doc_id;
	lR->next = NULL;
	//printf("%d:", lR->id);
	//for(i=0;i<lR->resCount ; i++)
		//printf("%d,",lR->res[i] );
	pthread_mutex_unlock( &res );

	DestroyEntryList( &resultsExact );
	for ( i = 0; i < NUM_DIFF_LEN_WORDS; ++i)
		DestroyEntryList( &(resultsHamming[i]) );
	DestroyEntryList( &resultsEdit );
	

 	DestroyBitMap(&bm); 
	free(doc->str);
	free(doc);
	//printf("telos\n");
        
	return EC_SUCCESS;
}

void SearchQuerys( EntryList * list , Index * index , Document *doc, int threshold ,cache *mnimi){
	int i , length;
	Word *tempword;
	//printf("or_len\tlen\tmatch \n");
	//printf("%d\t",length);
	CreateEntryList( list  );
	length = GetNumDocumentWords(doc);       
	//printf("%s\n", doc->str);
	//printf("sinolo %d\n",length );
	//for( i = 0 ; i < length ; i++ ){
		//tempword = getDocumentWord(i,doc);
		//printf("%s ",tempword->str);
	i = 0;
	int size = 0;
	char * str = doc->str;
	tempword = malloc( sizeof(Word) );
	while( doc->str[( i == 0 ? 0 : i-1 )]!='\0'  ){
		//i+=CreateWord2( (doc->str[i]), tempword) +1;
		//printf("====================%d\n",i );
		size = 0;
		while(str[size+i] != ' ' && str[size+i] != '\0' )
			size++;     
		tempword->size = size;
		tempword->str = malloc( tempword->size*sizeof(char)+1); 
		int j;
 		for(  j= 0 ; j < size ; j++  )
			tempword->str[j] = str[j+i];
		tempword->str[size] = '\0';
		i += size+1;
		//printf("%s %d\n", tempword->str, i);

		if( SearchCache( mnimi , tempword ,threshold , list ) == 0  ){
			LookupEntryIndex( tempword , threshold , index , list );
			UpdateCache( mnimi , tempword , threshold ,GetLast(list) );
		}
		free( tempword->str );
		//free((char *) tempword );

	}
	return;
}
	
void SearchQuerysHamming( EntryList list[][NUM_DIFF_LEN_WORDS] , Index **index , Document *doc, int threshold , cache *mnimi){
	int i , length;
	Word *tempword;
	//printf("or_len\tlen\tmatch \n");
	//printf("%d\t",length);
	for ( i = 0; i < NUM_DIFF_LEN_WORDS; ++i)
		CreateEntryList(&((*list)[i])  );
	i = 0;
	int size = 0;
	char * str = doc->str;
	tempword = malloc( sizeof(Word) );
	while( doc->str[( i == 0 ? 0 : i-1 )]!='\0'  ){
		size = 0;
		while(str[size+i] != ' ' && str[size+i] != '\0' )
			size++;     
		tempword->size = size;
		tempword->str = malloc( tempword->size*sizeof(char)+1); 
		int j;
 		for(  j= 0 ; j < size ; j++  )
			tempword->str[j] = str[j+i];
		tempword->str[size] = '\0';
		i += size+1;

		if( SearchCache( mnimi , tempword ,threshold ,  &((*list)[thesi]) ) == 0  ){
			int thesi = tempword->size - MIN_WORD_LENGTH;
			LookupEntryIndex( tempword , threshold , index[thesi] , &((*list)[thesi]) );
			UpdateCache( mnimi , tempword , threshold ,GetLast(  &((*list)[thesi])  ) );
		}
		free( tempword->str );
	}
	return;
}
	

enum ErrorCode GetNextAvailRes(DocID*         p_doc_id,
                               unsigned int*  p_num_res,
                               QueryID**      p_query_ids) {
	pthread_mutex_lock( &res);
	results *temp;
	temp = fR;
	int i;
	while(fR == NULL)
		pthread_cond_wait( &idleThread , &res );
	printf("done %d\n",fR->id );
	*p_doc_id = fR->id;
	*p_num_res = fR->resCount;
	*p_query_ids = fR->res;
	fR = fR->next;
	free(temp);
	pthread_mutex_unlock( &res );
}



enum ErrorCode SetUpIndex( EntryList * list  ) {
        int i,j;
	int index;                                                              /*index of the entry inside an array in hashtable*/
        
        initHash( list ) ;                                            /*initialize hash table*/
	
	Entry *newEntry = GetFirst(  list );                                   /*take the first entry from the list*/
        
	for( i = 0 ; i < GetNumberOfEntries( list ) ; i++) {           /*for every entry inside the list*/
		index = HASH_FindIndex(&(ix.ht), newEntry  );                          /*find entry's index*/
                
		if( index >= 0 )                                                /*if entry not fund*/
			HASH_Insert( &(ix.ht), newEntry  );                            /*insert the entry in the hash table*/                    
		else {
			HASH_Update(&(ix.ht),newEntry,-1-index);                /*else update the entry fund with the new entry*/
                }
		newEntry = GetNext( list );                            /**Get the next entry from the list*/
	}
	
        DestroyEntryList( list );                                      /*free the list*/
        
        
        int hashsize = (&(ix.ht))->numberOfBuckets;                             /*get the sixe of the hash table*/

        CreateEntryList( list  );                                       /*recreate the entryList*/

        
        for(i=0;i<hashsize;i++){
            if((&(ix.ht))->buckets[i] != NULL){
                for(j=0;j<=(((struct arrayBucket *)((&(ix.ht))->buckets[i])))->lastElement;j++){
                    /*get the entry from the array and add the entry to the list*/
                    AddEntry( list ,(((struct arrayBucket *)((&(ix.ht))->buckets[i])))->array[j] );
                }
            }
        }
        HASH_Free(&(ix.ht));                                                    /*free the hash table*/
      	return EC_SUCCESS;
        
}


void initHash(EntryList * list) {
	int size = GetNumberOfEntries( list );
	HASH_Initialize( &(ix.ht), size ,EntryHash ,
			 ARRAY_Initialize,
			 ARRAY_Insert ,
			 ARRAY_FindDataInBucket ,
			 ARRAY_Free ,
                         ARRAY_Update,
			 ENTRY_Allocate, ENTRY_Cpy,
			 ENTRY_Free, ENTRY_Cmp, ENTRY_Update
			);
}

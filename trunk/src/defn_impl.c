#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../headers/defn.h"
#include "../headers/defn_impl.h"
#include "../headers/hash.h"
#include "../headers/arrayBucket.h"
#include "../headers/wordFunctions.h"

/* COMP returns 0 if x =  = y and 1 otherwise */
#define COMP( x,y ) ( -( !( ( x ) ^ ( y ) )-1 ) )
/* min returns the min... */
#define min( x,y ) ( ( y )^( ( ( x )^( y ) )&-( ( x ) < ( y ) ) ) )
#define min3( x,y,z ) ( min( min( x,y ),z ) )


void CreateWord( const char* word_str,
                Word* word ) {
  word->size = strlen( word_str );
  word->str = malloc( word->size*sizeof(char)+1);
  strcpy( word->str,word_str );
  word->str[word->size]='\0';
  return;
}


int CreateWord2( const char* word_str,
                Word* word ) {
	//printf("%s\n", word_str);
  int size = 0 , i;
  while(word_str[size] != ' ' && word_str[size] != '\0' )
    size++;
  word->size = size;
  word->str = malloc( word->size*sizeof(char)+1);
  for ( i = 0 ; i < size ; i++  )
    word->str[i] = word_str[i];
  word->str[size] = '\0';
  return size;
}



void CreateQuery( QueryID query_id,
                 const char* query_str,
                 enum MatchType match_type,
                 unsigned int match_dist,
                 Query* query ) {

    query->id = query_id;
    query->str = malloc(strlen(query_str)*sizeof(char)+1);
    strcpy(query->str,query_str);
    query->match_type = match_type;
    query->match_dist = match_dist;

    return;

}

int GetNumQueryWords( const struct Query* query ) {

	/*the number of words in query is now one*/
	int numQueryWords=1;
    /*get the length of the query*/
    int i,length=strlen(query->str);

    /*while in query*/
    for(i=0;i<length;i++){

    	/*if character in query is the space character*/
    	if( query->str[i] == ' ' ){
        	/*add one to the number of words*/
            numQueryWords++;
        }

    }

    return numQueryWords;

}

const struct Word* getQueryWord( unsigned int word, const  Query* query ) {

	Word *queryWord;

	int i, length, end, counter = 0, start=0;
	/*set ending index to the end of the query's str*/
	end = length= strlen(query->str);
	queryWord = malloc(sizeof(Word));

	/* while index inside the query's str and the counter isn't greater than the word*/
	for(i=0 ;i<length && counter<=word ;i++){

		/*if the indexing character is the space*/
		if(query->str[i] == ' '){

			/*if that was the word that we were looking for*/
			if(counter == word){
				/*set the ending index to that space character*/
				end = i;
			}
			/*if we are still searching */
			else{
				/*set the starting index one character after the space found*/
				start = i+1;
			}

			/*increase the counter by one*/
			counter++;
		}

	}

	/*if the word wanted is the last word change the ending index to the last character*/
	if(end < start){
		end = length;
	}

	/*the difference between the indexes is now the returning struct Word size*/
	queryWord->size = end-start;
	/*allocate the memory for the word wanted in position 'word'*/
	queryWord->str = malloc(queryWord->size*sizeof(char));
	/*copy the word from the query to the returning struct Word*/
	strncpy(queryWord->str,&(query->str)[start],queryWord->size);

	return queryWord;

	/*We can use this instead. Use of strcpy and strtok_r*/
/*
    char *line;
    char *restline;
    int i;
    //allocate memory with size of the query
    line=malloc(strlen(query->str)*sizeof(char));

	//copy query str to start
    strcpy(line,query->str);

	//for every word in line in smaller position than the 'word' we are looking for
    for(i=0;i<word;i++){
    	//take the word and hold it in queryWord
        queryWord->str = strtok_r(line," ",&restline);
        line = restline;
    }
    //set the size for the word
    queryWord->size = strlen(queryWord->str);

  	return queryWord;
*/

}

void CreateDocument( DocID doc_id,
                    const char* doc_str,
                    Document* document ) {

    document->id = doc_id;
    document->str = malloc(strlen(doc_str)*sizeof(char)+1);
    strcpy(document->str,doc_str);
    //document->str = doc_str;

    return;

}

int GetNumDocumentWords( const struct Document* doc ) {

	/*the number of words in document is now one*/
	int numDocWords=1;
    /*get the length of the document*/
    int i,length=strlen(doc->str);

    /*while in document*/
    for(i=0;i<length;i++){

    	/*if character in document is the space character*/
    	if( doc->str[i] == ' ' ){
        	/*add one to the number of words*/
            numDocWords++;
        }

    }

    return numDocWords;

}

const struct Word* getDocumentWord( unsigned int word, const Document* doc ) {

	Word *queryWord;

	int i, length, end, counter = 0, start=0;
	/*set ending index to the end of the document's str*/
	end = length= strlen(doc->str);
        
	queryWord = malloc(sizeof(Word));

	/* while index inside the document's str and the counter isn't greater than the word*/
	for(i=0 ;i<length && counter<=word ;i++){

		/*if the indexing character is the space*/
		if(doc->str[i] == ' '){

			/*if that was the word that we were looking for*/
			if(counter == word){
				/*set the ending index to that space character*/
				end = i;
			}
			else{
				/*if we are still searching set the starting index one character after the space found*/
				start = i+1;
			}

			/*increase the counter by one*/
			counter++;
		}

	}

	/*if the word wanted is the last word change the ending index to the last character*/
	if(end < start){
		end = length;
	}

	/*the difference between the indexes is now the returning struct Word size*/
	queryWord->size = end-start;
	/*allocate the memory for the word wanted in position 'word'*/
	queryWord->str = malloc(((queryWord->size))*sizeof(char)+1);
	/*copy the word from the document to the returning struct Word*/
	strncpy(queryWord->str,&(doc->str)[start],queryWord->size);
        queryWord->str[queryWord->size]='\0';
      
	return queryWord;
}


int Equal( const Word* w1, const Word* w2 ) {
  int i;
  if( w1->size != w2->size )return -1;
  for( i = 0;i < w1->size;i++ )
    if( w1->str[i] != w2->str[i] )return 1;
  return 0;
}

int HammingDistance( const Word* w1, const Word* w2 ) {
  int i,acc = 0;
  if( w1->size != w2->size )return -1;
  for( i = 0;i < w1->size;i++ )
    acc+= COMP( w1->str[i],w2->str[i] );/* add 1 at every difference */
  return acc;
}

int EditDistance( const Word* w1, const Word* w2 ) {
  int i,j,n = w1->size+1,m = w2->size+1,k,temp;
  int *N[2];

/*
    INITIALIZE
*/
  N[0] = malloc( n*sizeof( int ) );
  N[1] = malloc( n*sizeof( int ) );
  for( i = 0 ; i < n ; i++ )N[0][i] = i;

/*
    MAIN ALGORITHM
*/
  k = 1;/*  k shows which line we are bulding and ( k+1 )%2 ( its compliment ) the previous line ( completed line ) */
  for( i = 0 ; i < m-1 ; i++ )/** for all the letters of w2 **/
  {
    N[k][0] = i+1;/* the distance between an empty string( the 0 place of w2 ) and the first i+1 char of w1 is i+1 */
    for( j = 0 ;j < n-1;j++ )/** for all the letters of w1 **/
    {
      /** choose the min out of /
                    remove           incert         substitute/no op(depence on the comparison)    **/
      N[k][j+1]=min3( N[k][j]+1 , N[(k+1)%2][j+1]+1 , N[(k+1)%2][j]+COMP(w1->str[j],w2->str[i]) );
    }
    k=(k+1)%2;/* swap the matrices */
  }
  
  temp = N[( k+1 )%2][n-1];
  free( N[0] );
  free( N[1] );
  return temp;


}

void RemoveDuplicates( struct Document* doc ) {

    
    int i, length, end, start=0;
    int numOfWords = GetNumDocumentWords(doc);                                  /*total number of words*/
    struct hashTable ht;
    struct Word *currWord = malloc(sizeof(struct Word));
    /*set ending index to the end of the document's str*/
    end = length= strlen(doc->str);
   
    /*Initialize the hush function*/ 
    HASH_Initialize(&ht, numOfWords, wordHash, ARRAY_Initialize, ARRAY_Insert, NULL ,ARRAY_Free, NULL ,WORD_Allocate, WORD_Cpy, WORD_Free, WORD_Cmp,NULL);	
    
    /* while index inside the document's str*/
	for(i=0 ;i<length;i++){

		/*if the indexing character is the space*/
		if(doc->str[i] == ' '){
                    /*the i is now the ending index of the word*/
                    end = i;
                    /*the size of the word is the difference between the ending and the starting index*/
                    currWord->size = end-start;
                    /*allocate the memory for the word wanted [start,end]*/
                    currWord->str = malloc((currWord->size+1)*sizeof(char));
	            /*copy the word from the document to the currWord str*/
	            strncpy(currWord->str,&(doc->str)[start],currWord->size);
                    /* the word to insert in ht is currWord*/
		    currWord->str[currWord->size] = '\0';
		    if(HASH_Insert(&ht, currWord) == 0){                        /*if the word already inside the hash table*/
                        memmove(doc->str+start-1,doc->str+end,length-start-currWord->size+1);/*delete the currWord str from the document*/
                        length = length-(end-start+1);                          /*decrease the length by the word size plus one space*/
                        i = start;                                              /*set i to the index of the word to look next*/
                        end=0;                                                  /*set the end to nothing yet*/
                    }
                    else{                                                       /*if the word is not inside the hash table*/
                        end = 0;
                        start = i+1;                                            /*move the starting index after the last space*/
                    }
        	    free(currWord->str);
		}

	}
        
        if(end < start){                                                        /*if the ending is less than starting index*/
            end = length;
        }
    
        /*set the size of the word to the difference between the ending and the starting index*/
        currWord->size = end-start;
        /*allocate the memory for the word wanted [start,end]*/
        currWord->str = malloc((currWord->size+1)*sizeof(char));
	/*copy the word from the document to the currWord str*/
	strncpy(currWord->str,&(doc->str)[start],currWord->size);
        /*Insert the currWord */
        currWord->str[currWord->size] = '\0';
        if(HASH_Insert(&ht, currWord) == 0){                                    /*if the currWord already inside the hash table*/
            memmove(doc->str+start-1,doc->str+end,1);        /*remove the ending word from the document*/
        }
	HASH_Free(&ht);
	free(currWord->str);
	free(currWord);
    return;
}

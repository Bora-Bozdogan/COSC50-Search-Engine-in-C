/* 
 * querier.c - CS50 'querier' program
 *
 * Bora Bozdogan, April 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h> 
#include "mem.h"
#include "webpage.h"
#include "hashtable.h"
#include "pagedir.h"
#include "file.h"
#include "index.h"
#include "word.h"

//struct used to get highest score while iterating counters
typedef struct node {
    int key;
    int score;
} node_t;

//struct used for updateScores, holds two counters so they can be cross-checked
typedef struct countersHolder {
    counters_t* partialScores;
    counters_t* newScores;
} countersHolder_t;

//function headers
static bool querier_validateParams(char* pageDirectory, char* indexFilename);
static char** querier_parse(char* line);
static counters_t* querier_process(char** words, index_t* index);
static void addScores(void* arg, const int key, const int count);
static void updateScores(void* arg, const int key, const int count);
static void initScores(void* arg, const int key, const int count);
static void getHighestScore(void* arg, const int key, const int count);
static bool querier_isLiteral(char* word);
int fileno(FILE *stream);
static void prompt(void);

/* ***************************
 *  main - start of querier.c
 *
 *  Parameters:
 *    argc - number of arguments
 *    argv - list of arguments
 *
 *  Returns:
 *    nonzero if error, void else    
 *
 *  Behavior:
 *    validates parameters, loads index, reads from stdin
 *    one line at a time, responds to queries with the most
 *    appropriate documents.
 *   
 */
int
main (int argc, char* argv[])
{   
    if (argc != 3) {
        fprintf(stderr, "Invalid argument number, input 2\n");
        exit(1);   
    }
    //argv[1] = pageDirectory - pathname of directory made by crawler
    //argv[2] = indexFilename - pathname of file produced by indexer
    char* pageDirectory = argv[1];
    char* indexFilename = argv[2];

    //validate the arguments
    if (!querier_validateParams(pageDirectory, indexFilename)) {
        exit(1);
    }

    //load index from indexFilename to internal data structure
    //no need to check return value, querier_validateParams checks indexFilename
    index_t* index = index_load(indexFilename);

    //read from stdin, one per line, until eof
    char* line;
    char** words;

    //initially prompt the user
    prompt();

    while ((line = file_readLine(stdin)) != NULL) {
        //clean and parse each query according to syntax
        //continue with algorithm if valid query
        if ((words = querier_parse(line)) != NULL) {
            //counter contains all docIDs with scores
            counters_t* scores = querier_process(words, index);

            node_t* highestScore = mem_malloc(sizeof(node_t));

            if (highestScore == NULL) {
                fprintf(stderr, "Couldn't allocate memory\n");
                exit(1);
            }
            
            //arbitary value to set to zero inside the while loop
            highestScore->score = -1; 
            highestScore->key = 0;

            //boolean that will be false after the first iteration of while loop
            bool firstIteration = true;

            //if value 0, means that every counter value has set to 0
            while (highestScore->score != 0) {
                
                //set highestScore to 0
                highestScore->score = 0;
                
                //find highest score, save key and score to highestScore
                counters_iterate(scores, highestScore, getHighestScore);
            
                //set value of counter with highest score to 0
                counters_set(scores, highestScore->key, 0);

                if (highestScore->score != 0) {
                    //use webpage to get URL easily
                    webpage_t* webpage = pagedir_load(pageDirectory, highestScore->key);
                    webpage_fetch(webpage);
                    
                    //print highest score, doc ID, URL
                    printf("%d %d %s\n", highestScore->score, highestScore->key, webpage_getURL(webpage));
                    webpage_delete(webpage);
                } 

                //if on first iteration and highest score in counters is zero
                if (firstIteration && highestScore->score == 0) {
                    //no documents were found, print message
                    printf("No documents match\n");
                }

                firstIteration = false;

            }
            //free variables
            free(highestScore);
            counters_delete(scores);
            freeWords(words);
        }
        free(line);

        //prompt the user again for next query
        prompt();
    }
    index_delete(index);

    //go next line after empty prompt message
    printf("\n");

    //exit zero when EOF reached
    exit(0);
}

/**************** getHighestScore() ****************                                                                                
 *  getHighestScore - main helper function,
 *      make node equal to highest score in scores
 *
 *  Parameters:
 *     arg - void* parameter used for reference to Node struct
 *     key - const char* key of counter's current loop 
 *     count - const int current value
 *
 *  Returns:
 *     void 
 *
 *  Behavior:
 *     sets struct to highest value among the counters
 *
 */
static void getHighestScore(void* arg, const int key, const int count) {
    node_t* highestScore = arg;
    
    if (count > highestScore->score) {
        highestScore->score = count;
        highestScore->key = key;
    }
}

/**************** querier_validateParams() ****************                                                                                
 *  querier_validateParams - validate given parameters
 *
 *  Parameters:
 *    pageDirectory - char* supposed to contain page directory
 *    indexFilename - char* supposed to contain file name of index file
 *
 *  Returns:
 *     bool - true if valid params, false else
 *
 *  Behavior:
 *     checks if pageDirectory is the pathname directory produced
 *     by the crawler and indexFilename is the pathname of a file
 *     that can be read. Returns false on failure, completes
 *     succesfully and returns true if no errors.
 */
static bool querier_validateParams(char* pageDirectory, char* indexFilename) {
    //check pagedirectory
    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "Invalid page directory\n");
        return false;
    }
    //check indexFilename if readable
    FILE* fp = fopen(indexFilename, "r");
    if (fp == NULL) {
        //file can't be read
        fprintf(stderr, "Couldn't open file for reading\n");
        return false; //exit nonzero if couldn't read file, per requirements spec
    } else {
        //file name valid, close pointer
        fclose(fp);
    }
    return true;
}

/**************** querier_parse() ****************                                                                                
 *  querier_parse - parse the query
 *
 *  Parameters:
 *     line - char* input read from stdin
 *
 *  Returns:
 *     char* - the parsed query
 *
 *  Behavior:
 *     clean the query, ignore blank lines, convert to lowercase
 *     parses the input line from stdin according to the backus-naur syntax
 *     if query syntax invalid, print error message, don't parse query
 *     if query empty, do nothing
 */
static char** querier_parse(char* line) {
    
    //check for blank lines, ignore if blank
    if (countWords(line) == 0) {
        return NULL;
    }

    //not blank, split sentence into words
    char** words = splitWords(line);
    //normalize each word, convert to lowercase
    for(char** currentWord = words; *currentWord != NULL; currentWord++) {
        normalizeWord(*currentWord);
    }

    //print the clean query for the user to see
    printf("Query:");
    for(char** currentWord = words; *currentWord != NULL; currentWord++) {
        printf(" %s", *currentWord);
    }
    printf("\n");
    fflush(stdout); //flush it out so testing.out looks proper order without buffers

    /* loop over words, make sure it fits backus-naur by checking if
    if it starts with a literal, or if two literals are together 
    or if it has invalid characters*/
    char* word = words[0]; // first word
    if (querier_isLiteral(word)) {
        //starts with 'and' or 'or', print error
        fprintf(stderr, "Error: '%s' cannot be first\n", word);
        freeWords(words);
        return NULL;
    }
    //loop over the words, to check if it has two adjacent literals, end with 
    //literal, or has invalid characters
    for(char** currentWord = words; *(currentWord + 1) != NULL; currentWord++) {
        char* nextWord = *(currentWord + 1);
        if (querier_isLiteral(nextWord) && querier_isLiteral(*currentWord)) {
            //two adjacent literals, print error
            fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", *currentWord, nextWord);
            freeWords(words);
            return NULL;
        }
        
        //check if last word is a literal
        if (*(currentWord + 2) == NULL) {
            //nextWord is the last word
            if (querier_isLiteral(nextWord)) {
                fprintf(stderr, "Error: '%s' cannot be last\n", nextWord);
                freeWords(words);
                return NULL;
            }
        }
    }

    //loop over words and check each word if it contains invalid characters
    for(char** currentWord = words; *(currentWord) != NULL; currentWord++) {
        //check if word has invalid characters
        for (char* c = *currentWord; *c != '\0'; c++) {
            if (isalpha((unsigned char)*c) == 0) {
                //c is invalid
                fprintf(stderr, "Error: invalid character '%c' in query\n", *c);
                freeWords(words);
                return NULL;
            }
        }   
    }

    //return array of words
    return words;
}

/**************** querier_process() ****************                                                                                
 *  querier_process - process the query
 *
 *  Parameters:
 *     words - char** array of words
 *     index - index_t* index of word counts
 *
 *  Returns:
 *     counter_t* counter containing all docIDs that satisfy any part of query
 *     with score as item    
 *
 *  Behavior:
 *     scores the document based on how well it fits the query. Keeps
 *     "partial score" for each andsequence seperated by 'or', where 
 *     it always sets partial score to lowest value seen in that
 *     andsequence. When it encounters an 'or', adds the partial score
 *     to total score. Holds counters that do this algortihm for any
 *     docID that contains any of the words.
 *
 */
static counters_t* querier_process(char** words, index_t* index) {

    //initialize counters holding docID as key, partial and total score as item
    counters_t* partialScores = counters_new();
    counters_t* totalScores = counters_new();

    //variable only true on the first loop of a new andsequence
    bool isFirstWord = true;


    for(char** currentWord = words; *currentWord != NULL; currentWord++) {

        if (strcmp(*currentWord, "and") == 0) {
            //word equals 'and', don't do anything
            continue;
        } else if (strcmp(*currentWord, "or") == 0) {
            //loop over all docIDs in partialScores, call addScores
            counters_iterate(partialScores, totalScores, addScores);
            //reset partialScores (the andsequence)
            counters_delete(partialScores);
            partialScores = counters_new();
            //set true for next andsequence
            isFirstWord = true;
        } else {
            //check if word in index, skip if not
            if (index_check(index, *currentWord)) {
                //get counter of word
                counters_t* newScores = index_find(index, *currentWord);
                if (isFirstWord) {
                    /*
                    first word of andsequence, can accept new docIDs
                    initialize score of each docID
                    */
                    counters_iterate(newScores, partialScores, initScores);
                    isFirstWord = false;
                } else {
                    /*
                    if newScores doesn't have a key that exists in partialScores
                    that key in partialScores must be set to 0
                    
                    following struct is required because counters_iterate must
                    access both the newScores and partialScores
                    */
                    countersHolder_t* holder = mem_malloc(sizeof(countersHolder_t));

                    if (holder == NULL) {
                        fprintf(stderr, "Couldn't allocate memory\n");
                        exit(1);
                    }

                    holder->partialScores = partialScores;
                    holder->newScores = newScores;

                    //iterate over partialScores, update scores based on newScores
                    counters_iterate(partialScores, holder, updateScores);
                    free(holder);
                }
            } else {
                //word not in index, reset partialScores, andsequence 0 for all
                counters_delete(partialScores);
                partialScores = counters_new();

                isFirstWord = false;
            }
        }
    }
    //add partialScores out of loop in case it never encounters an 'or'
    counters_iterate(partialScores, totalScores, addScores);
    //free unused counters
    counters_delete(partialScores);

    return totalScores;
}



/**************** querier_isLiteral() ****************                                                                                
 *  querier_isLiteral - check if the word is a literal (and - or)
 *
 *  Parameters:
 *     word - char* word to check
 *
 *  Returns:
 *     bool - true if word equals to 'and' or 'or'
 */
static bool querier_isLiteral(char* word) {
    if (strcmp(word, "and") == 0 || strcmp(word, "or" ) == 0) {
        //it's a literal, return true
        return true;
    }
    return false;
}

/**************** addScores() ****************                                                                                
 *  addScores - querier-process helper function,
 *      add the partial scores to total scores
 *
 *  Parameters:
 *     arg - void* parameter used for reference to totalScores
 *     key - const char* key of counter's current loop 
 *     count - const int current value
 *
 *  Returns:
 *     void 
 *
 *  Behavior:
 *     adds the partial scores to totalScores
 *
 */
static void addScores(void* arg, const int key, const int count) {
    counters_t* totalScores = arg;
    //increment the value of docID by partialScore
    /*
    this accounts for new docIDs as well since counters_get returns 0 for 
    previously unencountered keys, new counter will be created at the right
    value
    */
    //check if score infinite, if it stays infinite, andsequence wasn't satisfied
    if (count != INT_MAX) {
        counters_set(totalScores, key, counters_get(totalScores, key) + count);
    }
}

/**************** initScores() ****************                                                                                
 *  initScores - querier-process helper function,
 *      gets called on first word of andsequence
 *      initialize previously unseen docIDs
 *
 *  Parameters:
 *     arg - void* parameter used for reference to partialScores
 *     key - const char* key of counter's current loop 
 *     count - const int current value
 *
 *  Returns:
 *     void 
 *
 *  Behavior:
 *     sets all partial scores to first word's value
 *
 */
static void initScores(void* arg, const int key, const int count) {
    //it's first word of andsequence, counter should be set to count regardless
    counters_t* partialScores = arg;
    counters_set(partialScores, key, count);
}


/**************** updateScores() ****************                                                                                
 *  updateScores - querier-process helper function,
 *     updates partial scores
 *
 *  Parameters:
 *     arg - void* parameter used for reference to partialScores
 *     key - const char* key of counter's current loop 
 *     count - const int current value
 *
 *  Returns:
 *     void 
 *
 *  Behavior:
 *     checks if the word seen occurs in previous partialScores
 *     sets the partialScore to 0 if not, if seen, sets it to min
 */
static void updateScores(void* arg, const int key, const int count) {
    countersHolder_t* holder = arg;
    counters_t* partialScores = holder->partialScores;
    counters_t* newScores = holder->newScores;

    int newScore = counters_get(newScores, key);
    if (newScore > 0) {
        //docID in partialScores has the new word as well
        if (newScore < count) {
            //new value lower, set it to score for andsequence
            counters_set(partialScores, key, newScore);
        }
    } else {
        //docID in partialScores does not have new word, set to 0
        counters_set(partialScores, key, 0);
    }

}

/****** prompt() ******/
/* function provided by querier README, check the file for further details */
static void
prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

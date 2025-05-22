/* 
 * index.c - CS50 'index' module
 *
 * Bora Bozdogan, April 2025
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "mem.h"
#include "webpage.h"
#include "hashtable.h"
#include "counters.h"
#include "bag.h"
#include "pagedir.h"
#include "file.h"
#include "index.h"
#include "word.h"


/**************** local types ****************/
typedef struct index {
  hashtable_t* ht;          // hashtable keyed by word, storing counters as items
} index_t;

/**************** global functions ****************/
/* that is, visible outside this file */

/**************** index_new ****************/
/* Allocate and initialize an index */
/* see index.h for more information */
index_t* index_new(int numSlots) {
  index_t* index = mem_malloc(sizeof(index_t));

  if (index == NULL) {
    // error allocating memory for index; return error
    fprintf(stderr, "can't allocate memory for index");
    return NULL;
  }
  index->ht = hashtable_new(numSlots);
  return index;
}

/**************** index_add ****************/
/* add a word to the index */
/* see index.h for more information */
void index_add(index_t* index, char* word, int docID) {
  //check if key exists in index
  hashtable_t* ht = index->ht;
  counters_t* ctrs;
  if ((ctrs = hashtable_find(ht, word)) == NULL) {
    //key doesn't exist, create new counter with count as 0
    hashtable_insert(ht, word, ctrs = counters_new());
    counters_set(ctrs, docID, 0); //0 because gets incremented in indexer
  } else {
    //key exists, increase counter by 1
    counters_add(ctrs, docID);
  }
}

/**************** index_find ****************/
/* find counters for a word */
/* see index.h for more information */
counters_t* index_find(index_t* index, char* word) {
  //check if key exists in index
  hashtable_t* ht = index->ht;
  counters_t* ctrs;
  if ((ctrs = hashtable_find(ht, word)) == NULL) {
    //key doesn't exist, print error and exist nonzero
    fprintf(stderr, "word doesn't exist in index\n");
    exit(1);
  } 
  //key exists, return counter
  return ctrs;
}

/**************** index_set ****************/
/* set a counter value of index */
/* see index.h for more information */
void index_set(index_t* index, char* word, char* docID, int count) {
  //check if key exists in index
  hashtable_t* ht = index->ht;
  counters_t* ctrs;
  if ((ctrs = hashtable_find(ht, word)) == NULL) {
    //key doesn't exist, create new counter with count as 0
    hashtable_insert(ht, word, ctrs = counters_new());
    counters_set(ctrs, atoi(docID), count);
  } else {
    //key exists, increase counter by 1
    counters_set(ctrs, atoi(docID), count);
  }
}


/**************** index_check ****************/
/* check if a word is in index */
/* see index.h for more information */
bool index_check(index_t* index, char* word) {
  //true if hashtable finds word, else, false
  return (hashtable_find(index->ht, word) != NULL);
}

/**************** index_printValues ****************/
/* helper function that prints values of counters to a file */
/* not visible to outsiders, not in index.h */
static void index_printValues(void* arg, const int key, int count) {
  //write the docID (key) and count (int) pair on the file (arg)
  FILE* fp = arg;
  fprintf(fp, " %d %d", key, count);
}

/**************** index_getCounters ****************/
/* helper function that gets counters for index_save
 * and calls printValues for all of them
 */
/* not visible to outsiders, not in index.h */
static void index_getCounters(void* arg, const char* key, void* item) {
  FILE* fp = arg;
  counters_t* ctrs = item;
  //write the key (word) on the beginning of the file (arg)
  fprintf(fp, "%s", key);
  //call iterate on item (counters)
  counters_iterate(ctrs, fp, index_printValues);
  fprintf(fp, "\n");
}


/**************** index_save ****************/
/* save index content to a file */
/* see index.h for more information */
void index_save(index_t* index, char* indexFilename) {
  //check if the file can be opened to write
  hashtable_t* ht = index->ht;
  FILE* fp = fopen(indexFilename, "w");
  if (fp != NULL) {
    //loop over each element (word) of hashtable, call index_getCounters on each
    hashtable_iterate(ht, fp, index_getCounters);
    fclose(fp);
  } else {
    fprintf(stderr, "couldn't open file\n");
    exit(1);
  }
}

/**************** index_load() ****************/ 
 /* create index from file */
 /* see index.h for more information*/
index_t* index_load(char* indexFilename) {   
    //get file pointer, no need to check again after validateParams
    FILE* fp = fopen(indexFilename, "r");

    /*
    create index, use number of lines as hashtable slots as every line 
    corresponds to a word in the file that's being read. This is being 
    done instead of an arbitary number to be more memory-efficient.
    */
    index_t* index = index_new(file_numLines(fp));

    char* line;
    //read each line
    while ((line = file_readLine(fp)) != NULL) {
        
        //get words and initialize variables
        char** words = splitWords(line);
        int count = 0;

        //loop over words
        //starting from 1, pattern is 'docID counter' repeated
        //0 is the word itself, skip
        for (int i = 1; words[i] != NULL; i+=2) {
            char* docID = words[i];
            count = atoi(words[i+1]);
            index_set(index, words[0], docID, count);
        }

        //free the array of words and the line
        freeWords(words);
        free(line);

    }
    fclose(fp);
    return index;
}

/**************** itemdelete ****************/
/* index_delete helper function
 * 
 * deletes the counter
 * wrapper function for counters_delete to prevent compiler warnings
 * returns void
 *
*/
/* not visible to outsiders, not in index.h */
static void itemdelete(void* item) {
  counters_t* ctrs = item;
  counters_delete(ctrs);
}

/**************** index_delete ****************/
/* delete the index */
/* see index.h for more information */
void index_delete(index_t* index) {
  if (index != NULL) {
    //index is null, do nothing
    hashtable_delete(index->ht, itemdelete);
    free(index);
  } else if (index->ht == NULL) {
    //index is valid but hashtable null, just free index
    free(index);
  }
}


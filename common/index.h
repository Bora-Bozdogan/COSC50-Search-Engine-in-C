/* 
 * index.h - header file for CS50 'index' module
 * 
 * an index is a data structure that consists of a hashtable that
 * holds a word (char*) as key, and counters (counters_t*) as items
 * each counter holds docID (char*) as key with number of occurences of
 * the word on the file with corresponding docID (int) as items
 *
 * Bora Bozdogan, April 2025
 */
#ifndef INDEX_H
#define INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "mem.h"
#include "webpage.h"
#include "hashtable.h"
#include "bag.h"
#include "pagedir.h"
#include "file.h"
#include "index.h"
#include "word.h"

/**************** global types ****************/
typedef struct index index_t;  // opaque to users of the module

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index.
 *
 * We return:
 *   pointer to a new index, or NULL if error.
 * We guarantee:
 *   The hashtable of index is initialized empty
 *   with numSlots as slot count.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t* index_new(int numSlots);

/**************** index_add ****************/
/* Add new item to the index.
 *
 * Caller provides:
 *   a valid index pointer, a valid word string, a valid docID string.
 * We guarantee:
 *   if valid params, word counter is added to index
 *   counters value set to 1 if new, incremented if existing
 * Caller is responsible for:
 *   providing valid parameters
 */
void index_add(index_t* index, char* word, int docID);

/**************** index_set ****************/
/* Set an item value of index.
 *
 * Caller provides:
 *   a valid index pointer, a valid word string, a valid docID string.
 * We guarantee:
 *   if valid params, value of docID for a specific word is set to count
 *   if counter key doesn't exist, create new value, else, change value
 * Caller is responsible for:
 *   providing valid parameters
 */
void index_set(index_t* index, char* word, char* docID, int count);

/**************** index_check ****************/
/* check if a word is in the index
 *
 * Caller provides:
 *   valid index pointer.
 * We return:
 *   true if word exists in index
 *   false if not
 * We guarantee:
 *   the index is unchange.
 * Caller is responsible for:
 *   providing valid params.
 */
bool index_check(index_t* index, char* word);

/**************** index_save ****************/
/* Save the index to a file. 
 *
 * Caller provides:
 *   a valid index pointer, valid file location.
 * We guarantee:
 *   if fp null, exit nonzero
 *   for valid parameters and memory, print contents to indexFilename
 * We print: 
 *   a space-seperated list in the form of word docID count [docID count]
 */
void index_save(index_t* index, char* indexFilename);

/**************** index_delete ****************/
/* Delete the whole index.
 *
 * Caller provides:
 *   a valid index pointer.
 * We guarantee:
 *   we call itemdelete() on each item.
 *   we ignore NULL index.
 * Notes:
 *   the order in which items are deleted is undefined.
 */
void index_delete(index_t* index);

#endif // INDEX_H

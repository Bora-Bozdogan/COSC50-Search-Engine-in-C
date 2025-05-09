/* 
 * word.h - header file for CS50 'word' module
 *
 * word is a module that implements functions related to
 * the manipulation of char* words. 
 *
 * Bora Bozdogan, April 2025
 */

#ifndef WORD_H
#define WORD_H

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

/**************** normalizeWord ****************/
/* Normalize a word by converting to lowercase
 *
 * We return:
 *   void, char* is changed
 * We guarantee:
 *   a valid char* will be converted to lowercase
 * Caller is responsible for:
 *   providing valid parameters
 */
void normalizeWord(char* word);

/**************** countWords ****************/
/* Normalize a word by converting to lowercase
 *
 * We return:
 *   int, number of words in sentence
 * We guarantee:
 *   the word count of a valid char* will be returned
 * Caller is responsible for:
 *   providing valid parameters
 */
int countWords(const char* sentence);

/**************** splitWords ****************/
/* Split the words of sentence into an array 
 * 
 * We return:
 *   char**, array of words in the string
 * We guarantee:
 *   the array will be of exact size
 * Caller is responsible for:
 *   freeing the array of strings using freeWords
*/
char** splitWords(const char* sentence);

/**************** freeWords ****************/
/* free the array consisting of char* words
 * 
 * We return:
 *   void, input array is freed
 * We guarantee:
 *   a valid array will be freed
 * Caller is responsible for:
 *   providing valid parameters
*/
void freeWords(char** words);

#endif // WORD_H


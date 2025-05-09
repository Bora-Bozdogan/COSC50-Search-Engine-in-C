/* 
 * word.c - CS50 'word' module
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

/**************** global functions ****************/
/* that is, visible outside this file */

/**************** normalizeWord ****************/
/* normalize a word */
/* see word.h for more information */
void normalizeWord(char* word) {
    //implementation taken from webpage.h
    int len = strlen(word); //store strlen to prevent n**2 runtime
    for (int i = 0; i < len; i++) {
        word[i] = (char)tolower((unsigned char)word[i]);
    }
}

/**************** countWords ****************/
/* count the number of words in the sentence */
/* see word.h for more information */
int countWords(const char* sentence) {
    int size = 0;
    bool inWord = false;
    //iterate over each character in loop
    for (const char* p = sentence; *p != '\0'; p++) {
        char c = *p;
        if (c == ' ') {
            if (inWord) {
                size++; 
                inWord = false;
            }
        } else {
            inWord = true;
        }
    }

    if (inWord) {
        size++;
    }

    return size;
}

/**************** splitWords ****************/
/* split the words of sentence into an array */
/* see word.h for more information */
char** splitWords(const char* sentence) {
    //initilaize params
    int pos = 0;
    bool inWord = false;
    const char* start = NULL;
    int len = 0;
    //allocate array for words based on count
    char** words = malloc(sizeof(char*) * (countWords(sentence) + 1)); //+1 for NULL
    if (words == NULL) {
        return NULL;
    }
    //iterate over each character in loop
    for (const char* p = sentence; *p != '\0'; p++) {
        char c = *p;
        if (c != ' ' && c != '\0') {
            if (!inWord) {
                //start of word, get location & set inWord & set length of word as 1
                start = p;
                inWord = true;
                len = 1;
            } else {
                //in word and non null character, increment length of word
                len++;
            }
        } else {
            //null character
            if (inWord) {
                //exited a word, copy into words
                char* word = malloc(len + 1); //+1 for '\0'
                //copy string from start of word until start + length
                memcpy(word, start, len); 
                word[len] = '\0';
                words[pos] = word;
                pos++;
                inWord = false;
            }
        }
    }

    if (inWord) {
        //copy remaining word
        char* word = malloc(len + 1); //+1 for '\0'
        //copy string from start of word until start + length
        memcpy(word, start, len); 
        word[len] = '\0';
        words[pos] = word;
        pos++;
        inWord = false;
    }
    words[pos] = NULL; //add null terminator to the end of array
    return words;
}

/**************** freeWords ****************/
/* free the array consisting of char* words */
/* see word.h for more information */
void freeWords(char** words) {
    for (int i = 0; words[i] != NULL; i++) {
        free(words[i]);
    }
    free(words);
}
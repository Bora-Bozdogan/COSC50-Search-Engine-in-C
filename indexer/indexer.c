/* 
 * indexer.c - CS50 'indexer' program
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
#include "bag.h"
#include "pagedir.h"
#include "file.h"
#include "index.h"
#include "word.h"


//function headers
static bool validateParams(char* pageDirectory, char* indexFilename);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(webpage_t* webpage, int docID, index_t* index);

/* ***************************
 *  main - start of indexer.c
 *
 *  Parameters:
 *    argc - number of arguments
 *    argv - list of arguments
 *
 *  Returns:
 *    nonzero if error, void else    
 *
 *  Behavior:
 *    validates parameters, calls indexBuild to build index
 *    creates an index (refer to index.h for details) based on
 *    pageDirectory, writes it to indexFilename.
 *   
 */
int
main (int argc, char* argv[])
{   
    /*
    parse the command line, validate parameters, initialize other modules
    call indexBuild, with pageDirectory
    */
    //check argument count
    if (argc == 3) {
         //initialize variables
        char* pageDirectory = argv[1];
        char* indexFilename = argv[2];
        
        //validate parameters
        if (!validateParams(pageDirectory, indexFilename)) {
            exit(1);
        }
        //if successful, call indexBuild function 
        indexBuild(pageDirectory, indexFilename); 
    } else {
        fprintf(stderr, "\ninvalid argument number, please input 2\n");
        return 1;
    }
    return 0;
}

/**************** validateParams() ****************                                                                                
 *  validateParams - validate given parameters
 *
 *  Parameters:
 *    pageDirectory - char** pointer supposed to contain page directory
 *    indexFilename - char** pointer supposed to contain file name of index file
 *
 *  Returns:
 *     bool - true if valid params, false else
 *
 *  Behavior:
 *     checks if pageDirectory is the pathname directory produced
 *     by the crawler and indexFilename is the pathname of a file
 *     that can be written. Returns false on failure, completes
 *     succesfully and returns true if no errors.
 */
static bool validateParams(char* pageDirectory, char* indexFilename) {
    //check pagedirectory
    if (!pagedir_validate(pageDirectory)) {
        return false;
    }
    //create index file path for file name
    //check index file name
    FILE* fp = fopen(indexFilename, "w");
    if (fp == NULL) {
        //file can't be written
        fprintf(stderr, "Couldn't open file for writing\n");
        return false; //exit nonzero if couldn't write file, per requirements spec
    } else {
        //file name valid, close pointer
        fclose(fp);
    }
    return true;
}

/**************** indexBuild() ****************                                                                                
 *  indexFile - validate given parameters
 *
 *  Parameters:
 *    pageDirectory - char* that contains page directory
 *    indexFilename - char* that contains file name of index file
 *
 *  Returns:
 *     void
 *
 *  Behavior:
 *
 */
static void indexBuild(char* pageDirectory, char* indexFilename) {

    //create new index object
    index_t* index = index_new(800); //choose arbitrary size, per indexer specs
    int docID = 1;
    webpage_t* webpage;
    //loop over document ID numbers
    while ((webpage = pagedir_load(pageDirectory, docID)) != NULL) {
        //webpage exists, load the webpage from the document file 'pageDirectory/id'c
        indexPage(webpage, docID, index);
        //free pathname and webpage
        docID++;
        webpage_delete(webpage);
    }
    //save the index to a file
    index_save(index, indexFilename);

    //delete index
    index_delete(index);
}

/**************** indexPage() ****************                                                                                
 *  indexPage - validate given parameters
 *
 *  Parameters:
 *    pageDirectory - char* that contains page directory
 *    indexFilename - char* that contains file name of index file
 *
 *  Returns:
 *     void
 *
 *  Behavior:
 *
 */
static void indexPage(webpage_t* webpage, int docID, index_t* index) {
    int pos = 1;
    char* word;
    while((word = webpage_getNextWord(webpage, &pos)) != NULL) {
        //skip trivial words (less than length 3)
        if (strlen(word) >= 3) {
            //normalize the word (convert to lower case)
            normalizeWord(word);
            //look up the word in the index
            if (!index_check(index, word)) {
                //add the word to the index if needed
                index_add(index, word, docID);
            }
            //increment the count of occurences of this word in this docID
            //same function for adding and incrementing, check index.h
            index_add(index, word, docID); 
        }
        free(word);
    }

}
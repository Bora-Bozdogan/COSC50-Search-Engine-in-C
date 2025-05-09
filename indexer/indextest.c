/* 
 * indextest.c - CS50 'indextest' program
 *
 * Bora Bozdogan, April 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "file.h"
#include "index.h"

static bool validateParams(char* indexFilename, char* newIndexFilename);
static index_t* indextest_read(char* indexFilename);

 /* ***************************
 *  main - start of indextest.c
 *
 *  Parameters:
 *    argc - number of arguments
 *    argv - list of arguments
 *
 *  Returns:
 *    nonzero if error, void else    
 *
 *  Behavior:
 *    reads an index file into the internal index data structure
 *    writes out to a new file   
 *
 */
int
main (int argc, char* argv[])
{   
    if (argc == 3) { //check number of parameters
        //first parameter, existing index file location
        //second parameter, new index file location
        char* indexFilename = argv[1];
        char* newIndexFilename = argv[2];

        //still validate parameters for better defensive programming
        if (!validateParams(indexFilename, newIndexFilename)) {
            exit(1);
        }

        //create an index, fill based on file
        index_t* index = indextest_read(indexFilename);

        //write the index to another file
        index_save(index, newIndexFilename);

        //free the index
        index_delete(index);
    } else {
        fprintf(stderr, "invalid number of arguments, please input 2");
        return 1;
    }
}

/**************** validateParams() ****************                                                                                
 *  validateParams - validate given parameters
 *
 *  Parameters:
 *    indexFilename - char* supposed to contain location of existing index file
 *    newIndexFilename - char* supposed to contain location for new index file
 *
 *  Returns:
 *     bool - true if valid params, false else
 *
 *  Behavior:
 *     checks if indexFilename is the pathname directory containing
 *     a readable file and newIndexFilename is the pathname of a file
 *     that can be written. Returns false on failure, completes
 *     succesfully and returns true if no errors.
 */
static bool validateParams(char* indexFilename, char* newIndexFilename) {   
        
    //try opening existing file to read
    FILE* fp = fopen(indexFilename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Can't open the existing index file");
        return false;
    } else {
        fclose(fp);
    }

    //try creating new file to write
    fp = fopen(newIndexFilename, "w");
    if (fp == NULL) {
       fprintf(stderr, "Can't create the new index file");
        return false;
    } else {
        fclose(fp);
    }

    return true;

}


 /* ***************************
 *  readIndex - reads a file, creates an index based on it
 *
 *  Parameters:
 *    indexFilename - char* location of existing index
 *    newIndexFilename - char* location to write new file
 *
 *  Returns:
 *    nonzero if error, void else    
 *
 *  Behavior:
 *    reads a file, creates index based on it  
 *
 */
static index_t* indextest_read(char* indexFilename) {   
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

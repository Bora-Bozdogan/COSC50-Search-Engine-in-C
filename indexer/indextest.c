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
        index_t* index = index_load(indexFilename);

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
/* 
 * pagedir.h - header file for CS50 pagedir module
 *
 * pagedir module handles the operations related to creating filepaths for
 * .crawler constructs or webpages, and writing to them
 *
 * Bora Bozdogan, April 2025
 */

#include <stdio.h>
#include <stdbool.h>
#include "webpage.h"
#include "mem.h"

/**************** global types ****************/
// opaque to users of the module

/**************** functions ****************/

/**************** pagedir_init ****************/
/* construct a pathname for .crawler in pageDirectory, test opening it
 * 
 * We return:
 *   true if file is succesfully opened
 *   false if the file couldn't be opened
 * We guarantee:
 *   file path is unchanged
 * Caller is responsible for:
 *   using pagedir_save to write to file path
 */
bool pagedir_init(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Construct filepath for page. Write URL, depth, content.
 *
 * We return:
 *      void, error if invalid file open or write
 * We guarantee:
 *      with valid parameters, and enough memory,
 *      a file named docID, with URL, depth, and content writted will be saved 
 *      on pageDirectory
 * Caller is responsible for:
 *      providing valid page, pageDirectory, and docID values
*/
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);



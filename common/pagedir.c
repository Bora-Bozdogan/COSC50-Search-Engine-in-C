/* 
 * pagedir.c - CS50 'pagedir' program
 *
 * see pagedir.h for more information.
 *
 * Bora Bozdogan, April 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include <math.h>
#include <stdbool.h>
#include "webpage.h"
#include "file.h"

/**************** pagedir_init() ****************/
/* see pagedir.h for more information */
bool pagedir_init(const char* pageDirectory) {
    char* pathname;
    int len = strlen(pageDirectory);
    //ends with '/' (../cs50dev/ instead of ../cs50dev)
    bool hasSlash = (pageDirectory[strlen(pageDirectory) - 1] == '/');
    
    if (hasSlash) {
        //ends with '/', only add '.crawler' for pathname
        pathname = malloc(len + 9); //len + 9 because adding .crawler + \0
        strcpy(pathname, pageDirectory);
        strcat(pathname, ".crawler");
    } else {
        //doesn't end with '/', add '/.crawler' for pathname
        pathname = malloc(len + 10); //len + 10 because adding .crawler + \0
        strcpy(pathname, pageDirectory);
        strcat(pathname, "/.crawler");
    }

    //try opening file for writing, return false if error, return true else
    FILE* fp = fopen(pathname, "w");
    if (fp == NULL) {
        free(pathname);
        return false;
    }
    fclose(fp);
    free(pathname);
    return true;
}

/**************** pagedir_save() ****************/
/* see pagedir.h for more information */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    
    char* pathname;

    //gets how many digits the number is to allocate array
    int digits;
    if (docID == 0) {
        digits = 1;
    } else {
        digits = (int) log10(docID) + 1;
    }   
    //turn docID to a string, digits + 1 because of \0
    char id[digits + 1];
    snprintf(id, sizeof(id), "%d", docID);

    int len = strlen(pageDirectory);
    //ends with '/' (../cs50dev/ instead of ../cs50dev)
    bool hasSlash = (pageDirectory[strlen(pageDirectory) - 1] == '/');
    if (hasSlash) {
        //ends with '/', only add doc id for pathname
        pathname = malloc(len + strlen(id) + 1); 
        strcpy(pathname, pageDirectory);
        strcat(pathname, id);
    } else {
        //doesn't end with '/', add '/' and if for pathname
        pathname = malloc(len + strlen(id) + 2);
        strcpy(pathname, pageDirectory);
        strcat(pathname, "/");
        strcat(pathname, id);
    }

    //open file for writing
    FILE* fp = fopen(pathname, "w");
    if (fp == NULL) {
        //error
        fprintf(stderr, "Couldn't open file for writing");
        free(pathname);
        exit(1); //exit nonzero if couldn't write file, per requirements spec
    } else {
        //opened file
        //print URL on first line, depth on second, and contents on the rest
        fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
            //close file
            fclose(fp);
            free(pathname);
    }
}
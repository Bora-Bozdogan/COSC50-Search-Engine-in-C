/* 
 * crawler.c - CS50 'crawler' program
 *
 * see crawler.h for more information.
 *
 * Bora Bozdogan, April 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "webpage.h"
#include "hashtable.h"
#include "bag.h"
#include "pagedir.h"
#include "file.h"


static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);

static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

int
main (int argc, char* argv[])
{
    if (argc == 4) {
        //parse command line
        char* seedURL = argv[1];
        char* pageDirectory = argv[2];
        int maxDepth = atoi(argv[3]);
        parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

        //run crawl
        crawl(seedURL, pageDirectory, maxDepth);
    } else {
        fprintf(stderr, "\ninvalid argument number, please input 3\n");
        return 1;
    }
}

/**************** parseArgs() ****************/
/* given args from command line, extract them into function params
 * return only if succesful
*/
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth) {
    
    char* normalizedUrl = normalizeURL(*seedURL);
    //if internal, and can be normalized
    if (normalizedUrl != NULL && isInternalURL(normalizedUrl)) {
        //normalize seedUrl
        *seedURL = normalizedUrl;
    } else {
        //invalid url, print to stderr and exit nonzero
        fprintf(stderr, "\nInvalid URL\n");
        exit(1);
    }

    bool isInit = pagedir_init(*pageDirectory);
    //if succesfully initialized pagedir
    if (isInit == false) {
        //invalid page dir, print to stderr and exit nonzero
        fprintf(stderr, "\nInvalid page directory\n");
        exit(1);
    }

    if (*maxDepth < 0 || *maxDepth > 10) { 
        //out of range specified in requirement specs
        fprintf(stderr, "\nInvalid max depth, please choose [0, 10]\n");
        exit(1);
    }

}

/**************** crawl() ****************/
/* go through a page, check every link, save as webpages if internal and
 * valid. Returns void, adds valid webpages to pagedirectory specified.
*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) {
    //crawl from seedURL to maxDepth and save pages in pageDirectory
    //initialize docId counter
    int docId = 1;

    //initialize hashtable urls, add seedUrl 
    hashtable_t* visited = hashtable_new(200); //200 slots is stated to be used
    hashtable_insert(visited, seedURL, "");
    
    //initialize bag and add webpage at depth 0
    bag_t* toVisit = bag_new();
    webpage_t* init = webpage_new(seedURL, 0, NULL);
    if (init == NULL) {
        fprintf(stderr, "\nCouldn't create webpage\n");
        exit(1);
    } 
    bag_insert(toVisit, init);
    webpage_t *webpage;
    //while bag not empty, pull a webpage
    while ((webpage = bag_extract(toVisit)) != NULL) {
        //fetch the HTML for it
        if (webpage_fetch(webpage) == false) {
            fprintf(stderr, "\nCouldn't get html for webpage\n");
            exit(1);
        }
        printf("Fetched: %s\n", webpage_getURL(webpage));
        //if fetch was succesfull
        //save webpage to pageDirectory
        pagedir_save(webpage, pageDirectory, docId);
        docId++; 

        //if webpage is not at maxDepth
        if (webpage_getDepth(webpage) < maxDepth) {
            //pageScan the HTML
            printf("Scanning: %s\n", webpage_getURL(webpage));
            pageScan(webpage, toVisit, visited);
        }            
        //delete webpage
        webpage_delete(webpage);
    }
    //delete hashtable
    hashtable_delete(visited, NULL); //null itemdelete because items aren't malloc'd
    //delete bag
    bag_delete(toVisit, NULL); //null itemdelete because used webpage_delete 
    //exit 0 if successful, per requirements spec
    exit(0);
}

/**************** pageScan() ****************/
/* scan a page, go through every URL in the file, add to bag of pages visited
 * returns void, updates bag and hashtable.
*/
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {

    int pos = 0;
    char* nextURL = webpage_getNextURL(page, &pos);

    //while there is another URL in the page
    while (nextURL != NULL) {
        //normalize the URL
        char* normalizedUrl = normalizeURL(nextURL);
        printf("Found: %s\n", normalizedUrl);
        //if internal, and can be normalized
        if (normalizedUrl != NULL) {
            //normalize seedUrl
            free(nextURL);
            nextURL = normalizedUrl;
        } else {
            //invalid url, print to stderr and exit nonzero
            fprintf(stderr, "\nInvalid URL\n");
            free(nextURL);
            exit(1);
        } 

        //if that URL is Internal
        if (isInternalURL(nextURL)) {
            //insert the webpage into the hashtable
            //if that succeeded
            if (hashtable_insert(pagesSeen, nextURL, "")) {
                //create a webpage_t for it
                webpage_t* webpage = webpage_new(nextURL, webpage_getDepth(page) + 1, NULL);
                if (webpage == NULL) {
                    fprintf(stderr, "Couldn't create webpage");
                    exit(1);
                } 
                //insert the webpage into the bag
                printf("Added: %s\n", nextURL);
                bag_insert(pagesToCrawl, webpage);
            } else {
                //free this URL and get next URL
                printf("IgnDupl: %s\n", nextURL);
                free(normalizedUrl);
            }
        } else {
            //free this URL and get next URL
            printf("IgnExtrn: %s\n", nextURL);
            free(normalizedUrl);
        }
        nextURL = webpage_getNextURL(page, &pos);
    }
}

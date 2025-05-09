# CS50 TSE Indexer
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
The knowledge unit noted that an [implementation spec](https://github.com/CS50DartmouthSP25/home/blob/main/knowledge/units/design.md#implementation-spec) may include many topics; not all are relevant to the TSE or the Indexer.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use a data structure: an 'index' that consists of a hashtable that holds counters.
Each key is a word seen in the text, with item being a counter.
The key of the counter is docID of text, with word's count in that docID file.
The index starts empty, and gets built with indexPage and indexBuild functions.
The size of the hashtable of index (slots) is impossible to determine in advance, so we use 800.
We use exact mallocs in all other instances based on input size, and parameters.

## Control flow

The Indexer is implemented in one file `indexer.c`, with three functions.

### main

The `main` function checks argument number, validates parameters, calls indexBuild, and exits 0.
It exists 1 on any failure.

### validateParams

Given arguments from the command line, validate them; return only if successful.

* for `pageDirectory`, try opening the file
* for `indexFilename`, check if file is writeable
* if any trouble is found, print an error to stderr and exit non-zero.

### indexBuild

Create a index and call indexPage on every page to fill it.

Pseudocode:
    create a new 'index' object
    loop over document ID numbers, counting from 1
        load a webpage from the document file 'pageDirectory/id'
        if successful, 
            pass the webpage and docID to indexPage

### indexPage

Given a `webpage`, use `webpage_getNextWord` to get all words individually, ignoring trivial words (less than length 3); normalize the word using `normalizeWord` (convert to lowercase), if word not in `index`, add it, and increment its value.

Pseudocode:
	step through each word of the webpage,
	skip trivial words (less than length 3),
	normalize the word (converts to lower case),
	look up the word in the index,
		add the word to the index if needed
	increment the count of occurrences of this word in this docID

## Other modules

### pagedir

We create a re-usable module `pagedir.c` to handles the *pagesaver*  mentioned in the design (writing a page to the pageDirectory), and marking it as a Crawler-produced pageDirectory (as required in the spec).
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place... anticipating future use by the Indexer and Querier.

Pseudocode for `pagedir_init`:

	construct the pathname for the .crawler file in that directory
	open the file for writing; on error, return false.
	close the file and return true.


Pseudocode for `pagedir_save`:

	construct the pathname for the page file in pageDirectory
	open that file for writing
	print the URL
	print the depth
	print the contents of the webpage
	close the file

Pseudocode for `pagedir_validate`:

    construct the pathname for the .crawler file in that directory
	open the file for reading; on error, return false.
	close the file and return true.

Pseudocode for `pagedir_load`:

    construct the pathname for the page file in pageDirectory
	open that file for reading
	get the URL
	get the contents of the webpage
    create a webpage from url and contents
    free strings
    close the file


### index

We create a re-usable module `index.c` to handle index data structure mentioned in the design (holding numbers of words for each docID).
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to add or check values in index, and how to save it to another file... anticipating future use by the Indexer and Querier.

Pseudocode for `index_new`:
    allocate memory for index
    if can't
        print error and exit nonzero
    allocate memory for hashtable of index
    return index

Pseudocode for `index_add`:
    if word not in index
        add counter to index with word as key
        initialize counter to zero
    increment the counter of word for given docID

Pseudocode for `index_set`:
    if word not in index
        add counter to index with word as key
        initialize counter to zero
    set the counter value of word to given value

Pseudocode for `index_check`:
    return if word is in index

Pseudocode for `index_save`:
    open file
    for each counter in hashtable
        print word of counter to file
        for each key-value pair of counter
            print in 'key value' format to file
        print a new line
    close file
    

Pseudocode for `index_delete`:
    if index not null
        if hashtable not null
            delete hashtable and every element
            free index
        else
            free index

### word

we create the re-usable module `word.c` to handle any repeated transformations we have to conduct on char* words. We write this in a seperate file in `../common` to encapsulate all the knowledge about how to manipulate words, anticipating future use by the Indexer and Querier.

Pseudocode for `normalizeWord`:
    for every char of word
        call tolower() on char and assign it to its original slot

### libcs50

We leverage the modules of libcs50, most notably `bag`, `hashtable`, and `webpage`.
See that directory for module interfaces.
The new `webpage` module allows us to represent pages as `webpage_t` objects, to fetch a page from the Internet, and to scan a (fetched) page for URLs; in that regard, it serves as the *pagefetcher* described in the design.
Indeed, `webpage_fetch` enforces the 1-second delay for each fetch, so our crawler need not implement that part of the spec.

## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `indexer.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static bool validateParams(char** pageDirectory, char** indexFilename);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(webpage_t* webpage, char* docID, index_t* index);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
```

### index

Detailed descriptions of each function's interface is provided in `index.h` and is not repeated here.

```c
index_t* index_new(int numSlots);
void index_add(index_t* index, char* word, char* docID);
void index_set(index_t* index, char* word, char* docID, int count);
bool index_check(index_t* index, char* word);
void index_save(index_t* index, char* indexFilename);
void index_delete();
```

### word

Detailed descriptions of each function's interface is provided in `word.h` and is not repeated here.
```c
void normalizeWord(char* word);
```


## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_init` returns false if there is any trouble creating the `.crawler` file, allowing the Crawler to decide what to do; the `webpage` module returns false when URLs are not retrievable, and the Crawler does not treat that as a fatal error.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There are three units (indexer, word, and index).
The indexer represents the whole system. A program indextest will serve as a unit test for the index module; it reads an index file into the internal index data structure, then writes the index out to a new index file.
The index unit is strictly dependent to the indexer's functionality. So observing the outputs, and comparing them with files fed will be enough. If the indexer is functional, then it would be sensible to assume index works as intended.
The word unit currently consists of a single function, and it can be tested simply with a testing file like "normalizeWord("TEST")". Moreover, the indexer's functionality also depends on word, so output can be observed and checked for words that are converted to lowercase correctly.

### Regression testing

The indexer can take a long time to run on some sites when the pagedirectory consists of a great number of files produced by the crawler.
For routine regression tests, we index the `letters` files produced by crawler at depth 10; save the index.txt file created by indexer from one working run to compare (with `diff -r`) against future runs.

### Integration/system testing

Integration testing for the indexer includes confirming it as a full program by creating an index from pageDirectory containing crawler produced files and verifying its correctness by passing the resulting index file through indextest, ensuring it can be reloaded and saved without problems. 
Testing process includes running the indexer with a variety of invalid arguments: no arguments, a single argument, too many arguments, invalid or non-existent pageDirectory, pageDirectory lacking the .crawler file, invalid indexFilename such as one in a non-existent or read-only directory, or an already existing but unwritable file. 
Functional runs are performed on various valid crawler-made pageDirectory inputs (e.g., letters, toscrape, wikipedia at different depths), and output index files are validated using indextest. Additionally, both programs (indexer and indextest) are checked with Valgrind to make sure no memory leaks or invalid memory accesses exist.






# CS50 TSE Querier
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.

## Data structures 

We use two main data structures: 

an 'index' that is loaded from `indexFilename` using `index_load`, which holds the docID-count information for each word

'counters_t' structures that are used in the index, as well as to hold partial and total scores computed in `querier_process`

## Control flow

The querier is implemented in one file `querier.c`, with two main functions: `querier_parse` and `querier_process`, as well as other functions used for checking words and validating parameters and helper functions used for computing and ranking scores.

### main

validate args, call functions to parse and process query, and rank it

Pseudocode: 

    check argument count
    validate parameters
    initialize index
    while !eof call `querier_parse` to get clean query
        call `querier_process` on query to get scores
        while highest score in scores != 0
            get highest score, print score-docid-url
            set highest score to 0
    free variables
    exit zero


### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

for `pageDirectory`, call `pagedir_validate` to confirm it is a
readable file produced by the crawler
for `indexFilename`, check if the file is readable
if any trouble is found, print an error to stderr and exit non-zero.

### querier_parse

Normalize the input given and check if it fits the backus-naur syntax. Return null on invalid query, return normalized query if valid

Pseudocode:

    if query blank
        return null
    split query into an array of words using `splitWords`
    for each word in array
        normalize word by converting it to lowercase using `normalizeWord`
    print clean query for user
    if first word literal
        return NULL
    loop over words until before last word
        if two adjacent literals
            return NULL
        if last word literal
            return NULL
    loop over all words
        if word has invalid characters
            return NULL
    return clean query

### querier_process

scores the documents based on how well it fits the valid query given by `querier_parse`

Pseudocode:

    initialize counters partialScores and totalScores
    loop over words in query
        if word and
            skip
        if word or  
            add partialScores to totalScores
            reset partialScores
        else 
            if word in index
                get counter for current word in index
                if first word
                    initialize score of each word in partialScores
                else 
                    update partialScores based on already initialized docIDs
            else 
                set all partialscores to 0
    add last set of partialScores to totalScores
    delete partialScores
    return totalScores

## Other modules

### index

We use the index module for holding docid-frequency pairs for each word. Check index.h for more details

We add index two new functions, `index_find` and `index_load`

Pseudocode for index_find

    if key doesn't exist in index
        exit nonzero
    return counters associated with key

Pseudocode for index_load

    loop over words in line of given file
        set index with word as key, counters as item
        docID as counters key, word count as counters item
    return index

### word

We use the word module to normalize words and split sentences into array of words, check word.h for more details.


### libcs50

We leverage the modules of libcs50, most notably `counters`
See that directory for module interfaces.

## Function prototypes

### querier

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `querier.c` and is not repeated here.

```c
main (int argc, char* argv[]);
static bool querier_validateParams(char* pageDirectory, char* indexFilename);
static char** querier_parse(char* line);
static counters_t* querier_process(char** words, index_t* index);
static void addScores(void* arg, const int key, const int count);
static void updateScores(void* arg, const int key, const int count);
static void initScores(void* arg, const int key, const int count);
static void getHighestScore(void* arg, const int key, const int count);
static bool querier_isLiteral(char* word);
int fileno(FILE *stream);
static void prompt(void);

```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit, e.g., if a function receives bad parameters.

Since all functions are static, and depend on the initial parameters supplied, querier_validateParams validates the arguments, making the possibility of a bad argument error rare.


## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There is only a single unit, the querier. It represents the whole system and is covered below.
The extra functions implemented in index are closely tied to the querier, and it is sufficient to observe their function during querier running.

### Integration/system testing

Please refer to the design spec for detailed testing cases.

We write a script `testing.sh` that invokes the querier several times, with a variety of command-line and input arguments.

First, a sequence of invocations with erroneous command line, testing each of the possible mistakes that can be made.

Second, runs that test different numbered queries, different combinations of word in index and word not in index, and different spacing and capitalizations.

Third, runs that test different queries with valgrind.

We run the test with the `make test` command of the makefile, which invokes `./testing.sh`

We verify correct behavior by studying the output, and cross checking with their respective index files.

# CS50 TSE Querier
## Design Spec

According to the [Querier Requirements Spec](REQUIREMENTS.md), the TSE *querier* is a standalone program that gets input from user, turns the input into a query, prints out the documents produced by the crawler that match the query the best based on the index file produced by the indexer.

### User interface

The querier's first interface with the user is on the command-line; it must always have two arguments.

```
querier pageDirectory indexFilename
```

For example, if `letters` is a pageDirectory in `../data`,

``` bash
$ querier ../data/letters ../data/letters.index
```

The querier's second interface with the user is the stdin, where the querier accepts strings to turn to queries. Each line
is cleaned and parsed and processed to return documents that match the best. Queries are tokenized into words and 'and' - 'or' literals.


### Inputs and outputs

**Input**: the querier reads files from a directory by constructing file pathnames from the `pageDirectory` parameter followed by a numeric document ID.

The querier reads document files in sequential ID order, beginning at 1, until is unable to open one of those files.

The querier loads an index from the path `indexFilename` into an internal index data structure.

The querier accepts user queries from stdin, one line at a time, to process as explained in requirements. 

**Output**: 
as described in the requirements spec,

for each valid query, the querier prints lines of the form 
```
<score> <docID> <URL>
```

in descending score order

if no documents match, prints

```
No documents match
```

prints error messages to stderr on any errors


### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which checks argument count, validats parameters, reads queries one line until EOF, prints best results for each query.
 2. *querier_validateParams*, which checks if pageDirectory and indexFilename are valid, readable file paths
 3. *querier_parse*, cleans the query and prints error message if invalid query
 4. *querier_process*, scores the documents based on how well it fits the query 
 5. *getHighestScore* helper function for *main* that returns the highest score in scores returned by *querier_process*
 6. *addScores, initScores, updateScores* helper functions for *querier_process* that create scores for documents per query
 7. *querier_isLiteral* which checks if a word is 'and' or 'or'

And some helper modules that provide data structures:

 1. *index*, a module providing the data structure to represent the in-memory index, and functions to read and write index files, as well as create index files from documents.
 2. *webpage*, a module providing the data structure to represent webpages, and a function to get the URL for a webpage.
 3. *pagedir*, a module providing functions to load webpages from files in the pageDirectory;
 4. *word*, a module providing a function to normalize a word, and create an array of words from a sentence.
 5. *counters* a module providing data structure counters used to hold scores and document ids

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validate parameters, initialize other modules
    while not end of file
      parse query with querier_parse
      create scores for docIDs with querier_process
      print score-docID-url in decreasing order
      

where *querier_parse:*
    
    check for blank lines
    split the sentence into words and normalize each word 
    print clean query
    check if first word is a literal
    loop over the words
      check for adjacent literals
      check for literal as last word
      check for invalid characters
    return words in array

where *querier_process:*

    initialize counters partialScores, totalScores

    loop over words of valid query
      if word not 'or'
        make partialScores for each docID equal to the count of word in query that appears least in docID
      if word 'or'
        add partialScores to totalScores, reset partialScores
    return partialScores

### Major data structures

  *index_t* data struct holding docID-word frequency information for each word
  *counters_t* data struct holding key-item pairs, maps docID to word count or score, used for partial and total score calculation, and in index
  *node_t* data structure to be used in *main*, holding a key and score to store the highest score
  *countersHolder_t* data structure holding two counters to be used in *updateScores*

### Testing plan
*Unit testing* The *querier* itself will work as unit testing, using all the modules mentioned to provide rankings.

*Integration testing*.  The *querier*, as a complete program, will be tested by running the program under the following conditions, to test all modules comprehensively.

1. no arguments  
2. a single argument  
3. too many arguments  
4. invalid and non-existent pageDirectory  
5. invalid indexFilename (directory path)  
6. query that starts with an 'or' or 'and'  
7. query that has adjacent 'or' and 'and's  
8. query that ends with an 'or' or 'and'  
9. query that has invalid characters  
10. single word query  
11. query with no output (no documents found)  
12. and query (implicit and explicit)  
13. or query  
14. and or query  
15. in index and not in index query  
16. in index or not in index query  
17. random spacing query  
18. random capitalization query  
19. valgrind and query  
20. valgrind or query  
21. valgrind and or query

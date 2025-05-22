#!/bin/bash

# no arguments
echo
echo "no arguments"
./querier

# a single argument
echo
echo "a single argument"
./querier ../data/letters/0

# too many arguments
echo
echo "too many arguments"
./querier ../data/letters/0 output1.txt extra

# invalid and non-existent pageDirectory
echo
echo "invalid and non-existent pageDirectory"
./querier ../data/nonexistent output2.txt

# invalid indexFilename (directory path)
echo
echo "invalid indexFilename (directory path)"
./querier ../data/toscrape/1/ invalidIndexfilename

# query that starts with an 'or' or 'and'
echo 
echo "query that starts with an 'or' or 'and'"
echo "or bread" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# query that has adjacent 'or' and 'and's
echo 
echo "query that has adjacent 'or' and 'and's"
echo "white and or black" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# query that ends with an 'or' or 'and'
echo 
echo "query that ends with an 'or' or 'and'"
echo "bread or" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# query that has invalid characters
echo 
echo "query that has invalid characters"
echo "bread!" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# single word query
echo 
echo "single word query"
echo "bread" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# query with no output (no documents found)
echo 
echo "query with no output (no documents found)"
echo "awordthatsnotinindex" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# and query (implicit and explict)
echo 
echo "and query (implicit and explict)"
echo "street and affairs" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index
echo "street affairs" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# or query
echo 
echo "or query"
echo "bread or ice" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# and or query
echo 
echo "and or query"
echo "capable wise or drawing and applicants" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index 

# in index and not in index query
echo 
echo "in index and not in index query"
echo "capable and wordnotinindex" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# in index or not in index query
echo 
echo "in index or not in index query"
echo "bread or wordnotinindex" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# random spacing query
echo 
echo "random spacing query"
echo "capable        wise      or   butter" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# random capitalization query
echo 
echo "random capitalization query"
echo "bReaD oR bUtTeR" | ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# valgrind and query
echo 
echo "valgrind and query"
echo "capable and wise" | valgrind --leak-check=full ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index 

# valgrind or query
echo 
echo "valgrind or query"
echo "capable or wise" | valgrind --leak-check=full ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index

# valgrind and or query
echo 
echo "valgrind and or query"
echo "capable wise or drawing and applicants" | valgrind --leak-check=full ./querier ../data/toscrape/1/ ../data/indexes/toscrape1.index



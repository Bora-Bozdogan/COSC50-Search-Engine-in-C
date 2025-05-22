I have implemented all the functionality for the querier. It supports 'and' and 'or'
operators with 'or' having precedence over 'and', and it prints the results by decreasing score, meeting the full requirements.

ASSUMPTIONS and DIFFERENCES: 

I assumed that two types of directory inputs were possible
    1- ends with slash (../cs50dev/)
    2- doesn't end with slash (../cs50dev/)
    therefore I made the memory allocation a dynamic process ready for both cases.

I assumed I can modify index.c to have index_load, which creates an index from a filename
since it was used in numerous occasions, and just call this function for querier and indextest

I assumed I can modify index.c to have a function that gives the hashtable for more detailed operations

I assumed the score wouldn't reach INT_MAX which is 2 ^ 31, therefore I didn't require a check to cap it at INT_MAX

Differently from the shown error examples, 
I assumed I should print the query before the error for the invalid characters as well, as it is better for
users to see their query. 

I assumed I don't need to implement the querier exact same way explained in the README file, as my implementation
is fully functional, has no memory leaks, or isn't less efficient.
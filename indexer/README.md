ASSUMPTIONS and DIFFERENCES: 

I assumed we're making the argument check in the main, as the function 
description doesn't involve checking for argument count.

I assumed the pathname provided might or might not have a / at the end (./cs50-
dev/ or ./cs50-dev), so I made the file checks dynamic

I assumed I can write a parseArgs function instead of parsing them in main,
as it enabled a clearer code 

I assumed I don't need to write the helper functions for functions like index_delete in index.h or implementation.md
since they are never used outside of the specific function scope.
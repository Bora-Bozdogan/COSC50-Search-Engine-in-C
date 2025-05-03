!/bin/bash

#tests are based on github page for crawler recommendations

#invalid checks
#invalid argument count
echo
echo invalid argument count:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters/0 0 12

#invalid url
echo
echo invalid url:
./crawler http://ahmedwahabsavestheworldfromdecepticons.html ../data/letters/0 0

#invalid page directory
echo
echo invalid page directory:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../ahmed/0 0

#invalid depth
echo
echo invalid depth:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters/0 6000

#valid url but not internal
echo
echo valid url but not internal:
./crawler https://en.wikipedia.org/wiki/Algorithm ../data/letters/0 0

#create necessary directories
rm -rf ../data

mkdir ../data
mkdir ../data/letters
mkdir ../data/letters/0
mkdir ../data/letters/1
mkdir ../data/letters/10 
mkdir ../data/letters/memorycheck1

mkdir ../data/toscrape
mkdir ../data/toscrape/0
mkdir ../data/toscrape/1

mkdir ../data/wikipedia
mkdir ../data/wikipedia/0
mkdir ../data/wikipedia/1
mkdir ../data/wikipedia/memorycheck2

#test letters at depths 0-10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters/0 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters/1 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters/10 10

#test toscrape at depths 0-1, 0 being valgrind
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape/0 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape/1/ 1

#test wikipedia at depths 0-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia/0/ 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia/1/ 1

#letters at depths 10 with valgrind and wikipedia 0 with valgrind
echo
echo letters at depths 10 with valgrind and wikipedia 0 with valgrind
valgrind --leak-check=full ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters/memorycheck1 10
valgrind --leak-check=full ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia/memorycheck2/ 0

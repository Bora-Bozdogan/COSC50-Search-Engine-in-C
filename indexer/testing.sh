echo "letters depth 0 with output files put through indextest"
./indexer ../data/letters/0 ../data/letters-0.txt
./indextest ../data/letters-0.txt ../data/letters-0-copy.txt
~/cs50-dev/shared/tse/indexcmp ../data/letters-0.txt ../data/letters-0-copy.txt

#!/bin/bash

# variety of invalid arguments

# no arguments
echo
echo "no arguments"
./indexer

# a single argument
echo
echo "a single argument"
./indexer ../data/letters/0

# too many arguments
echo
echo "too many arguments"
./indexer ../data/letters/0 output1.txt extra

# invalid and non-existent pageDirectory
echo
echo "invalid and non-existent pageDirectory"
./indexer ../data/nonexistent output2.txt

# pageDirectory lacking the .crawler file
echo
echo "pageDirectory lacking the .crawler file"
mkdir tempdir
touch tempdir/somefile
./indexer tempdir output3.txt
rm -r tempdir

# invalid indexFilename (directory path)
echo
echo "invalid indexFilename (directory path)"
mkdir readonlydir
chmod -w readonlydir
./indexer ../data/letters/0 readonlydir/
chmod +w readonlydir
rmdir readonlydir

# already existing but unwriteable indexFilename
echo
echo "already existing but unwriteable indexFilename"
touch unwritable.txt
chmod -w unwritable.txt
./indexer ../data/letters/0 unwritable.txt
chmod +w unwritable.txt
rm unwritable.txt

# letters depths 0,1,10 with output files put through indextest and compared through indexcmp
echo
echo "letters depth 0 with output files put through indextest and compared through indexcmp"
./indexer ../data/letters/0 ../data/letters-0.txt
./indextest ../data/letters-0.txt ../data/letters-0-copy.txt
~/cs50-dev/shared/tse/indexcmp ../data/letters-0.txt ../data/letters-0-copy.txt

echo
echo "letters depth 1 with output files put through indextest and compared through indexcmp"
./indexer ../data/letters/1 ../data/letters-1.txt
./indextest ../data/letters-1.txt ../data/letters-1-copy.txt
~/cs50-dev/shared/tse/indexcmp ../data/letters-1.txt ../data/letters-1-copy.txt

echo
echo "letters depth 10 with output files put through indextest and compared through indexcmp"
./indexer ../data/letters/10 ../data/letters-10.txt
./indextest ../data/letters-10.txt ../data/letters-10-copy.txt
~/cs50-dev/shared/tse/indexcmp ../data/letters-10.txt ../data/letters-10-copy.txt

# toscrape depth 0 with output files put through indextest and compared through indexcmp
echo
echo "toscrape depth 0 with output files put through indextest and compared through indexcmp"
./indexer ../data/toscrape/0 ../data/toscrape-0.txt
./indextest ../data/toscrape-0.txt ../data/toscrape-0-copy.txt
~/cs50-dev/shared/tse/indexcmp ../data/toscrape-0.txt ../data/toscrape-0-copy.txt

echo
echo "toscrape depth 1 with output files put through indextest and compared through indexcmp"
./indexer ../data/toscrape/1 ../data/toscrape-1.txt
./indextest ../data/toscrape-1.txt ../data/toscrape-1-copy.txt
~/cs50-dev/shared/tse/indexcmp ../data/toscrape-1.txt ../data/toscrape-1-copy.txt

# wikipedia depth 0 with output files put through indextest and compared through indexcmp
echo
echo "wikipedia depth 0 with output files put through indextest and compared through indexcmp"
./indexer ../data/wikipedia/0 ../data/wiki-0.txt
./indextest ../data/wiki-0.txt ../data/wiki-0-copy.txt
~/cs50-dev/shared/tse/indexcmp ../data/wiki-0.txt ../data/wiki-0-copy.txt

echo
echo "wikipedia depth 1 with output files put through indextest and compared through indexcmp"
./indexer ../data/wikipedia/1 ../data/wiki-1.txt
./indextest ../data/wiki-1.txt ../data/wiki-1-copy.txt
~/cs50-dev/shared/tse/indexcmp ../data/wiki-1.txt ../data/wiki-1-copy.txt

# valgrind checks on letters depth 10 and toscrape depth 1
echo
echo "valgrind: letters depth 10"
valgrind --leak-check=full --show-leak-kinds=all ./indexer ../data/letters/10 ../data/valgrind-letters.txt
valgrind --leak-check=full --show-leak-kinds=all ./indextest ../data/valgrind-letters.txt ../data/valgrind-letters-copy.txt

echo
echo "valgrind: toscrape depth 1"
valgrind --leak-check=full --show-leak-kinds=all ./indexer ../data/toscrape/1 ../data/valgrind-toscrape.txt
valgrind --leak-check=full --show-leak-kinds=all ./indextest ../data/valgrind-toscrape.txt ../data/valgrind-toscrape-copy.txt


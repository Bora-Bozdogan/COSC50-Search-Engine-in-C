# COSC50 Search Engine in C

A lightweight, modular search engine I built in C for the Dartmouth College course COSC50: Software Design and Implementation. 
It crawls web pages, indexes content, and returns ranked search results based on user queries.
The data structures used are also personally built in C for the same course.

## 📁 Project Structure

```
search-engine/
├── crawler/         # Crawls pages and stores them in the data/ directory
├── indexer/         # Builds an inverted index from crawled data
├── querier/         # Accepts user input and returns ranked results
├── libcs50/         # Shared helper functions and modules
├── data/            # (Ignored in Git) Contains fetched pages
├── README.md        # This file
└── .gitignore
```

## ⚙️ Features

- **Crawler**: Visits pages starting from a seed URL, saving them up to a given depth.
- **Indexer**: Builds a searchable inverted index mapping words to page IDs.
- **Querier**: Accepts multi-word AND queries and ranks relevant pages.
- **Thread-safe & Modular**: Uses a clean interface and minimal dependencies.

## 🧠 Concepts Covered

- Web crawling and HTTP
- Hash tables and data structures
- Memory management in C
- Modular design and testing
- File I/O and input sanitization

## 🚀 Getting Started

### 1. Build the project

```bash
make all
```

### 2. Run the crawler

```bash
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ./data 2
```

### 3. Build the index

```bash
./indexer ./data ./index.dat
```

### 4. Run the querier

```bash
./querier ./data ./index.dat
```

## 🧪 Testing

Unit tests and memory checks (Valgrind) are included in the Makefile:

```bash
make valgrind
```

## 📝 Notes

- All code written in C with no external dependencies.
- Designed for Unix-based systems.
- Data folder (`/data`) is excluded from version control.

## 📚 Acknowledgements

Developed by **Bora Bozdogan** as part of **COSC50: Software Design and Implementation** at Dartmouth College.

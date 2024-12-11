/*
*  WordHashTable.h
*  Drew Galen & Pablo Herrera
*  11/30/2024
*
*  CS 15 Proj 4: gerp
*
*  This file defines the WordHashTable class, which provides a hash table
*  implemenation to store and manage words and their associated info.
*  The class supports adding words, searching for words (both case-sensitive
*  and case-insensitive), and resiing the hash table dynamically to accomodate
*  an increasing number of entries.
*
*  The hash table is implmeneted using a multi-level vector strucuture,
*  the outer vector holds multiple inner vectors, which in turn hold a 
*  vector of 'WordEntry' objects. Each 'WordEntry' contains a word, the filename
*  it was found in, the line of the file, and the line number.
*
*/

#ifndef WORDHASHTABLE_H
#define WORDHASHTABLE_H

#include <string>
#include <iostream>
#include <vector>
#include <functional>

using namespace std;


/*
* WordEntry Struct
* Purpose: Represents an entry in the hash table that stores information related
*   to a specific word found in a file. Each entry contains the word itself,
*   the filename where it was found, the line of text containing the word
*   and the line number in the file where thw word was found.
*/
struct WordEntry {
    std::string word;
    std::string filename;
    std::string line;
    int lineNumber;
    std::string dir;
    
    WordEntry(const std::string& w, const std::string& f, const std::string& l, int lineNum, const std::string &d)
        : word(w), filename(f), line(l), lineNumber(lineNum), dir(d) {}
    
    // Overload the < operator to compare WordEntry objects
    bool operator<(const WordEntry& other) const {
        if (word != other.word)
            return word < other.word;
        if (filename != other.filename)
            return filename < other.filename;
        return lineNumber < other.lineNumber; // Only compare filename, word, and line number
    }
};


class WordHashTable {
public:
    // Consutructor
    WordHashTable();
    // Destructor
    ~WordHashTable();

    // Adds a word along with some of its info to the hash table
    void addWord(const string &word, const string &filename, const string &line, const int &lineNumber, const string &dir);
    
    // Searches for a word and returns a vector of all matching entries
    vector<WordEntry> searchWord(const string &word) const;
    vector<WordEntry> searchInsensitive(const string &word) const;

    // Ptint all the entries in the hash table (USE FOR DEBUGGING)
    void printTable() const;
    
private:
    // Initial hash sizes
    const int OUTER_SIZE = 100;
    const int INNER_SIZE = 0;
    int numEntries = 0;
    // std::vector<int> innerNumEntries;

    // Array of pointers to vectors of WordEntry objects
    vector<vector<vector<WordEntry>>> table;
    size_t tableSize;
    vector<size_t> insideTableSizes;

    // Computes the hash index for a given word
    size_t hashFunction(const string &word) const;

    // Computes hash index for a case sensitive word
    // size_t insideHashFunction(const string &word, int index) const;

    // Converts a string to lowercase
    string toLowercase(string str) const;

    // Rezising methods
    void resizeOuterTable();
    void resizeInnerTable(size_t primaryIndex);


};

#endif

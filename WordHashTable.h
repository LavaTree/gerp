#ifndef WORDHASHTABLE_H
#define WORDHASHTABLE_H

#include <string>
#include <iostream>
#include <vector>
#include <functional>

using namespace std;


// Struct to hold a word and the info associated with it
struct WordEntry {
    std::string word;
    std::string filename;
    std::string line;
    int lineNumber;
    
    WordEntry(const std::string& w, const std::string& f, const std::string& l, int lineNum)
        : word(w), filename(f), line(l), lineNumber(lineNum) {}
};

class WordHashTable {
public:

    WordHashTable(size_t outerSize = 100, size_t innerSize = 10);
    ~WordHashTable();

    // Adds a word along with some of its info to the hash table
    void addWord(const string &word, const string &filename, const string &line, const int &lineNumber);
    
    // Searches for a word and returns a vector of all matching entries
    vector<WordEntry> searchWord(const string &word) const;
    vector<WordEntry> searchInsensitive(const string &word) const;

    // Ptint all the entries in the hash table (USE FOR DEBUGGING)
    void printTable() const;
    
private:
    // Array of pointers to vectors of WordEntry objects
    vector<vector<vector<WordEntry>>> table;
    size_t tableSize;
    size_t insideTableSize;

    double numEntries;
    // Computes the hash index for a given word
    size_t hashFunction(const string &word) const;

    // Computes hash index for a case sensitive word
    size_t insideHashFunction(const string &word) const;

    // Converts a string to lowercase
    string toLowercase(string str) const;

    void resizeTable();

};

#endif


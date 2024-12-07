#include "WordHashTable.h"
#include <cstring>

using namespace std;

WordHashTable::WordHashTable(size_t size) : tableSize(size) {
    // dynamically allocate memory for the hash table
    table.resize(tableSize, vector<vector<WordEntry>>(insideTableSize));
}

WordHashTable::~WordHashTable() {
    // clean up mememory

}

size_t WordHashTable::hashFunction(const string &word) const {
    // create an instance of std::hash for strings
    hash<string> hasher;
    // return the hash value modulo number of buckets
    string iWord = toLowercase(word);
    return hasher(iWord) % tableSize;
}

size_t WordHashTable::insideHashFunction(const string &word) const {
    hash<string> hasher;
    return hasher(word) % insideTableSize;
}

void WordHashTable::addWord(const string &word, const string &filename,
                            const string &lineInfo){
    
    // gets the index if a given word
    size_t index = hashFunction(word);
    size_t insideIndex = insideHashFunction(word);

    // inserts the WordEntry into the appropiate bucket
    vector<WordEntry> &bucket = table[index][insideIndex];
    for (const WordEntry &entry : bucket) {
        if (entry.word == word) {
            // Avoid duplicates
            return;
        }
    }

    bucket.push_back(entry);
}



vector<WordEntry> WordHashTable::searchWord(const string &word) const {
    size_t index = hashFunction(word);
    size_t insideIndex = insideHashFunction(word);

    const vector<WordEntry> &bucket = table[index][insideIndex];
    vector<WordEntry> result;

    // Find exact matches
    for (const WordEntry &entry : bucket) {
        if (entry.word == word) {
            result.push_back(entry);
        }
    }

    return result;
}

vector<WordEntry> WordHashTable::searchInsensitive(const string &word) const {
    size_t index = hashFunction(word);
    const vector<vector<WordEntry>> &insideTable = table[index];

    vector<WordEntry> result;

    // Iterate through all inside buckets
    for (const vector<WordEntry> &bucket : insideTable) {
        for (const WordEntry &entry : bucket) {
            result.push_back(entry);
        }
    }

    return result;
}


void WordHashTable::printTable() const {
    for (size_t i = 0; i < tableSize; i++) {
        for (size_t j = 0; j < insideTableSize; j++) {
            for (const WordEntry &entry : table[i][j]) {
                cout << "Word: " << entry.word << " in file: " << entry.filename
                     << " on line: " << entry.line << endl;
            }
        }
    }
}

string WordHashTable::toLowercase(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' and str[i] <= 'Z') {
            str[i] += 32;
        }
    }
    return str;
}
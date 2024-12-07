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

void addWord(const string &word, const string &filename, const string &line, const int &lineNumber) {
    
    // gets the index if a given word
    size_t index = hashFunction(word);
    size_t insideIndex = insideHashFunction(word);

    WordEntry newEntry(word, filename, line, lineNumber);

    // Check if the same word is already present in the bucket for the same file and line
    for (const WordEntry &entry : bucket) {
        if (entry.word == word and entry.filename == filename and entry.line == line) {
            // Duplicate entry; do not add
            return;
        }
    }

    bucket.push_back(newEntry);
    numEntries++;

    // Check load factor and resize if necessary
    if (numEntries / (tableSize * insideTableSize) > 0.7) {
        resizeTable();
    }
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

void WordHashTable::resizeTable() {
    // Double the table size
    size_t newTableSize = tableSize * 2;

    // Create a new table with the new size
    vector<vector<vector<WordEntry>>> newTable(newTableSize, vector<vector<WordEntry>>(insideTableSize));

    // Rehash all entries into the new table
    for (size_t i = 0; i < tableSize; i++) {
        for (size_t j = 0; j < insideTableSize; j++) {
            for (const WordEntry &entry : table[i][j]) {
                // Compute new indices in the resized table
                size_t newPrimaryIndex = hash<string>()(toLowercase(entry.word)) % newTableSize;
                size_t newinsideIndex = hash<string>()(entry.word) % insideTableSize;

                // Add the entry to the appropriate bucket in the new table
                newTable[newPrimaryIndex][newinsideIndex].push_back(entry);
            }
        }
    }

    // Replace the old table with the new table and update the size
    table = std::move(newTable);
    tableSize = newTableSize;
}
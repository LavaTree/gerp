#include "WordHashTable.h"
#include <cstring>

using namespace std;

WordHashTable::WordHashTable() {
    // dynamically allocate memory for the hash table
    table.resize(OUTER_SIZE);

    for (auto &bucket : table) {
        bucket.resize(INNER_SIZE);
    }

    tableSize = OUTER_SIZE;

    insideTableSizes.resize(OUTER_SIZE, INNER_SIZE);


}

WordHashTable::~WordHashTable() {
    // clean up mememory

}

size_t WordHashTable::hashFunction(const string &word) const {
    hash<string> hasher;
    
    //Outside hash holds case insensitive word
    string iWord = toLowercase(word);

    return hasher(iWord) % tableSize;
}

void WordHashTable::addWord(const string &word, const string &filename, const string &line, const int &lineNumber) {
    // gets the index if a given word
    size_t index = hashFunction(word);


    vector<vector<WordEntry>> &bucket = table[index];
    vector<WordEntry> *innerBucket = nullptr;
    WordEntry newEntry(word, filename, line, lineNumber);


    // Check if the same word is already present in the bucket for the same file and line
    for (int i = 0; i < bucket.size(); i++) {  
        if (not bucket[i].empty() and word == bucket[i].front().word) {
            innerBucket = &bucket[i];
        }  
    }

    if (innerBucket == nullptr) {
        bucket.emplace_back();
        innerBucket = &bucket.back();
    }

    for (const WordEntry &entry : *innerBucket) {
        if (entry.filename == filename and entry.lineNumber == lineNumber) {
            return;
        }
    }

    innerBucket->push_back(newEntry);
    numEntries++;

    if ((double)numEntries / tableSize > 0.7) {
        resizeOuterTable();
    }

    if ((double)numEntries / insideTableSizes[index] > 0.7) {
        resizeInnerTable(index);
    }

}



vector<WordEntry> WordHashTable::searchWord(const string &word) const {
    size_t index = hashFunction(word);

    const vector<vector<WordEntry>> &bucket = table[index];
    vector<WordEntry> result;

    // Find exact matches
 
    for (const auto &innerBucket : bucket) {
        if (!innerBucket.empty() && innerBucket.front().word == word) {
            return innerBucket;
        }
    }

    return result;
}

vector<WordEntry> WordHashTable::searchInsensitive(const string &word) const {
    size_t index = hashFunction(word);
    const vector<vector<WordEntry>> &bucket = table[index];
    const vector<WordEntry> *innerBucket;

    vector<WordEntry> result;

    // Iterate through all word entries inside bucket
    for (int i = 0; i < bucket.size(); i++) {
        innerBucket = &bucket[i];
        for (const WordEntry &entry : bucket[i]) {
            result.push_back(entry);
        } 
    }

    return result;
}


void WordHashTable::printTable() const {
    for (size_t i = 0; i < tableSize; i++) {
        for (size_t j = 0; j < insideTableSizes[i]; j++) {
            for (const WordEntry &entry : table[i][j]) {
                cout << "Word: " << entry.word << " in file: " << entry.filename
                     << " on line: " << entry.line << endl;
            }
        }
    }
}

string WordHashTable::toLowercase(string str) const{
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' and str[i] <= 'Z') {
            str[i] += 32;
        }
    }
    return str;
}

void WordHashTable::resizeOuterTable() {
    size_t newTableSize = tableSize * 2;

    std::cerr << "Resizing outer table. Current size: " << tableSize
              << ", New size: " << newTableSize << std::endl;

    vector<vector<vector<WordEntry>>> newTable(newTableSize);

    // Rehash all entries into the new outer table
    for (size_t i = 0; i < tableSize; i++) {
        newTable[i].resize(insideTableSizes[i]);
        for (size_t j = 0; j < insideTableSizes[i]; j++) {
            for (const WordEntry &entry : table[i][j]) {
                size_t newPrimaryIndex = hash<string>()(toLowercase(entry.word)) % newTableSize;
                            std::cerr << "Rehashing word: " << entry.word
                          << " to new primary index: " << newPrimaryIndex << std::endl;
                newTable[newPrimaryIndex][j].push_back(entry);
            }
        }
    }

    // Replace the old table with the new table and update the size
    table = std::move(newTable);
    tableSize = newTableSize;
}

void WordHashTable::resizeInnerTable(size_t index) {
    size_t newInsideTableSize = insideTableSizes[index] * 2;

    table[index].resize(newInsideTableSize);
}


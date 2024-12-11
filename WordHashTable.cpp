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

    return hasher(iWord);
}

void WordHashTable::addWord(const string &word, const string &filename, const string &line, const int &lineNumber) {
    // gets the index if a given word
    size_t index = hashFunction(word) % tableSize;


    vector<vector<WordEntry>> &bucket = table[index];
    vector<WordEntry> *innerBucket = nullptr;
    WordEntry newEntry(word, filename, line, lineNumber);


    // Check if the same word is already present in the bucket for the same file and line
    for (int i = 0; i < bucket.size(); i++) {  
        if (not bucket[i].empty() and word == bucket[i].front().word) {
            innerBucket = &bucket[i];
            break;
        }  
    }

    if (innerBucket == nullptr) {
        bucket.push_back(vector<WordEntry>());
        innerBucket = &bucket.back();
        numEntries++;
    }

    for (const WordEntry &entry : *innerBucket) {
        if (entry.filename == filename and entry.lineNumber == lineNumber) {
            return;
        }
    }

    innerBucket->push_back(newEntry);

    if ((double)numEntries / tableSize > 0.7) {
        resizeOuterTable();
    }

    // if ((double)innerNumEntries[index] / insideTableSizes[index] > 0.7) {
    //     resizeInnerTable(index);
    // }

}



vector<WordEntry> WordHashTable::searchWord(const string &word) const { 
    size_t index = hashFunction(word) % tableSize;

    const vector<vector<WordEntry>> &bucket = table[index];
    vector<WordEntry> result;

    // Find exact matches
 
    for (const auto &innerBucket : bucket) {
        if (not innerBucket.empty() and innerBucket.front().word == word)
        if (not innerBucket.empty() and innerBucket.front().word == word) {
            return innerBucket;
        }
    }

    return result;
}

vector<WordEntry> WordHashTable::searchInsensitive(const string &word) const {
    size_t index = hashFunction(word) % tableSize;
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
        for (const auto &innerBucket : table[i]) {
            for (const WordEntry &entry : innerBucket) {
                cout << "(" << entry.word << ")" << entry.filename << "/" << entry.lineNumber << ": " <<  entry.line << endl;
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

    cerr << "Resizing outer table. Current size: " << tableSize
         << ", New size: " << newTableSize << endl;

    // Create a new table with the updated size
    std::vector<std::vector<std::vector<WordEntry>>> newTable(newTableSize);

    // Rehash all entries into the new outer table
    for (size_t i = 0; i < tableSize; i++) {
        for (const auto &innerBucket : table[i]) {
            for (const WordEntry &entry : innerBucket) {
                // Calculate the new index based on the updated table size
                size_t newPrimaryIndex = hashFunction(entry.word) % newTableSize;

                // Ensure the bucket exists before inserting
                if (newTable[newPrimaryIndex].empty()) {
                    newTable[newPrimaryIndex].resize(1); // Start with a single bucket
                }

                // Add the entry to the appropriate new bucket
                newTable[newPrimaryIndex][0].push_back(entry);
            }
        }
    }

    // Replace the old table with the new table and update the size
    table = std::move(newTable);
    tableSize = newTableSize;

    cerr << "Resizing complete. New table size: " << tableSize << endl;
}


void WordHashTable::resizeInnerTable(size_t index) {
    size_t newInsideTableSize = insideTableSizes[index] * 2;

    std::vector<std::vector<WordEntry>> newInnerTable(newInsideTableSize);


    for (size_t i = 0; i < table[index].size(); ++i) {
        for (const WordEntry &entry : table[index][i]) {
            newInnerTable[i].push_back(entry);
        }
    }

    table[index] = std::move(newInnerTable);
    insideTableSizes[index] = newInsideTableSize;
}


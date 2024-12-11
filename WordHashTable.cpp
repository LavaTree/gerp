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
    
    if ((double)numEntries / tableSize > 0.7) {
        resizeOuterTable();
    }

    size_t index = hashFunction(word) % tableSize;
    vector<vector<WordEntry>> *bucket = &table[index];
    vector<WordEntry> *innerBucket = nullptr;
    WordEntry newEntry(word, filename, line, lineNumber);

    // Check if the same word is already present in the bucket for the same file and line
    while (not bucket->empty()) {
        // cerr << "INDEX: " << index << endl;
        if (toLowercase(bucket->front().front().word) == toLowercase(word)) {
            for (int i = 0; i < bucket->size(); i++) {
                if (not bucket->front().empty() and word == bucket->front().front().word) {
                    // Debugging output to track what is happening with the bucket size
                    // std::cerr << "Word '" << word << "' already exists in bucket of size " << bucket->front().size() << std::endl;
                    
                    // Skip adding the word if it's already present in the current bucket
                    innerBucket = &bucket->front();
                    innerBucket->push_back(newEntry);
                    return;
                }
            }
        } 
        index = (index + 1) % tableSize;
        bucket = &table[index];
        
    }

    
    // Word not found, so add it
    // std::cerr << "Word '" << word << "' not found, adding to new bucket." << std::endl;
    // std::cerr << "Before adding, bucket size is " << bucket->size() << std::endl;
    bucket->push_back(vector<WordEntry>());
    innerBucket = &bucket->back();
    numEntries++;  // Update number of entries in the table

    
    innerBucket->push_back(newEntry);
    // std::cerr << "After adding, bucket size is " << bucket->size() << std::endl;
    

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

    // Rehash entries into the new outer table
    for (size_t i = 0; i < tableSize; i++) {
       if (not table[i].empty()) {
            WordEntry entry = table[i].front().front();
            size_t newIndex = hashFunction(entry.word) % newTableSize;
            // cerr << "Processing outer index: " << i
            //      << " with bucket size: " << table[i].size() << endl;

            if (newTable[newIndex].empty()) {
                // cerr << "Inserting bucket at new index: " << newIndex << endl;
                for (int j = 0; j < table[i].size(); j++) {
                    // cerr << "Copying inner bucket index: " << j << endl;
                    newTable[newIndex].push_back(table[i][j]);
                }
                
            } else {
                // cerr << "Collision detected at new index: " << newIndex
                //          << ", resolving with linear probing..." << endl;
                while (not newTable[newIndex].empty()) {
                    
                    newIndex = (newIndex + 1) % newTableSize;
                    
                }

                for (int j = 0; j < table[i].size(); j++) {
                    newTable[newIndex].push_back(table[i][j]);
                }
            }
       }
    }

    // Replace the old table with the new table and update the size
    table = std::move(newTable);
    tableSize = newTableSize;

    cerr << "Resizing complete. New table size: " << tableSize << endl;
}




void WordHashTable::resizeInnerTable(size_t index) {
    size_t size = insideTableSizes[index];
    size_t newSize = size * 2;

    // Create a new, larger inner table
    vector<vector<WordEntry>> newInnerTable(newSize);

    // Rehash entries from the old inner table into the new inner table
    for (const auto &bucket : table[index]) {
        for (const WordEntry &entry : bucket) {
            size_t newInnerIndex = hashFunction(entry.word) % newSize;
            newInnerTable[newInnerIndex].push_back(entry);
        }
    }

    // Replace the old inner table with the new one
    table[index] = std::move(newInnerTable);
    insideTableSizes[index] = newSize;
}


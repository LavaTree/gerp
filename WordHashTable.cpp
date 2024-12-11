/*
 *  gerp.cpp
 *  Drew Galen & Pablo Herrera
 *  11/30/2024
 *
 *  CS 15 Proj 4: gerp
 *
 *  This file contains the implementation of the WordHashTable class.
 *
 */
#include "WordHashTable.h"
#include <cstring>

using namespace std;

/*
* Name: WordHashTable()
* Purpose: this is the constructor for WordHashTable class. It initializezs the
*   the hash table by dynamically allocating memory for the outer and inner table. 
* Parameters: none
* Returns: none
*/
WordHashTable::WordHashTable() {
    // Dynamically allocate memory for the hash table
    table.resize(OUTER_SIZE);

    // Initialze each bucket in the outer table with default size
    for (auto &bucket : table) {
        bucket.resize(INNER_SIZE);
    }

    // Set initial sizes of both the outer and inner table
    tableSize = OUTER_SIZE;
    insideTableSizes.resize(OUTER_SIZE, INNER_SIZE);

}

/*
* Name: ~WordHashTable()
* Purpose: this is the destructor for WordHashTable. It cleans up memory.
* Parameters: none
* Return: none
* Notes: Nothing is explicitly needed since we are making use of STL containers
*   which manage memory
*/
WordHashTable::~WordHashTable() {
    // clean up mememory
    // Nothing explicitly due to STL containers

}

/*
* Name: hashFunction()
* Purpose: hash a word to produce a hashing that can be used for indexing
* Parameters:
*   - const string &word: the address of a string with the word to be hashed
* Return:
*   - size_t: the hash value generated for the word
* Notes: the word is converted to lowercase before being hashed. We are also
*   making use of std::hash<string>()
*/
size_t WordHashTable::hashFunction(const string &word) const {
    hash<string> hasher;
    
    // Conver the word to lowercase
    string iWord = toLowercase(word);

    return hasher(iWord);
}

/*
* Name: addWord()
* Purpose: add a WordEntry object to the inner table after checking if the
*   entry already exists. If the load factor of the table it over 0.7
*   the table will be resized. The function makes use of linear probing
*   to handle collisions when multiple words map to the same index.
* Paramteters:
*   - const string &word: a reference to the word to be added
*   - const string &filename: a reference to the filename where the word
*   was found
*   - const string &line: a reference to the line of text where the word
*   appears
*   - const int &lineNumber: a reference to the line number in the file
*   where the word was found
* Return: none
*/
void WordHashTable::addWord(const string &word, const string &filename, const string &line, const int &lineNumber, const string &dir) {
    // Resize table if we exceed load factor of 0.7
    if ((double)numEntries / tableSize > 0.7) {
        resizeOuterTable();
    }

    // Compute the index in the outer tabls
    size_t index = hashFunction(word) % tableSize;
    vector<vector<WordEntry>> *bucket = &table[index];
    vector<WordEntry> *innerBucket = nullptr;
    WordEntry newEntry(word, filename, line, lineNumber, dir);
    cout << "This is the new word: " << newEntry.word << endl;

    // Check if word is already in the bucket
    while (not bucket->empty()) {
        
        // Check if the word already exists in  the current bucket
        if (toLowercase(bucket->front().front().word) == toLowercase(word)) {
            for (int i = 0; i < bucket->size(); i++) {
                if (not bucket->front().empty() and word == bucket->front().front().word) {
                    
                    // If the word is table add new entry to the inner bucket
                    innerBucket = &bucket->front();
                    cout << "adding word: " << newEntry.word << endl;
                    innerBucket->push_back(newEntry);
                    return;
                }
            }
        } 

        // Linear probing
        index = (index + 1) % tableSize;
        bucket = &table[index];
        
    }

    
    // If word is not found create a bucket and add to table
    bucket->push_back(vector<WordEntry>());
    innerBucket = &bucket->back();
    cout << "Not found, new bucket for: " << newEntry.word << endl;

    numEntries++; 

    innerBucket->push_back(newEntry);

}


/*
* Name: searchWord()
* Purpose: search for a word in the hash table.
* Parameters:
*   - const string &word: a reference to the word to be searched in the table
* Return: a vector of 'WordEntry' objects with the results of the search
* Notes: This function performs a case-sensitive search for an exact word match
*   If no matches are found an empty vector is returned, indicating that the   
*   word is not in the hash table 
*/
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

/*
* Name: searchInsensitive()
* Purpose: search for a word in the hash tabl (case-insensitive)
* Parameters:
*   - const string &word: A reference to the word to be searched in the table
* Return: a vecotr of 'WordEntry' objects with the results of the search
* Notes: the function will perform a case-insensitive search for an exact word
*   match. If no math is found, an empty vectory is returned, indicating that
*   the word is not in the hash table
*/
vector<WordEntry> WordHashTable::searchInsensitive(const string &word) const {
    size_t index = hashFunction(word) % tableSize;
    const vector<vector<WordEntry>> &bucket = table[index];
    const vector<WordEntry> *innerBucket;

    vector<WordEntry> result;

    // Iterate through the bucket to find an exact match
    for (int i = 0; i < bucket.size(); i++) {
        innerBucket = &bucket[i];
        for (const WordEntry &entry : bucket[i]) {
            result.push_back(entry);
        } 
    }

    // Returns an empty result if no match is found
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

// void WordHashTable::printTable() const {
//     std::set<std::string> printedLines; // To track unique lines

//     for (size_t i = 0; i < tableSize; i++) {
//         for (const auto &innerBucket : table[i]) {
//             for (const WordEntry &entry : innerBucket) {
//                 // Create a unique identifier for the line (filename + line number)
//                 std::string uniqueLine = entry.filename + ":" + std::to_string(entry.lineNumber) + ": " + entry.line;

//                 if (printedLines.find(uniqueLine) == printedLines.end()) {
//                     // Line not printed yet, print it and add to the set
//                     printedLines.insert(uniqueLine);
//                     std::cout << uniqueLine << std::endl;
//                 }
//             }
//         }
//     }
// }


/*
* Name: toLowercase()
* Purpose: this function converts all uppercase letters in a string to 
*   lowercase using its ASCII value
* Parameters:
*   - string str: The input string to be converted to lowercase
* Return:
*   - string str: The string with all lowercase letters
*/
string WordHashTable::toLowercase(string str) const{
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' and str[i] <= 'Z') {
            str[i] += 32;
        }
    }
    return str;
}

/*
* Name: resizeOuterTable()
* Purpose: this functions resizes the outer hash table when the number of
*   entries exceeds a load factor of 0.7. The function will double the size
*   of the table, rehash the entries, and resolve collisions using linear
*   probing.
* Parameters: noe
* Return: none
* Notes: The function makes use of 'std::move' to efficiently transfer
*   into the new table
*/
void WordHashTable::resizeOuterTable() {
    size_t newTableSize = tableSize * 2;

    // cerr << "Resizing outer table. Current size: " << tableSize
    //      << ", New size: " << newTableSize << endl;

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

                // Handle collisions in new table with linear probing
                while (not newTable[newIndex].empty()) {
                    
                    newIndex = (newIndex + 1) % newTableSize;
                    
                }

                // Copy the buckets to the new table
                for (int j = 0; j < table[i].size(); j++) {
                    newTable[newIndex].push_back(table[i][j]);
                }
            }
       }
    }

    // Replace the old table with the new table and update the size
    table = std::move(newTable);
    tableSize = newTableSize;

    // cerr << "Resizing complete. New table size: " << tableSize << endl;
}

/*
* Name: resizeInnerTable()
* Purpose: this functions resizes the inner hash table when the number of
*   entries exceeds a load factor of 0.7. The function will double the size
*   of the table, rehash the entries, and resolve collisions using linear
*   probing.
* Parameters: noe
* Return: none
* Notes: The function makes use of 'std::move' to efficiently transfer
*   into the new table
*/
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

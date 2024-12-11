/*
 *  gerp.cpp
 *  Drew Galen & Pablo Herrera
 *  11/14/2024
 *
 *  CS 15 Proj 4: gerp
 *
 *  This file contains an implementation of the gerp class.
 *
 */

#include "gerp.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>

/*
* Name: run()
* Purpose: Open the specified output file, initialize the hash table of words
*   by processing the given directory, and start the query loop for user input
* Paramenters: 
*   - std::string dir: the name of the directory to process
*   - std::string filename: the name of the file to write output to
* Return: none
*/
void gerp::run(std::string dir, std::string filename) {
    outFile.open(filename);
    traverseDirectory(dir);
    query();
}

/*
* Name: query()
* Purpose: implements the query loop that handles user input. Processes
*   commands and queries for searching words, changing output files, and
*   terminating the program.
* Parameters: none
* Return: none
*/
void gerp::query() {
    std::string input, word;
    std::istringstream iss(input);
    bool first = true;

    while (true) {
        std::cout << "Query? ";
    
        if (not iss or iss.eof() or first) {
            std::getline(std::cin, input);
            iss.clear();
            iss.str(input);
            first = false;
        } 

        iss >> word;
        
        if (word == "@q") {
            quit();
            return;
        } else if (word == "@i") {
            iss >> word;
            iAnyString(word);  
        } else if (word == "@f") {
            iss >> word;
            newOutputFile(word);
        } else AnyString(word); 
    }
    quit();
}

/*
* Name: AnyString()
* Purpose: Processes a case-sensitive word query by searching the hash table
*   for matches. If a match is found it is outputted to the outputfile
* Parameters: 
*   - std::string &word: The address of the word to search for
* Return: none
*/
void gerp::AnyString(std::string &word) {
    std::string queryword = wordParse.toWord(word);

    // Search for the word in the hash table
    std::vector<WordEntry> results = wordTable.searchWord(queryword);

    if (results.empty()) {
        outFile << word <<" Not Found. Try with @insensitive or @i." << std::endl;
    } else {
        // Set to track seen entries using WordEntry objects
        std::set<WordEntry> seenEntries;

        // Process each entry in the results
        for (const auto &entry : results) {
            // Debug: print the entry to verify if the comparison works
            std::cout << "Processing: " << entry.word << " in " << entry.filename 
                      << " at line " << entry.lineNumber << std::endl;
            
            // If the entry has not been seen before, print it and add it to the set
            if (seenEntries.find(entry) == seenEntries.end()) {
                outFile << entry.dir << "/";
                outFile << entry.filename << ":" << entry.lineNumber << ": ";
                outFile << entry.line << std::endl;

                // Mark the entry as seen
                seenEntries.insert(entry);
            }
        }
    }
}


// /*
// * Name: iAnyString()
// * Purpose: Processes a case-insensitive word query by searching the hash table
// *   for matches. If a match is found it is outputted to the outputfile
// * Parameters:
// *   - std::string &word: the address of the word to search for
// * Return: none
// */
void gerp::iAnyString(std::string &word) {
    std::string queryWord = wordParse.toWord(word);

    // Search for the word in the hash table (case-insensitive)
    std::vector<WordEntry> results = wordTable.searchInsensitive(queryWord);

    if (results.empty()) {
        outFile << word << " Not Found." << std::endl;
    } else {
        // Set to track unique word entries (using WordEntry objects)
        std::set<WordEntry> seenEntries;

        // Process each entry in the results
        for (const auto &entry : results) {
            // Print the entry if it's not a duplicate (based on the full entry criteria)
            if (seenEntries.find(entry) == seenEntries.end()) {
                outFile << entry.dir << "/";
                outFile << entry.filename << ":" << entry.lineNumber << ": ";
                outFile << entry.line << std::endl;

                // Mark the entry as seen for future checks
                seenEntries.insert(entry);
            }
        }
    }
}


/*
* Name: quit()
* Purpose: quit the program and print goodby message to cout
* Paramters: none
* Return: none
*/
void gerp::quit() {
    std::cout << "Goodbye! Thank you and have a nice day." << std::endl;;
    return;
}

/*
* Name: newOutputFile()
* Purpose: closes the current outputfile and opens a new file for output
*   based on the provided file name.
* Parameters:
*   - std::string &filename: the address of a string with the name of the 
*   new output file to be openes
* Return: none
*/
void gerp::newOutputFile(std::string &file_name) {

    if (outFile.is_open()) {
        outFile.close();
    }

    outFile.open(file_name);
    //abort if cant open
}

/*
 * name:      traverseDirectory( )
 * purpose:   prints out the full paths of each file in the tree 
 * on separate lines
 * arguments: the string name of the root directory
 * returns:   nothing
 */
void gerp::traverseDirectory(string directory) {
    FSTree *tree = new FSTree(directory);
    recTraverseHelper("", tree->getRoot());
    delete tree;
    
}

/*
 * name:      recTraverseHelper( )
 * purpose:   a helper recursive function for traveseDirectory( )
 * arguments: the string name of the directory path and a pointer
 * to the current directory to traverse
 * returns:   nothing
 */
void gerp::recTraverseHelper(string dir, DirNode *curr) {
    
    //Not printing empty folders, only files
    if (curr->isEmpty()) return;

    dir += curr->getName();

    //For every sub directory keep checking for internal directories
    for (int i = 0; i <= curr->numSubDirs() - 1; i++) {
        DirNode *next = curr->getSubDir(i);
        recTraverseHelper(dir + "/", next);
    }

    //For every file process into hash
    for (int i = 0; i < curr->numFiles(); i++) {
        std::string filename = curr->getFile(i);
        processFile(filename, dir);
    }

}

/*
* name: processFile()
* purpose: processes a file, adding its words to the hash table
* arguments: the file path and a reference to WordHashTable
* returns: nothing
*/
void gerp::processFile(const string &filename, const string &dir) {
    ifstream fileStream(dir + "/" + filename);
    if(not fileStream.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return;
    }

    string line;
    int lineNumber = 0;

    while(getline(fileStream, line)) {
        lineNumber++;
        istringstream iss(line);
        string word;
        while(iss >> word) {
            string strippedWord = wordParse.toWord(word);
            if(not strippedWord.empty()) {
                wordTable.addWord(strippedWord, filename, line, lineNumber, dir);
            }
        }
    }
}
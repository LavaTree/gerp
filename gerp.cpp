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

parser wordParse;
WordHashTable wordTable;

//Run program
void gerp::run(std::string dir, std::ostream &outFile) {
    traverseDirectory(dir);
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

    //For every file simply print file name with directory location
    for (int i = 0; i <= curr->numFiles() - 1; i++) {
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
                wordTable.addWord(strippedWord, filename, "Line " + to_string(lineNumber));
            }
        }
    }
}
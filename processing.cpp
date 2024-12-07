/*
*  processing.cpp
*  Drew Galen & Pablo Herrera 
*  11/14/2024
*
*  CS 15 Proj 4: gerp
*
* This file implements the functions defined in processing.h
*
*/

#include "processing.h"
#include <string>
#include <iostream>


using namespace std;

/*
 * name:      stripNonAlphaNum( )
 * purpose:   strips all leading and trailing non-alphanumeric 
 * characters from a given string
 * arguments: the string to strip
 * returns:   the stripped string
 */
string stripNonAlphaNum(string input) {
    std::string result;

    //Start and end indexes of the substring word
    int start = 0;
    int end = input.size();

    while (start < input.size() and not isValidChar(input[start])) {
        start++;
    }

    if (start == end) return "";

    while (end > start and not isValidChar(input[end])) {
        end--;
    }

    int length = end - start + 1;

    return input.substr(start, length);
}

/*
 * name:      isValidChar( )
 * purpose:   finds if a character is a valid letter or number 
 * arguments: the char to check
 * returns:   a boolean true if valid false otherwise
 */
bool isValidChar(char c) {
    if (c >= 'A' and c <= 'Z') {
        return true;
    } else if (c >= 'a' and c <= 'z') {
        return true;
    } else if (c >= '0' and c <= '9') {
        return true;
    }
    return false;
}
    

/*
 * name:      traverseDirectory( )
 * purpose:   prints out the full paths of each file in the tree 
 * on separate lines
 * arguments: the string name of the root directory
 * returns:   nothing
 */
void traverseDirectory(string directory) {
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
void recTraverseHelper(string dir, DirNode *curr) {
    
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
        std::string name = curr->getFile(i);
        std::cout << dir << "/" << name << endl;
    }

}

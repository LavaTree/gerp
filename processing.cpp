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

    //Only append letters and numbers
    for (char c : input) {
        if (c >= 'A' and c <= 'Z') {
            result += c;
        } else if (c >= 'a' && c <= 'z') {
            result += c;
        } else if (c >= '0' and c <= '9') {
            result += c;
        }
    }

    return result;
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
    
    dir += curr->getName() + "/";

    //Not printing empty folders, only files
    if (curr->isEmpty()) return;

    //For every sub directory keep checking for internal directories
    for (int i = 1; i <= curr->numSubDirs(); i++) {
        DirNode *next = curr->getSubDir(i);
        recTraverseHelper(dir, next);
    }

    //For every file simply print file name with directory location
    for (int i = 1; i <= curr->numFiles(); i++) {
        std::string name = curr->getFile(i);
        std::cout << dir << "/" << name << endl;
    }

}
#ifndef __PROCESSING_H
#define __PROCESSING_H

/*
*  processing.h
*  Drew Galen & Pablo Herrera 
*  11/14/2024
*
*  CS 15 Proj 4: gerp
*
* This file defines three different functions that will be crucial
* for the implementation of the gerp program, which performrs efficient
* directory traversal and text processing. These functions include:
*
* stripNonAlphaNum()
* traverseDirectory()
* recTraverseHelper()
*
* These functions will support key operations within the gerp program,
* enabiling to navigate directories and process queries for further analysis
* and search purposes.
*/

#include <string>
#include "FSTree.h"
using namespace std;

string stripNonAlphaNum(string input);
bool isValidChar(char c);
void traverseDirectory(string directory);
void recTraverseHelper(string dir, DirNode *curr);

#endif 
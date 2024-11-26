#ifndef __PROCESSING_H
#define __PROCESSING_H

/*
*   TODO: Update file header if you use this file in your solution to phase 2 
*/

#include <string>
#include "FSTree.h"
using namespace std;

string stripNonAlphaNum(string input);
void traverseDirectory(string directory);
void recTraverseHelper(string dir, DirNode *curr);

#endif 
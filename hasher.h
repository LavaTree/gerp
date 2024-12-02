/*
*  hasher.h
*  Drew Galen & Pablo Herrera
*  11/25/2024
*
*  CS 15 Proj 4: hasher
*/

#ifndef _HASHER_H
#define _HASHER_H

#include <string>
#include <iostream>
#include <fstream>
#include <functional>

#include "FSTree.h"
#include "parser.h"

class hasher {
   public:

   private:
    struct entry {
        std::string line;
        std::string directory;
        int lineNum;
    };

    std::vector<std::string> key;
    std::vector<std::list<entry>> value;

      
    
};

#endif
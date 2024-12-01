/*
*  gerp.h
*  Drew Galen & Pablo Herrera
*  11/25/2024
*
*  CS 15 Proj 4: gerp
*
*  This file defines the gerp class, which will run the gerp program
*  that will allow for searching words within a given directory.
*
*  The class currently contains a public method to run that can be called
*  from a main class in order to initialize an instance of gerp. In other 
*  words, begin to run the gerp program. 
*/

#ifndef _GERP_H
#define _GERP_H

#include <string>
#include <iostream>
#include <fstream>
#include <functional>

#include "FSTree.h"
#include "parser.h"

class gerp {
   public:
        void run(std::string dir, std::ostream &outFile);

   private:
      struct entry {
         std::string line;
         std::string directory;
         int lineNum;
      };

      std::hash<std::string> hashFunction;
      std::vector<std::list<Entry>> table;

      parser wordParse;
   
      string stripNonAlphaNum(string input);
      bool isValidChar(char c);
      void traverseDirectory(string directory);
      void recTraverseHelper(string dir, DirNode *curr);

      void AnyString();
      void iAnyString();
      void newOutputFile(std::ostream &outFile);
      void quit();
    
};

#endif
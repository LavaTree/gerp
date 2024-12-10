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
#include "WordHashTable.h"

class gerp {
   public:
        void run(std::string dir, std::string filename);

   private:

      const string USAGE = "Usage: ./gerp inputDirectory outputFile";

      void abort(string error_message);

      parser wordParse;
      WordHashTable wordTable;
      std::ofstream outFile;

      void query();
      void AnyString(std::string &word);
      void iAnyString(std::string &word);
      void quit();
      void newOutputFile(std::string &file_name);
   
      void traverseDirectory(string directory);
      void recTraverseHelper(string dir, DirNode *curr);

      void processFile(const string &filename, const string &dir);
    
};

#endif
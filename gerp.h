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
#include <map>
#include <iostream>
#include <fstream>

class gerp {
   public:
        void run(std::string dir, std::ostream &outFile);

   private:
    
    
};

#endif
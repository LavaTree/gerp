/*
 *  main.cpp
 *  Drew Galen
 *  Pablo Herrera
 *  11/25/2024
 *
 *  CS 15 Proj 4: gerp
 *
 *  This file is the driver file for the gerp program.
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

#include "gerp.h"

using namespace std;

const string USAGE = "Usage: ./gerp inputDirectory outputFile";

void abort(string error_message);

template<typename streamtype>
void open_or_die(streamtype &stream, std::string file_name);

int main(int argc, char *argv[])
{
    if (argc != 3) abort(USAGE);

    std::string dir = argv[1]; 
    std::string oName = argv[2];
    std::ofstream oFile;

    open_or_die(oFile, oName);
    oFile.close();

    gerp program;

    program.run(dir, oName);

    return 0;
}

/*
 * name:      abort( )
 * purpose:   Prints an error to cerr stream and
 * exits program
 * arguments: a string error message to display
 * returns:   nothing
 * effects:   error thrown
 */
void abort(string error_message)
{
        cerr << error_message << endl;
        exit(EXIT_FAILURE);
}

/*
 * name:      open_or_die( )
 * purpose:   Attempts to open file, aborts program
 * if unable to open
 * arguments: a pointer to a stream that corresponds
 * to the file and the name of the file to attempt to
 * open
 * returns:   nothing
 * effects:   none
 */
template<typename streamtype>
void open_or_die(streamtype &stream, std::string file_name)
{
        stream.open(file_name);
        if (not stream.is_open())
                abort("Unable to open file " + file_name);
}

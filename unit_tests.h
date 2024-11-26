/*
 *  unit_tests.h
 *  Drew Galen, Pablo Herra
 *  11/25/2024
 *
 *  CS15 Proj 4: gerp
 *
 *  A simple testing framework to ensure all the functions in
 *  gerp work smoothly. 
 *
 */


#include <cassert>
#include <stdexcept>
#include <fstream>
#include <string>

#include "processing.h"

// Test the frequency file with simple text files
// Results print to cout
void frequency_simple() {
    std::string ref = "comp";

    std::string test1 = "comp";
    std::string test2 = "comp.";
    std::string test3 = "-comp";
    std::string test4 = "@#comp?@!";

    std::string stest = stripNonAlphaNum(test1);
    assert (stest == ref);

    stest = stripNonAlphaNum(test2);
    assert (stest == ref);

    stest = stripNonAlphaNum(test3);
    assert (stest == ref);

    stest = stripNonAlphaNum(test4);
    assert (stest == ref);

}
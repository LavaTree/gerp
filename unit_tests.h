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
#include <cstdlib>
#include <sstream>

#include "gerp.h"
#include "parser.h"

// Test the frequency file with simple text files
void frequency_simple() {
    std::string ref = "comp";

    std::string test1 = "comp";
    std::string test2 = "comp.";
    std::string test3 = "-comp";
    std::string test4 = "@#comp?@!";

    std::string stest = toWord(test1);
    assert (stest == ref);

    stest = toWord(test2);
    assert (stest == ref);

    stest = toWord(test3);
    assert (stest == ref);

    stest = toWord(test4);
    assert (stest == ref);

}

// Test the frequency file with advanced text files
void frequency_advanced() {
    std::string ref = "COMP-15";

    std::string test1 = "COMP-15";
    std::string test2 = "@COMP-15";
    std::string test3 = "$!@COMP-15@!";
    std::string test4 = "(*&^%$%^&*(*&^%^&COMP-15^)";

    std::string stest = toWord(test1);
    assert (stest == ref);

    stest = toWord(test2);
    assert (stest == ref);

    stest = toWord(test3);
    assert (stest == ref);

    stest = toWord(test4);
    assert (stest == ref);

}

// Tests that toWord() works on an empty string
void NonAlphaNum_empty() {
    std::string input = "";
    std::string result = toWord(input);
    assert(result == "");
}

// Tests that toWord() works on a string with no alphanumerical 
// characters
void NonAlphaNum_no_chars() {
    std::string input = "!@#$%^&*()";
    std::string result = toWord(input);
    assert(result == "");
}

// Tests that toWord() works on a string with only alphanumerical
// characters
void NonAlphaNum_onlyAphaNum() {
    std::string input = "abc123";
    std::string result = toWord(input);
    assert(result == "abc123");
}


// Tests that toWord() works with a string of only spaces
void NonAlphaNum_onlyspaces() {
    std::string input = "     ";
    std::string result = toWord(input);
    assert(result == "");
}

// Tests traverseDirectory() on a small directory
void test_traverseDirectory() {
    system("mkdir -p test_dir/sub_dir");
    system("touch test_dir/file1.txt");
    system("touch test_dir/sub_dir/file2.txt");

    std::ostringstream capturedOutput;
    std::streambuf *originalCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(capturedOutput.rdbuf());

    traverseDirectory("test_dir");
    std::cout.rdbuf(originalCoutBuffer);

    std::string output = capturedOutput.str();
    std::string expectedOutput =
        
        "test_dir/sub_dir/file2.txt\n"
        "test_dir/file1.txt\n";


    std::cout << "Actual Output:\n" << capturedOutput.str() << std::endl;
    std::cout << "Expected Output:\n" << expectedOutput << std::endl;
    assert(output == expectedOutput);


    system("rm -rf test_dir");
}
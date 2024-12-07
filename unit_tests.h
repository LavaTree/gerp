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

#include "processing.h"
#include "WordHashTable.h"



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

// Tests that stripNonAlphaNum() works on an empty string
void NonAlphaNum_empty() {
    std::string input = "";
    std::string result = stripNonAlphaNum(input);
    assert(result == "");
}

// Tests that stripNonAlphaNum() works on a string with no alphanumerical 
// characters
void NonAlphaNum_no_chars() {
    std::string input = "!@#$%^&*()";
    std::string result = stripNonAlphaNum(input);
    assert(result == "");
}

// Tests that stripNonAlphaNum() works on a string with only alphanumerical
// characters
void NonAlphaNum_onlyAphaNum() {
    std::string input = "abc123";
    std::string result = stripNonAlphaNum(input);
    assert(result == "abc123");
}

// Tests that stripNonAlphaNum() works on a string that includes spaces
void NonAlphaNum_withSpaces() {
    std::string input = "a b c 1 2 3";
    std::string result = stripNonAlphaNum(input);
    assert(result == "abc123");
}

// Tests that stripNonAlphaNum() works on a string with the newline char
void NonAlphaNum_newline() {
    std::string input = "abc\n123";
    std::string result = stripNonAlphaNum(input);
    assert(result == "abc123");
}

// Tests that stripNonAlphaNum() works with a string of only spaces
void NonAlphaNum_onlyspaces() {
    std::string input = "     ";
    std::string result = stripNonAlphaNum(input);
    assert(result == "");
}

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

void test_addWord() {
    WordHashTable wordTable;
    wordTable.addWord("hello", "file1.txt", "Line 1");

    wordTable.printTable();
    auto results = wordTable.searchWord("hello");
    assert(results.size() == 1);
}

void test_addWord_duplicate() {
    WordHashTable wordTable;
    wordTable.addWord("hello", "file1.txt", "Line 1");
    wordTable.addWord("hello", "file2.txt", "Line 2");

    wordTable.printTable();

    auto results = wordTable.searchWord("hello");
    assert(results.size() == 2);
}

void test_addWord_case_sesitivity() {
    WordHashTable wordTable;
    wordTable.addWord("Hello", "file1.txt", "Line 1");
    wordTable.addWord("hello", "file2.txt", "Line 2");

    wordTable.printTable();

    auto results1 = wordTable.searchWord("Hello");
    auto results2 = wordTable.searchWord("hello");
    assert(results1.size() == 1);
    assert(results2.size() == 1);
}
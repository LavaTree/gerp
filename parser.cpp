/*
 *  gerp.cpp
 *  Drew Galen & Pablo Herrera
 *  12/1/2024
 *
 *  CS 15 Proj 4: gerp
 *
 *  This file contains an implementation of the parser class.
 *
 */

#include "parser.h"

/*
 * name:      toWord( )
 * purpose:   strips all leading and trailing non-alphanumeric 
 * characters from a given string
 * arguments: the string to strip
 * returns:   the stripped string
 */
string parser::toWord(string input) {
    std::string result;

    //Start and end indexes of the substring word
    int start = 0;
    int end = input.size();

    while (start < input.size() and not isValidChar(input[start])) {
        start++;
    }

    if (start == end) return "";

    while (end > start and not isValidChar(input[end])) {
        end--;
    }

    int length = end - start + 1;

    return input.substr(start, length);
}

/*
 * name:      isValidChar( )
 * purpose:   finds if a character is a valid letter or number 
 * arguments: the char to check
 * returns:   a boolean true if valid false otherwise
 */
bool parser::isValidChar(char c) {
    if (c >= 'A' and c <= 'Z') {
        return true;
    } else if (c >= 'a' and c <= 'z') {
        return true;
    } else if (c >= '0' and c <= '9') {
        return true;
    }
    return false;
}
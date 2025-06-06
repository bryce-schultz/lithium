//**************************************************
// File: Error.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the Error class, representing
// errors with messages and source code ranges.
//
// Errors can be printed by calling the error
// functions below, but it is recommended to add
// a custom error macro in the source file to accomidate
// the type of error being reported. This also allows
// adding additional functionality such as to the error
// like increasing an error count.
//
// Example:
// #define error(msg, range)
//     rangeError(msg, range, __FILE__, __LINE__);
//     errorCount++
//
// errorCount would be defined in the source file
// with the error macro, and would be incremented
//**************************************************

#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include "Range.h"
#include "Token.h"
#include "Location.h"
#include "Color.h"

using std::string;
using std::cerr;
using std::stringstream;
using std::endl;

string getErrorLineSquiggles(const Range &range);
string getErrorLineLocationSquiggles(const Location &location, const Range &range);

void tokenError(const string &msg, const Token &token, const string &cppFile = "", int cppLine = 0);
void rangeError(const string &msg, const Range &range, const string &cppFile = "", int cppLine = 0);
void tokenRangeError(const string &msg, const Token &token, const Range &range, const string &cppFile = "", int cppLine = 0);
void locationRangeError(const string &msg, const Location &location, const Range &range, const string &cppFile = "", int cppLine = 0);
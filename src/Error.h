//**************************************************
// File: Error.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the Error class, representing
// errors with messages and source code ranges.
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

class Error
{
public:
    Error();
    Error(const string& message, const Range &range);

    string getMessage() const;
    Range getRange() const;
    string toString() const;
private:
    string message;
    Range range;
};

string getErrorLineSquiggles(const Range &range);
string getErrorLineTokenSquiggles(const Token &token, const Range &range);

void tokenError(const string &msg, const Token &token, const string &cppFile = "", int cppLine = 0);
void rangeError(const string &msg, const Range &range, const string &cppFile = "", int cppLine = 0);
void tokenRangeError(const string &msg, const Token &token, const Range &range, const string &cppFile = "", int cppLine = 0);
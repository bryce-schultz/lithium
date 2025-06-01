//**************************************************
// File: Error.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the Error class, which
// represents errors with messages and source ranges.
//**************************************************

#include "Error.h"

Error::Error():
    message(),
    range()
{ }

Error::Error(const string &message, const Range &range):
    message(message),
    range(range)
{
}

string Error::getMessage() const
{
    return message;
}

Range Error::getRange() const
{
    return range;
}

string Error::toString() const
{
    return "error: " + range.getStart().toString() + ": " + message;
}

string getErrorLineSquiggles(const Range &range)
{
    string line = range.getStart().getSourceLine();
    size_t start = range.getStart().getColumn() - 1; // Convert to 0-based index
    size_t end = range.getEnd().getColumn() - 1; // Convert to 0-based index

    if (start > line.length()) start = line.length() - 1;
    if (end > line.length()) end = line.length() - 1;

    if (start == end)
    {
        end = start + 1; // ensure at least one character is highlighted, for expected end tokens like ';'
    }

    string result = line + "\n";
    result += string(start, ' ');
    result += red + string(end - start, '~') + reset; // Add squiggles from start to end position
    result += "\n";
    result += string(start, ' ') + blodLightRed + "^" + reset; // Add caret at the start position

    return result;
}

string getErrorLineLocationSquiggles(const Location &location, const Range &range)
{
    string line = range.getStart().getSourceLine();
    size_t start = range.getStart().getColumn() - 1; // Convert to 0-based index
    size_t end = range.getEnd().getColumn() - 1; // Convert to 0-based index
    size_t tokenStart = location.getColumn() - 1; // Convert to 0-based index

    if (start > line.length()) start = line.length() - 1;
    if (end > line.length()) end = line.length() - 1;

    if (start == end)
    {
        end = start + 1; // ensure at least one character is highlighted, for expected end tokens like ';'
    }

    string result = line + "\n";
    result += string(start, ' ');
    result += red + string(end - start, '~') + reset; // Add squiggles from start to end position
    result += "\n";
    result += string(tokenStart, ' ') + blodLightRed + "^" + reset; // Add caret at the start position

    return result;
}

void tokenError(const string &msg, const Token &token, const string &cppFile, int cppLine)
{
    stringstream ss;
    ss << red << "error" << reset << ": " << token.getRange().getStart().toString() << ": " << msg << "\n"
    << getErrorLineSquiggles(token.getRange());

    if (!cppFile.empty() && cppLine > 0)
    {
        ss << "\n-> " << cppFile << ":" << cppLine;
    }

    cerr << ss.str() << endl;
}

void rangeError(const string &msg, const Range &range, const string &cppFile, int cppLine)
{
    stringstream ss;
    ss << red << "error" << reset << ": " << range.getStart().toString() << ": " << msg << "\n"
    << getErrorLineSquiggles(range);

    if (!cppFile.empty() && cppLine > 0)
    {
        ss << "\n-> " << cppFile << ":" << cppLine;
    }

    cerr << ss.str() << endl;
}

void tokenRangeError(const string &msg, const Token &token, const Range &range, const string &cppFile, int cppLine)
{
    stringstream ss;
    ss << red << "error" << reset << ": " << range.getStart().toString() << ": " << msg << "\n"
    << getErrorLineLocationSquiggles(token.getRange().getStart(), range);

    if (!cppFile.empty() && cppLine > 0)
    {
        ss << "\n-> " << cppFile << ":" << cppLine;
    }

    cerr << ss.str() << endl;
}

void locationRangeError(const string &msg, const Location &location, const Range &range, const string &cppFile, int cppLine)
{
    stringstream ss;
    ss << red << "error" << reset << ": " << location.toString() << ": " << msg << "\n"
    << getErrorLineLocationSquiggles(location, range);

    if (!cppFile.empty() && cppLine > 0)
    {
        ss << "\n-> " << cppFile << ":" << cppLine;
    }

    cerr << ss.str() << endl;
}

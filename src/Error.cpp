//**************************************************
// File: Error.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the Error class, which
// represents errors with messages and source ranges.
//**************************************************

#include <iostream>
#include <sstream>
#include <set>
#include <string>

#include "Error.h"
#include "Utils.h"

using std::set;

// used to prevent showing multiple errors at the same location
static set<Location> errorLocations;
static bool reportAllErrorsFlag = false;

#ifdef DEBUG
static bool includeCppFileAndLine = true;
#else
static bool includeCppFileAndLine = false;
#endif

#define bar "│ "
#define ball "○ "

void reportAllErrors(bool reportAll)
{
    reportAllErrorsFlag = reportAll;
}

void clearErrorLocations()
{
    errorLocations.clear();
}

string getErrorLineSquiggles(const Range &range)
{
    string line = Utils::ltrim(range.getStart().getSourceLine());
    size_t amountTrimmed = range.getStart().getSourceLine().length() - line.length();
    size_t start = range.getStart().getColumn() - 1; // Convert to 0-based index
    size_t end = range.getEnd().getColumn() - 1; // Convert to 0-based index

    start -= amountTrimmed; // Adjust start position based on trimmed length
    end -= amountTrimmed; // Adjust end position based on trimmed length

    size_t startLine = range.getStart().getLine() - 1; // Convert to 0-based index
    size_t endLine = range.getEnd().getLine() - 1; // Convert to 0-based index

    if (start > line.length()) start = line.length() - 1;
    if (end > line.length()) end = line.length() - 1;

    if (start > end && startLine == endLine)
    {
        std::swap(start, end);
    }
    else if (startLine < endLine)
    {
        // only print to the end of the line if the range spans multiple lines
        end = line.length();
    }

    if (start == end)
    {
        end = start + 1; // ensure at least one character is highlighted, for expected end tokens like ';'
    }

    stringstream result;
    result << bar << line << "\n" bar;
    result << string(start, ' ');
    result << red << string(end - start, '~') << reset; // Add squiggles from start to end position
    result << "\n" bar;
    result << string(start, ' ') << boldRed << "^" << reset; // Add caret at the start position

    return result.str();
}

string getErrorLineLocationSquiggles(const Location &location, const Range &range)
{
    string line = Utils::ltrim(range.getStart().getSourceLine());
    size_t amountTrimmed = range.getStart().getSourceLine().length() - line.length();

    size_t start = range.getStart().getColumn() - 1; // Convert to 0-based index
    size_t end = range.getEnd().getColumn() - 1; // Convert to 0-based index
    size_t tokenStart = location.getColumn() - 1; // Convert to 0-based index

    start -= amountTrimmed; // Adjust start position based on trimmed length
    end -= amountTrimmed; // Adjust end position based on trimmed length
    tokenStart -= amountTrimmed; // Adjust token start position based on trimmed length

    size_t startLine = range.getStart().getLine() - 1; // Convert to 0-based index
    size_t endLine = range.getEnd().getLine() - 1; // Convert to 0-based index

    if (start > line.length()) start = line.length() - 1;
    if (end > line.length()) end = line.length() - 1;

    if (start > end && startLine == endLine)
    {
        std::swap(start, end);
    }
    else if (startLine < endLine)
    {
        // only print to the end of the line if the range spans multiple lines
        end = line.length();
    }

    if (start == end)
    {
        end = start + 1; // ensure at least one character is highlighted, for expected end tokens like ';'
    }

    if (start == end)
    {
        end = start + 1; // ensure at least one character is highlighted, for expected end tokens like ';'
    }

    stringstream result;
    result << bar << line << "\n" bar;
    result << string(start, ' ');
    result << red << string(end - start, '~') << reset; // add squiggles from start to end position
    result << "\n" bar;
    result << string(tokenStart, ' ') << boldRed << "^" << reset; // add caret at the start position

    return result.str();
}

void generalError(const string &msg, const string &cppFile, int cppLine)
{
    stringstream ss;
    ss << red << "error" << reset << ": " << msg;

    if (includeCppFileAndLine && !cppFile.empty() && cppLine > 0)
    {
        ss << "\n" ball << cppFile << ":" << cppLine;
    }

    cerr << ss.str() << endl;
}

void tokenError(const string &msg, const Token &token, const string &cppFile, int cppLine)
{
    if (!reportAllErrorsFlag && errorLocations.find(token.getRange().getStart()) != errorLocations.end())
    {
        return; // skip if this location has already been reported
    }

    stringstream ss;
    ss << red << "error" << reset << ": " << token.getRange().getStart().toString() << ": " << msg << "\n"
    << getErrorLineSquiggles(token.getRange());

    if (includeCppFileAndLine && !cppFile.empty() && cppLine > 0)
    {
        ss <<  "\n" ball << cppFile << ":" << cppLine;
    }

    cerr << ss.str() << endl;

    errorLocations.insert(token.getRange().getStart()); // mark this location as reported
}

void rangeError(const string &msg, const Range &range, const string &cppFile, int cppLine)
{
    if (!reportAllErrorsFlag && errorLocations.find(range.getStart()) != errorLocations.end())
    {
        return; // skip if this location has already been reported
    }

    stringstream ss;
    ss << red << "error" << reset << ": " << range.getStart().toString() << ": " << msg << "\n"
    << getErrorLineSquiggles(range);

    if (includeCppFileAndLine && !cppFile.empty() && cppLine > 0)
    {
        ss << "\n" ball << cppFile << ":" << cppLine;
    }

    cerr << ss.str() << endl;

    errorLocations.insert(range.getStart()); // mark this location as reported
}

void tokenRangeError(const string &msg, const Token &token, const Range &range, const string &cppFile, int cppLine)
{
    if (!reportAllErrorsFlag && errorLocations.find(token.getRange().getStart()) != errorLocations.end())
    {
        return; // skip if this location has already been reported
    }

    if (!reportAllErrorsFlag && errorLocations.find(range.getStart()) != errorLocations.end())
    {
        return; // skip if this location has already been reported
    }

    stringstream ss;
    ss << red << "error" << reset << ": " << token.getRange().getStart().toString() << ": " << msg << "\n"
    << getErrorLineLocationSquiggles(token.getRange().getStart(), range);

    if (includeCppFileAndLine && !cppFile.empty() && cppLine > 0)
    {
        ss << "\n" ball << cppFile << ":" << cppLine;
    }

    cerr << ss.str() << endl;

    errorLocations.insert(token.getRange().getStart()); // mark this location as reported
    errorLocations.insert(range.getStart()); // also mark the start of the range as reported
}

void locationRangeError(const string &msg, const Location &location, const Range &range, const string &cppFile, int cppLine)
{
    if (!reportAllErrorsFlag && errorLocations.find(location) != errorLocations.end())
    {
        return; // skip if this location has already been reported
    }

    if (!reportAllErrorsFlag && errorLocations.find(range.getStart()) != errorLocations.end())
    {
        return; // skip if this location has already been reported
    }

    stringstream ss;
    ss << red << "error" << reset << ": " << location.toString() << ": " << msg << "\n"
    << getErrorLineLocationSquiggles(location, range);

    if (includeCppFileAndLine && !cppFile.empty() && cppLine > 0)
    {
        ss << "\n" ball << cppFile << ":" << cppLine;
    }

    cerr << ss.str() << endl;

    errorLocations.insert(location); // mark this location as reported
    errorLocations.insert(range.getStart()); // also mark the start of the range as reported
}

//**************************************************
// File: Location.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the Location class, which
// tracks positions in the source code for error reporting.
//**************************************************

#include <sstream>

#include "Location.h"

using std::stringstream;
using std::shared_ptr;
using std::string;
using std::to_string;
using std::pair;

Location::Location():
    pos(0),
    input(nullptr)
{ }

Location::Location(size_t pos, shared_ptr<string> input, shared_ptr<string> filename):
    pos(pos),
    input(input),
    filename(filename)
{ }


Location::Location(const Location &other):
    pos(other.pos),
    input(other.input),
    filename(other.filename)
{  }

Location &Location::operator=(const Location &other)
{
    if (this != &other)
    {
        pos = other.pos;
        input = other.input; // copy the shared_ptr
        filename = other.filename; // copy the filename if needed
    }
    return *this;
}

string Location::getSourceLine() const
{
    if (!input || input->empty())
    {
        static string emptyLine; // return an empty line if no input is available
        return emptyLine;
    }

    auto lineColumn = calculateLineAndColumn();
    size_t lineStart = 0;
    size_t currentLine = 1;

    for (size_t i = 0; i < input->length(); ++i)
    {
        if (input->at(i) == '\n')
        {
            if (currentLine == lineColumn.first)
            {
                return input->substr(lineStart, i - lineStart);
            }
            lineStart = i + 1; // move to the start of the next line
            currentLine++;
        }
    }

    // If we reach here, it means we are at the last line
    return input->substr(lineStart); // return the last line
}

size_t Location::getLine() const
{
    return calculateLineAndColumn().first;
}

size_t Location::getColumn() const
{
    return calculateLineAndColumn().second;
}

size_t Location::getPos() const
{
    return pos;
}

string Location::getFilename() const
{
    if (filename)
    {
        return *filename;
    }

    return "";
}

void Location::move(int offset)
{
    pos += offset;
}

bool Location::operator==(const Location &other) const
{
    return pos == other.pos;
}

bool Location::operator!=(const Location& other) const
{
    return !(*this == other);
}

bool Location::operator<(const Location& other) const
{
    if (pos < other.pos)
    {
        return true;
    }
    if (pos > other.pos)
    {
        return false;
    }

    return false;
}

bool Location::operator<=(const Location& other) const
{
    return *this < other || *this == other;
}

bool Location::operator>(const Location& other) const
{
    return !(*this <= other);
}

bool Location::operator>=(const Location& other) const
{
    return !(*this < other);
}

string Location::toString() const
{
    string result;
    if (filename)
    {
        result += *filename + ":";
    }

    auto lineColumn = calculateLineAndColumn();
    result += to_string(lineColumn.first) + ":" + to_string(lineColumn.second);

    return result;
}

pair<size_t, size_t> Location::calculateLineAndColumn() const
{
    if (!input || input->empty())
    {
        return {1, 1}; // default to line 1, column 1 if no input
    }

    size_t line = 1;
    size_t column = 1;

    for (size_t i = 0; i < pos && i < input->length(); ++i)
    {
        if ((*input)[i] == '\n')
        {
            line++;
            column = 1; // reset column on new line
        }
        else
        {
            column++;
        }
    }

    return {line, column};
}

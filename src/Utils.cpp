//**************************************************
// File: Utils.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements utility functions for reading files
// and getting input lines, including handling indentation
// and multi-line input in a user-friendly way.
//**************************************************

#include "Utils.h"

#include <fstream>
#include <string>
#include <stdexcept>
#include <iterator>
#include <iostream>

using std::ifstream;
using std::string;
using std::istreambuf_iterator;
using std::ios;
using std::cout;
using std::cin;
using std::getline;
using std::max;
using std::runtime_error;

string Utils::readWholeFile(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        throw runtime_error("could not open file: " + filename);
    }
    return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}

bool Utils::fileExists(const string &filename)
{
    ifstream file(filename);
    bool good = file.good();
    file.close();
    return good;
}

string Utils::getInputLine()
{
    string line, part;
    int indent = 0;

    while (true)
    {
        cout << (line.empty() ? "> " : "| ");
        cout.flush();
        if (!getline(cin, part))
        {
            cout << "\r\n";
            return "exit";
        }
        if (part == "exit")
            return "exit";
        if (part.empty())
            continue;

        // count opening and closing braces to track indentation
        for (char c : part)
        {
            if (c == '{') indent++;
            if (c == '}') indent = std::max(0, indent - 1);
        }

        line += part + "\n";

        // if not able to continue the line, break
        if (indent == 0 && part.back() != '{' && part.back() != '(' && part.back() != '[' && part.back() != ',')
            break;
    }

    // remove trailing newline if it exists
    /*if (!line.empty() && line.back() == '\n')
    {
        line.pop_back();
    }*/

    return line;
}

string Utils::join(const vector<string> &lines, const string &delimiter)
{
    if (lines.empty())
        return "";

    string result;
    for (const auto &line : lines)
    {
        if (!result.empty())
            result += delimiter;
        result += line;
    }
    return result;
}

string Utils::truncate(const string &str, size_t maxLength, const string &suffix)
{
    if (str.length() <= maxLength)
    {
        return str;
    }

    size_t suffixLength = suffix.length();
    if (maxLength <= suffixLength)
    {
        return suffix.substr(0, maxLength);
    }

    return str.substr(0, maxLength - suffixLength) + suffix;
}

string Utils::formatString(const string &format, const vector<string> &args)
{
    // loop through the format string and replace placeholders with arguments, placeholder will be %
    string result;
    size_t argIndex = 0;
    for (size_t i = 0; i < format.length(); ++i)
    {
        if (format[i] == '%' && argIndex < args.size())
        {
            result += args[argIndex++];
        }
        else
        {
            result += format[i];
        }
    }

    return result;
}

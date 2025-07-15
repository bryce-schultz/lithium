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
#include <algorithm>
#include <filesystem>
#include <vector>
#include <cstdlib>

#include <fcntl.h> // platform specific for file open modes

using std::ifstream;
using std::string;
using std::istreambuf_iterator;
using std::ios;
using std::cout;
using std::cin;
using std::endl;
using std::getline;
using std::max;
using std::runtime_error;
using std::vector;
using std::invalid_argument;
using std::make_shared;

namespace fs = std::filesystem;

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
            if (c == '}') indent = max(0, indent - 1);
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

string Utils::getModulePath(const string &moduleName, const string &basePath)
{
    //for debugging purposes, print the module name and base path and the current working directory
    // cout << "getModulePath called with moduleName: " << moduleName << ", basePath: " << basePath << "current working directory: " << getCurrentDirectory() << endl;

    string home = getHomeDirectory();
    if (moduleName.empty())
    {
        return string();
    }

    vector<string> searchPaths = {
        basePath.empty() ? "./" : basePath,
        "./modules/"
    };

    if (!home.empty())
    {
        searchPaths.push_back(home + "/modules/");
    }

    for (const auto &path : searchPaths)
    {
        string fullPath = path + moduleName + getLithiumFileExtension();
        if (fileExists(fullPath))
        {
            return fullPath;
        }
    }

    return "";
}

string Utils::getHomeDirectory()
{
    const char *home = getenv("HOME");
    if (home)
    {
        return string(home);
    }
    else
    {
        return "";
    }
}

string Utils::getLithiumFileExtension()
{
    return ".li";
}

string Utils::getCurrentDirectory()
{
    return fs::current_path().string();
}

mode_t Utils::parseOpenMode(const string &mode)
{
    if (mode == "r")
        return O_RDONLY;
    else if (mode == "w")
        return O_WRONLY | O_CREAT | O_TRUNC;
    else if (mode == "a")
        return O_WRONLY | O_CREAT | O_APPEND;
    else if (mode == "r+")
        return O_RDWR;
    else if (mode == "w+")
        return O_RDWR | O_CREAT | O_TRUNC;
    else if (mode == "a+")
        return O_RDWR | O_CREAT | O_APPEND;

    throw invalid_argument("Invalid file open mode: " + mode);
}

void Utils::printArgs(int argc, char **argv)
{
    cout << "Command line arguments: ";
    for (int i = 0; i < argc; ++i)
    {
        cout << argv[i];
        if (i < argc - 1)
        {
            cout << ", ";
        }
    }
    cout << endl;
}

string Utils::ltrim(const string &str)
{
    size_t start = str.find_first_not_of(" \t");
    if (start == string::npos) return ""; // string is all whitespace
    return str.substr(start);
}

void Utils::removePrefix(string &str, const string &prefix)
{
    if (str.find(prefix) == 0)
    {
        str.erase(0, prefix.length());
    }
}

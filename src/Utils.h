//**************************************************
// File: Utils.h
//
// Author: Bryce Schultz
//
// Purpose:  Declares utility functions for reading files
// and getting input lines, including handling indentation
// and multi-line input in a user-friendly way.
// This is useful for parsing and interpreting code in a
// more natural way, allowing users to write code that
// spans multiple lines without worrying about syntax errors
// due to line breaks or indentation issues.
//**************************************************
#pragma once

#include <string>
#include <vector>

// macros for unused parameters to avoid compiler warnings
#define UNUSED(x) (void)(x)

namespace Utils
{
    using std::string;
    using std::vector;

    string readWholeFile(const string &filename);

    bool fileExists(const string &filename);

    string getInputLine();

    string join(const vector<string> &lines, const string &delimiter = "\n");

    string truncate(const string &str, size_t maxLength, const string &suffix = "...");

    string formatString(const string &format, const vector<string> &args);

    string getModulePath(const string &moduleName, const string &basePath = "");

    string getHomeDirectory();

    string getLithiumFileExtension();

    string getCurrentDirectory();

    mode_t parseOpenMode(const string &mode);

    void printArgs(int argc, char **argv);

    void removePrefix(string &str, const string &prefix);
}
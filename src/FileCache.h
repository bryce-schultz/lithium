//**************************************************
// File: FileCache.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the FileCache class, which
// manages file streams and caching for source files.
//**************************************************

#pragma once
#include <unordered_map>
#include <string>
#include <istream>
#include <iostream>
#include <fstream>

using std::string;
using std::istream;
using std::ifstream;

typedef int FileId;

constexpr FileId INVALID_FILE_ID = -1;

struct FileRecord
{
    string filename;
    istream *stream;
};

class FileCache
{
public:
    FileId openFile(const string &filename);
    FileId openFile(istream &stream, const string &filename = "");

    void closeFile(FileId id);
    void closeAllFiles();

    FileRecord *getFileRecord(FileId id);
    FileRecord *getFileRecord(const string &filename);
private:
    std::unordered_map<FileId, FileRecord> records;
    static FileId nextId;
};

namespace global
{
    extern FileCache files;
}
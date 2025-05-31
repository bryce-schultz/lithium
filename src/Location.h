#pragma once

#include <cstdint>
#include <string>

#include "FileCache.h"

using std::string;
using std::pair;

class Location
{
public:
    Location();
    Location(int pos, FileId fileId = INVALID_FILE_ID);
    ~Location();

    Location(const Location& other);
    Location& operator=(const Location& other);
    Location(Location&& other) noexcept;
    Location& operator=(Location&& other) noexcept;

    int getLine() const;
    int getColumn() const;
    FileId getFileId() const;
    string getFilename() const;

    void move(int offset);
    void moveTo(int pos, FileId fileId = INVALID_FILE_ID);

    bool operator==(const Location& other) const;
    bool operator!=(const Location& other) const;
    bool operator<(const Location& other) const;
    bool operator<=(const Location& other) const;
    bool operator>(const Location& other) const;
    bool operator>=(const Location& other) const;

    string toString() const;
private:
    pair<int, int> calculateLineAndColumn() const;
private:
    int pos;
    FileId fileId;
};
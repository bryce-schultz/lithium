#pragma once

#include <cstdint>
#include <string>

#include "FileCache.h"

using std::string;

class Location
{
public:
    Location();
    Location(int line, int column, FileId fileId = INVALID_FILE_ID);
    ~Location();

    Location(const Location& other);
    Location& operator=(const Location& other);
    Location(Location&& other) noexcept;
    Location& operator=(Location&& other) noexcept;

    int getLine() const;
    int getColumn() const;
    FileId getFileId() const;
    string getFilename() const;

    void setLine(int line);
    void setColumn(int column);

    void incrementLine(int count = 1);
    void incrementColumn(int count = 1);
    void decrementLine(int count = 1);
    void decrementColumn(int count = 1);
    void resetLine(int line = 1);
    void resetColumn(int column = 1);

    bool operator==(const Location& other) const;
    bool operator!=(const Location& other) const;
    bool operator<(const Location& other) const;
    bool operator<=(const Location& other) const;
    bool operator>(const Location& other) const;
    bool operator>=(const Location& other) const;

    string toString() const;
private:
    int line;
    int column;
    FileId fileId;
};
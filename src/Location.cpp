#include "Location.h"

Location::Location():
    line(0), 
    column(0), 
    fileId(INVALID_FILE_ID) 
{ }

Location::Location(int line, int column, FileId fileId):
    line(line), 
    column(column), 
    fileId(fileId) 
{
}

Location::~Location()
{
    line = 0;
    column = 0;
    fileId = INVALID_FILE_ID;
}

Location::Location(const Location &other):
    line(other.line), 
    column(other.column), 
    fileId(other.fileId)
{ }

Location &Location::operator=(const Location &other)
{
    if (this != &other) 
    {
        line = other.line;
        column = other.column;
        fileId = other.fileId;
    }

    return *this;
}

Location::Location(Location &&other) noexcept
{
    line = other.line;
    column = other.column;
    fileId = other.fileId;
    
    other.line = 0;
    other.column = 0;
    other.fileId = INVALID_FILE_ID;
}

Location &Location::operator=(Location &&other) noexcept
{
    if (this != &other) 
    {
        line = other.line;
        column = other.column;
        fileId = other.fileId;
        
        other.line = 0;
        other.column = 0;
        other.fileId = INVALID_FILE_ID;
    }

    return *this;
}

int Location::getLine() const
{
    return line;
}

int Location::getColumn() const
{
    return column;
}

FileId Location::getFileId() const
{
    return fileId;
}

string Location::getFilename() const
{
    auto record = global::files.getFileRecord(fileId);
    if (!record) 
    {
        return "";
    }

    return record->filename;
}

void Location::setLine(int line)
{
    this->line = line;
}

void Location::setColumn(int column)
{
    this->column = column;
}

void Location::incrementLine(int count)
{
    line += count;
}

void Location::incrementColumn(int count)
{
    column += count;
}

void Location::decrementLine(int count)
{
    if (line > count) 
    {
        line -= count;
    } 
    else 
    {
        line = 1; // Prevent going below line 1
    }
}

void Location::decrementColumn(int count)
{
    if (column > count) 
    {
        column -= count;
    } 
    else 
    {
        column = 1; // Prevent going below column 1
    }
}

void Location::resetLine(int line)
{
    this->line = line > 0 ? line : 1; // Ensure line is at least 1
}

void Location::resetColumn(int column)
{
    this->column = column > 0 ? column : 1; // Ensure column is at least 1
}

bool Location::operator==(const Location& other) const
{
    return line == other.line &&
        column == other.column &&
        fileId == other.fileId;
}

bool Location::operator!=(const Location& other) const
{
    return !(*this == other);
}

bool Location::operator<(const Location& other) const
{
    if (fileId < other.fileId) return true;
    if (fileId > other.fileId) return false;
    if (line < other.line) return true;
    if (line > other.line) return false;
    return column < other.column;
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
    auto record = global::files.getFileRecord(fileId);
    if (!record)
    {
        return std::to_string(line) + ":" + std::to_string(column);
    }
    string filename = record->filename;

    return filename + ":" + std::to_string(line) + ":" + std::to_string(column);
}

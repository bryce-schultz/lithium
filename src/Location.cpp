#include "Location.h"

Location::Location():
    pos(0),
    fileId(INVALID_FILE_ID) 
{ }

Location::Location(int pos, FileId fileId):
    pos(pos),
    fileId(fileId) 
{
}

Location::~Location()
{
    pos = 0;
    fileId = INVALID_FILE_ID;
}

Location::Location(const Location &other):
    pos(other.pos), 
    fileId(other.fileId)
{ }

Location &Location::operator=(const Location &other)
{
    if (this != &other) 
    {
        pos = other.pos;
        fileId = other.fileId;
    }

    return *this;
}

Location::Location(Location &&other) noexcept
{
    pos = other.pos;
    fileId = other.fileId;
    
    pos = 0;
    other.fileId = INVALID_FILE_ID;
}

Location &Location::operator=(Location &&other) noexcept
{
    if (this != &other) 
    {
        pos = other.pos;
        fileId = other.fileId;
        
        other.pos = 0;
        other.fileId = INVALID_FILE_ID;
    }

    return *this;
}

int Location::getLine() const
{
    return calculateLineAndColumn().first;
}

int Location::getColumn() const
{
    return calculateLineAndColumn().second;
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

void Location::move(int offset)
{
    if (fileId == INVALID_FILE_ID) 
    {
        return; // cannot move if fileId is invalid
    }

    pos += offset;
    if (pos < 0) 
    {
        pos = 0; // prevent negative position
    }
}

void Location::moveTo(int pos, FileId fileId)
{
    if (fileId == INVALID_FILE_ID) 
    {
        fileId = this->fileId; // use current fileId if not provided
    }

    this->pos = pos;
    this->fileId = fileId;
}

bool Location::operator==(const Location &other) const
{
    return pos == other.pos &&
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
    if (pos < other.pos) return true;
    if (pos > other.pos) return false;

    return false; // equal positions
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
        return std::to_string(getLine()) + ":" + std::to_string(getColumn());
    }
    string filename = record->filename;

    return filename + ":" + std::to_string(getLine()) + ":" + std::to_string(getColumn());
}

pair<int, int> Location::calculateLineAndColumn() const
{
    auto record = global::files.getFileRecord(fileId);
    if (!record || !record->stream) 
    {
        return {1, 1}; // Default to line 1, column 1 if no file record or stream
    }

    istream *input = record->stream;
    // save the current position
    auto savePos = input->tellg();
    if (input->eof() || savePos == std::streampos(-1))
    {
        input->clear(); // Clear the eofbit flag
    }
    input->seekg(0, std::ios::beg); // Reset to the beginning of the file

    int lineCount = 1;
    int columnCount = 1;
    char c;
    while (input->get(c)) 
    {
        if (input->tellg() > pos)
        {
            break; // Stop when we reach the position
        }
        if (c == '\n') 
        {
            lineCount++;
            columnCount = 1; // Reset column count on new line
        } 
        else 
        {
            columnCount++;
        }
    }

    input->seekg(savePos); // Restore the original position
    return {lineCount, columnCount};
}

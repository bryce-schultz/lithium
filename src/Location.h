#pragma once

#include <cstdint>
#include <memory>
#include <string>

using std::string;
using std::pair;
using std::shared_ptr;
using std::make_shared;

class Location
{
public:
    Location();
    Location(size_t pos, shared_ptr<string> input, shared_ptr<string> filename = nullptr);

    Location(const Location& other);
    Location& operator=(const Location& other);

    string getSourceLine() const;

    size_t getLine() const;
    size_t getColumn() const;
    size_t getPos() const;
    string getFilename() const;

    void move(int offset);

    bool operator==(const Location& other) const;
    bool operator!=(const Location& other) const;
    bool operator<(const Location& other) const;
    bool operator<=(const Location& other) const;
    bool operator>(const Location& other) const;
    bool operator>=(const Location& other) const;

    string toString() const;
private:
    pair<size_t, size_t> calculateLineAndColumn() const;
private:
    size_t pos;
    shared_ptr<string> input;
    shared_ptr<string> filename;
};
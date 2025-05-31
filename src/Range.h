//**************************************************
// File: Range.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the Range class, representing
// a range of source code locations (start and end).
//**************************************************

#pragma once

#include "Location.h"

class Range
{
public:
    Range();
    Range(const Location& start, const Location& end);
    ~Range();

    Range(const Range& other);
    Range& operator=(const Range& other);
    Range(Range&& other) noexcept;
    Range& operator=(Range&& other) noexcept;
    Location getStart() const;
    Location getEnd() const;
    void setStart(const Location& start);
    void setEnd(const Location& end);
    bool operator==(const Range& other) const;
    bool operator!=(const Range& other) const;
    string toString() const;
private:
    Location start;
    Location end;
};
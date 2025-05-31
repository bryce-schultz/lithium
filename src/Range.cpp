//**************************************************
// File: Range.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the Range class, which
// represents a range of source code locations.
//**************************************************

#include "Range.h"

Range::Range()
{
}

Range::Range(const Location &start, const Location &end):
    start(start),
    end(end)
{
}

Range::~Range()
{
    // No dynamic memory to release, but we can clear the locations
    start = Location();
    end = Location();
}

Range::Range(const Range &other):
    start(other.start),
    end(other.end)
{ }

Range &Range::operator=(const Range &other)
{
    if (this != &other) {
        start = other.start;
        end = other.end;
    }
    return *this;
}

Range::Range(Range &&other) noexcept
{
    start = std::move(other.start);
    end = std::move(other.end);
    
    // Reset the moved-from object
    other.start = Location();
    other.end = Location();
}

Range &Range::operator=(Range &&other) noexcept
{
    if (this != &other) {
        start = std::move(other.start);
        end = std::move(other.end);
        
        // Reset the moved-from object
        other.start = Location();
        other.end = Location();
    }
    return *this;
}

Location Range::getStart() const
{
    return start;
}

Location Range::getEnd() const
{
    return end;
}

void Range::setStart(const Location &start)
{
    this->start = start;
}

void Range::setEnd(const Location &end)
{
    this->end = end;
}

bool Range::operator==(const Range &other) const
{
    return start == other.start && end == other.end;
}

bool Range::operator!=(const Range &other) const
{
    return !(*this == other);
}

string Range::toString() const
{
    return start.toString() + " - " + end.toString();
}
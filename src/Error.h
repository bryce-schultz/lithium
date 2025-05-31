#pragma once

#include <string>

#include "Range.h"

using std::string;

class Error
{
public:
    Error();
    Error(const string& message, const Range &range);

    string getMessage() const;
    Range getRange() const;
    string toString() const;
private:
    string message;
    Range range;
};
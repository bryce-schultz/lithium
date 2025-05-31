#include "Error.h"

Error::Error(): 
    message(), 
    range() 
{ }

Error::Error(const string &message, const Range &range):
    message(message), 
    range(range) 
{
}

string Error::getMessage() const
{
    return message;
}

Range Error::getRange() const
{
    return range;
}

string Error::toString() const
{
    return "error: " + range.getStart().toString() + ": " + message;
}
#pragma once

#include <memory>
#include <string>
#include <map>
#include <set>

#include "Value.h"
#include "Environment.h"
#include "Range.h"

using std::shared_ptr;
using std::make_shared;
using std::string;
using std::map;
using std::set;

class ErrorValue : public Value
{
public:
    ErrorValue(const string &message, const Range &range = {}):
        Value(Value::Type::null, range),
        message(message)
    { }

    inline string toString() const override { return "Error: " + message; }
    inline bool toBoolean() const override { return false; }
    inline string typeAsString() const override { return "error"; }

    const string &getValue() const { return message; }
private:
    string message;
};

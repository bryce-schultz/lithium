#include <string>
#include <vector>
#include <memory>
#include <regex>

#include "StringValue.h"
#include "Error.h"
#include "Values.h"
#include "Environment.h"
#include "Utils.h"

using std::shared_ptr;
using std::string;
using std::vector;
using std::make_shared;
using std::static_pointer_cast;
using std::regex;

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__)

#define errorAt(msg, location, range) \
    locationRangeError(msg, location, range, __FILE__, __LINE__)

StringValue::StringValue(char c, const Range &range):
    Value(Type::string, range), value(1, c)
{
    //std::cout << "StringValue constructor called with char: " << c << std::endl; // Debug output
    registerBuiltins();
}

StringValue::StringValue(const string &value, const Range &range):
    Value(Type::string, range), value(value)
{
    //std::cout << "StringValue constructor called with value: " << value << std::endl; // Debug output
    registerBuiltins();
}

StringValue::~StringValue()
{
    //std::cout << "StringValue destructor called for value: " << value << std::endl;
}

void StringValue::registerBuiltins()
{
    // length() -> number
    addMember("length", make_shared<BuiltinFunctionValue>(
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("length() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }
            return make_shared<NumberValue>(static_cast<double>(this->value.length()), range);
        },
        getRange()
    ), true);

    // empty() -> boolean
    addMember("empty", make_shared<BuiltinFunctionValue>(
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("empty() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }
            return make_shared<BooleanValue>(this->value.empty(), range);
        },
        getRange()
    ), true);

    // split(delimiter) -> []
    addMember("split", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            string delimiter = " "; // default delimiter is space
            if (args.size() > 1)
            {
                error("split() expects at most 1 argument, but got " + std::to_string(args.size()), range);
                return nullptr;
            }
            if (args.size() == 1)
            {
                if (args[0]->getType() != Value::Type::string)
                {
                    error("split() expects a string argument, but got " + args[0]->typeAsString(), range);
                    return nullptr;
                }

                delimiter = static_pointer_cast<StringValue>(args[0])->getValue();
            }
            vector<shared_ptr<Value>> parts;
            size_t pos = 0, found;
            while ((found = this->value.find(delimiter, pos)) != string::npos)
            {
                parts.push_back(make_shared<StringValue>(this->value.substr(pos, found - pos), range));
                pos = found + delimiter.length();
            }
            parts.push_back(make_shared<StringValue>(this->value.substr(pos), range));
            return make_shared<ArrayValue>(parts, range);
        },
        getRange()
    ), true);

    // lower() -> string
    addMember("lower", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("lower() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }

            string lowerValue = value;
            for (char &c : lowerValue)
            {
                c = static_cast<char>(tolower(c));
            }

            return make_shared<StringValue>(lowerValue, range);
        }
    ), true);

    // upper() -> string
    addMember("upper", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("upper() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }

            string upperValue = value;
            for (char &c : upperValue)
            {
                c = static_cast<char>(toupper(c));
            }

            return make_shared<StringValue>(upperValue, range);
        }
    ), true);

    // code() -> number or array
    addMember("code", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("code() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }

            if (value.empty())
            {
                // Return empty array for empty string
                return make_shared<ArrayValue>(vector<shared_ptr<Value>>(), range);
            }
            else if (value.length() == 1)
            {
                // Return single number for single character
                return make_shared<NumberValue>(static_cast<double>(static_cast<unsigned char>(value[0])), range);
            }
            else
            {
                // Return array of char codes for multi-character string
                vector<shared_ptr<Value>> charCodes;
                charCodes.reserve(value.length());
                for (char c : value)
                {
                    charCodes.emplace_back(make_shared<NumberValue>(static_cast<double>(static_cast<unsigned char>(c)), range));
                }
                return make_shared<ArrayValue>(charCodes, range);
            }
        }
    ), true);

    // find(substring) -> number
    addMember("find", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (args.size() != 1)
            {
                error("find() expects exactly 1 argument, got " + std::to_string(args.size()), range);
                return nullptr;
            }

            if (args[0]->getType() != Value::Type::string)
            {
                errorAt("find() expects a string argument, but got " + args[0]->typeAsString(), args[0]->getRange().getStart(), range);
                return nullptr;
            }

            const string &substring = static_pointer_cast<StringValue>(args[0])->getValue();
            size_t pos = value.find(substring);
            if (pos == string::npos)
            {
                return make_shared<NullValue>(range); // Not found
            }
            return make_shared<NumberValue>(static_cast<double>(pos), range);
        }
    ), true);

    // isNumeric() -> boolean
    addMember("isNumeric", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("isNumeric() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }

            // Check if the string can be converted to a number
            try
            {
                stod(value); // Try to convert to double
                return make_shared<BooleanValue>(true, range);
            }
            catch (const std::invalid_argument&)
            {
                return make_shared<BooleanValue>(false, range);
            }
        }
    ), true);

    // strip() -> string
    addMember("strip", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("strip() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }

            string strippedValue = value;
            // Remove leading and trailing whitespace
            strippedValue.erase(0, strippedValue.find_first_not_of(" \t\n\r\f\v"));
            strippedValue.erase(strippedValue.find_last_not_of(" \t\n\r\f\v") + 1);
            return make_shared<StringValue>(strippedValue, range);
        }
    ), true);

    // rstrip() -> string
    addMember("rstrip", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("rstrip() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }

            string rstrippedValue = value;
            // Remove trailing whitespace
            rstrippedValue.erase(rstrippedValue.find_last_not_of(" \t\n\r\f\v") + 1);
            return make_shared<StringValue>(rstrippedValue, range);
        }
    ), true);

    // lstrip() -> string
    addMember("lstrip", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("lstrip() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }

            string lstrippedValue = value;
            // Remove leading whitespace
            lstrippedValue.erase(0, lstrippedValue.find_first_not_of(" \t\n\r\f\v"));
            return make_shared<StringValue>(lstrippedValue, range);
        }
    ), true);

    // startsWith(prefix) -> boolean
    addMember("startsWith", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (args.size() != 1)
            {
                errorAt("startsWith() expects exactly 1 argument", range.getStart(), range);
                return nullptr;
            }

            if (args[0]->getType() != Value::Type::string)
            {
                errorAt("startsWith() expects a string argument, but got " + args[0]->typeAsString(), args[0]->getRange().getStart(), range);
                return nullptr;
            }

            const string &prefix = static_pointer_cast<StringValue>(args[0])->getValue();
            return make_shared<BooleanValue>(value.rfind(prefix, 0) == 0, range);
        }
    ), true);

    // endsWith(suffix) -> boolean
    addMember("endsWith", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (args.size() != 1)
            {
                errorAt("endsWith() expects exactly 1 argument", range.getStart(), range);
                return nullptr;
            }

            if (args[0]->getType() != Value::Type::string)
            {
                errorAt("endsWith() expects a string argument, but got " + args[0]->typeAsString(), args[0]->getRange().getStart(), range);
                return nullptr;
            }

            const string &suffix = static_pointer_cast<StringValue>(args[0])->getValue();
            return make_shared<BooleanValue>(value.length() >= suffix.length() && value.compare(value.length() - suffix.length(), suffix.length(), suffix) == 0, range);
        }
    ), true);

    // contains(substring) -> boolean
    addMember("contains", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (args.size() != 1)
            {
                errorAt("contains() expects exactly 1 argument", range.getStart(), range);
                return nullptr;
            }

            if (args[0]->getType() != Value::Type::string)
            {
                errorAt("contains() expects a string argument, but got " + args[0]->typeAsString(), args[0]->getRange().getStart(), range);
                return nullptr;
            }

            const string &substring = static_pointer_cast<StringValue>(args[0])->getValue();
            return make_shared<BooleanValue>(value.find(substring) != string::npos, range);
        }
    ), true);

    // match(regex) -> boolean
    addMember("match", make_shared<BuiltinFunctionValue>(
    [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (args.size() != 1)
            {
                errorAt("match() expects exactly 1 argument", range.getStart(), range);
                return nullptr;
            }

            if (args[0]->getType() != Value::Type::string)
            {
                errorAt("match() expects a string argument, but got " + args[0]->typeAsString(), args[0]->getRange().getStart(), range);
                return nullptr;
            }

            const string &pattern = static_pointer_cast<StringValue>(args[0])->getValue();
            regex regexPattern(pattern);
            return make_shared<BooleanValue>(regex_match(value, regexPattern), range);
        }
    ), true);
}

string StringValue::toString() const
{
    return value;
}

bool StringValue::toBoolean() const
{
    return !value.empty();
}

string StringValue::typeAsString() const
{
    return "string";
}

shared_ptr<Value> StringValue::add(const shared_ptr<NumberValue> &other) const
{
    return make_shared<StringValue>(value + other->toString(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::add(const shared_ptr<StringValue> &other) const
{
    return make_shared<StringValue>(value + other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::add(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<StringValue>(value + (other->getValue() ? "true" : "false"), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::add(const shared_ptr<NullValue> &other) const
{
    return make_shared<StringValue>(value + other->toString(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::add(const shared_ptr<ArrayValue> &other) const
{
    return make_shared<StringValue>(value + other->toString(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::add(const shared_ptr<ClassValue> &other) const
{
    return make_shared<StringValue>(value + other->toString(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::add(const shared_ptr<ObjectValue> &other) const
{
    return make_shared<StringValue>(value + other->toString(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::eq(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value == other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::eq(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::eq(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::eq(const shared_ptr<NullValue> &other) const
{
    return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::ne(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value != other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::ne(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(true, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::ne(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<BooleanValue>(true, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::ne(const shared_ptr<NullValue> &other) const
{
    return make_shared<BooleanValue>(true, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::lt(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value < other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::le(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value <= other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::gt(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value > other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> StringValue::ge(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value >= other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}
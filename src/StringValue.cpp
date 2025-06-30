#include <string>
#include <vector>

#include "StringValue.h"
#include "Error.h"
#include "Values.h"
#include "Environment.h"
#include "Utils.h"

using std::shared_ptr;
using std::string;
using std::vector;
using std::make_shared;

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__)

StringValue::StringValue(char c, Range range):
    Value(Type::string, range), // set the type to string
    value(1, c)
{
    constants.insert("length");
}

StringValue::StringValue(const string &value, Range range):
    Value(Type::string, range),
    value(value)
{
    constants.insert("length");

    addMember("split", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
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
                parts.push_back(std::make_shared<StringValue>(this->value.substr(pos, found - pos), range));
                pos = found + delimiter.length();
            }
            parts.push_back(std::make_shared<StringValue>(this->value.substr(pos), range));
            return make_shared<ArrayValue>(parts, range);
        },
        getRange()
    ), true);
}

const string &StringValue::getValue() const
{
    return value;
}

void StringValue::setValue(const string &value)
{
    this->value = value;
}

char StringValue::getCharAt(int index) const
{
    if (index < 0 || index >= static_cast<int>(value.length()))
    {
        return '\0'; // return null character for out-of-bounds index
    }
    return value[index];
}

size_t StringValue::length() const
{
    return value.length();
}

bool StringValue::isEmpty() const
{
    return value.empty();
}

string StringValue::toString() const
{
    return value;
}

bool StringValue::toBoolean() const
{
    return !value.empty();
}

shared_ptr<Value> StringValue::getMember(const string &name) const
{
    if (name == "length")
    {
        return make_shared<NumberValue>(static_cast<double>(value.length()), Range(getRange().getStart(), getRange().getEnd()));
    }
    return Value::getMember(name);
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
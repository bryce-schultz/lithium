#include "StringValue.h"

#include "Values.h"

StringValue::StringValue(const string &value, Range range):
    Value(Type::string, range), // set the type to string
    value(value)
{
    replaceEscapeSequences();
}

const string &StringValue::getValue() const
{
    return value;
}

void StringValue::setValue(const string &value)
{
    this->value = value;
}

string StringValue::toString() const
{
    return value;
}

bool StringValue::toBoolean() const
{
    return !value.empty();
}

shared_ptr<Value> StringValue::add(const shared_ptr<NumberValue> &other) const
{
    return make_shared<StringValue>(value + other->toString());
}

shared_ptr<Value> StringValue::add(const shared_ptr<StringValue> &other) const
{
    return make_shared<StringValue>(value + other->getValue());
}

shared_ptr<Value> StringValue::add(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<StringValue>(value + (other->getValue() ? "true" : "false"));
}

shared_ptr<Value> StringValue::add(const shared_ptr<NullValue> &other) const
{
    return make_shared<StringValue>(value + other->toString());
}

shared_ptr<Value> StringValue::eq(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value == other->getValue());
}

shared_ptr<Value> StringValue::ne(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value != other->getValue());
}

shared_ptr<Value> StringValue::lt(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value < other->getValue());
}

shared_ptr<Value> StringValue::le(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value <= other->getValue());
}

shared_ptr<Value> StringValue::gt(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value > other->getValue());
}

shared_ptr<Value> StringValue::ge(const shared_ptr<StringValue> &other) const
{
    return make_shared<BooleanValue>(value >= other->getValue());
}

void StringValue::replaceEscapeSequences()
{
    std::string result;
    result.reserve(value.size());
    for (size_t i = 0; i < value.size(); ++i)
    {
        if (value[i] == '\\' && i + 1 < value.size())
        {
            char next = value[i + 1];
            switch (next) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case 'r': result += '\r'; break;
                case '"': result += '"'; break;
                case '\'': result += '\''; break;
                case '\\': result += '\\'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'v': result += '\v'; break;
                default: result += next; break;
            }
            ++i; // skip the next character
        }
        else
        {
            result += value[i];
        }
    }
    this->value = result;
    type = Type::string;
}

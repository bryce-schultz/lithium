#include <cmath>

#include "NumberValue.h"
#include "Values.h"
#include "Error.h"

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__)

NumberValue::NumberValue(double value, Range range):
    Value(Type::number, range),
    value(value)
{ }

double NumberValue::getValue() const
{
    return value;
}

void NumberValue::setValue(double value)
{
    this->value = value;
}

bool NumberValue::isInteger() const
{
    return value == static_cast<int>(value);
}

string NumberValue::toString() const
{

    string str = to_string(value);
    str.erase(str.find_last_not_of('0') + 1, string::npos);
    str.erase(str.find_last_not_of('.') + 1, string::npos);
    if (str.empty())
    {
        str = "0";
    }
    return str;
}

bool NumberValue::toBoolean() const
{
    return value != 0.0;
}

shared_ptr<Value> NumberValue::add(const shared_ptr<NumberValue> &other) const
{
    return make_shared<NumberValue>(value + other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::add(const shared_ptr<StringValue> &other) const
{
    return make_shared<StringValue>(toString() + other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::sub(const shared_ptr<NumberValue> &other) const
{
    return make_shared<NumberValue>(value - other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::mul(const shared_ptr<NumberValue> &other) const
{
    return make_shared<NumberValue>(value * other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::div(const shared_ptr<NumberValue> &other) const
{
    if (other->getValue() == 0.0)
    {
        error("cannot divide by zero", other->getRange());
        return make_shared<NullValue>(Range(getRange().getStart(), other->getRange().getEnd()));
    }
    return make_shared<NumberValue>(value / other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::mod(const shared_ptr<NumberValue> &other) const
{
    if (other->getValue() == 0.0)
    {
        error("cannot divide by zero", other->getRange());
        return make_shared<NullValue>(Range(getRange().getStart(), other->getRange().getEnd()));
    }
    return make_shared<NumberValue>(fmod(value, other->getValue()), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::eq(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value == other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::eq(const shared_ptr<NullValue> &other) const
{
    return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::ne(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value != other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::ne(const shared_ptr<NullValue> &other) const
{
    return make_shared<BooleanValue>(true, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::lt(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value < other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::le(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value <= other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::gt(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value > other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::ge(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value >= other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::logicalAnd(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(toBoolean() && other->toBoolean(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::logicalOr(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(toBoolean() || other->toBoolean(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::unaryMinus() const
{
    return make_shared<NumberValue>(-value, getRange());
}
#include <cmath>

#include "NumberValue.h"
#include "Values.h"
#include "Error.h"
#include "Exceptions.h"
#include "Utils.h"

// Epsilon for floating-point comparison - handles precision issues like 0.1 + 0.2 == 0.3
constexpr double EPSILON = 1e-15;

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__); \
    throw ErrorException(msg, range)

NumberValue::NumberValue(int value, const Range &range):
    Value(Type::number, range), value(value)
{
}

NumberValue::NumberValue(unsigned long value, const Range &range):
    Value(Type::number, range), value(static_cast<double>(value))
{
}

NumberValue::NumberValue(double value, const Range &range):
    Value(Type::number, range), value(value)
{
}

string NumberValue::toString() const
{
    // Round to 15 decimal places to eliminate floating-point precision artifacts
    double rounded = std::round(value * 1e15) / 1e15;
    
    string str = to_string(rounded);
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

string NumberValue::typeAsString() const
{
    return "number";
}

shared_ptr<Value> NumberValue::add(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
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
        return make_shared<NullValue>(Range::getEmpty()); // This won't be reached due to exception
    }
    return make_shared<NumberValue>(value / other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::mod(const shared_ptr<NumberValue> &other) const
{
    if (other->getValue() == 0.0)
    {
        error("cannot divide by zero", other->getRange());
        return make_shared<NullValue>(Range::getEmpty()); // This won't be reached due to exception
    }
    return make_shared<NumberValue>(fmod(value, other->getValue()), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::eq(const shared_ptr<NumberValue> &other) const
{
    // Use epsilon comparison for floating-point numbers to handle precision issues
    double diff = std::abs(value - other->getValue());
    bool isEqual = diff < EPSILON;
    return make_shared<BooleanValue>(isEqual, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::eq(const shared_ptr<NullValue> &other) const
{
    UNUSED(other);
    return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::ne(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value != other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> NumberValue::ne(const shared_ptr<NullValue> &other) const
{
    UNUSED(other);
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

shared_ptr<Value> NumberValue::unaryNot() const
{
    return make_shared<BooleanValue>(!toBoolean(), getRange());
}
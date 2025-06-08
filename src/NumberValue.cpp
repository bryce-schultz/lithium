#include <cmath>

#include "NumberValue.h"
#include "Values.h"

NumberValue::NumberValue(double value):
    value(value)
{
    type = Type::number;
}

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
    if (isInteger())
    {
        return std::to_string(static_cast<int>(value));
    }
    return std::to_string(value);
}

bool NumberValue::toBoolean() const
{
    return value != 0.0;
}

shared_ptr<Value> NumberValue::add(const shared_ptr<NumberValue> &other) const
{
    return make_shared<NumberValue>(value + other->getValue());
}

shared_ptr<Value> NumberValue::add(const shared_ptr<StringValue> &other) const
{
    return make_shared<StringValue>(toString() + other->getValue());
}

shared_ptr<Value> NumberValue::sub(const shared_ptr<NumberValue> &other) const
{
    return make_shared<NumberValue>(value - other->getValue());
}

shared_ptr<Value> NumberValue::mul(const shared_ptr<NumberValue> &other) const
{
    return make_shared<NumberValue>(value * other->getValue());
}

shared_ptr<Value> NumberValue::div(const shared_ptr<NumberValue> &other) const
{
    if (other->getValue() == 0.0)
    {
        return nullptr;
    }
    return make_shared<NumberValue>(value / other->getValue());
}

shared_ptr<Value> NumberValue::mod(const shared_ptr<NumberValue> &other) const
{
    if (other->getValue() == 0.0)
    {
        return nullptr; // Handle modulo by zero error
    }
    return make_shared<NumberValue>(fmod(value, other->getValue()));
}

shared_ptr<Value> NumberValue::eq(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value == other->getValue());
}

shared_ptr<Value> NumberValue::ne(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value != other->getValue());
}

shared_ptr<Value> NumberValue::lt(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value < other->getValue());
}

shared_ptr<Value> NumberValue::le(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value <= other->getValue());
}

shared_ptr<Value> NumberValue::gt(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value > other->getValue());
}

shared_ptr<Value> NumberValue::ge(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value >= other->getValue());
}

shared_ptr<Value> NumberValue::logicalAnd(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(toBoolean() && other->toBoolean());
}

shared_ptr<Value> NumberValue::logicalOr(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(toBoolean() || other->toBoolean());
}
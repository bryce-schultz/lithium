#include "BooleanValue.h"

#include "Values.h"

BooleanValue::BooleanValue(bool value, const Range &range):
    Value(Type::boolean, range), // set the type to boolean
    value(value)
{ }

bool BooleanValue::getValue() const
{
    return value;
}

void BooleanValue::setValue(bool value)
{
    this->value = value;
}

string BooleanValue::toString() const
{
    return value ? "true" : "false";
}

bool BooleanValue::toBoolean() const
{
    return value;
}

string BooleanValue::typeAsString() const
{
    return "boolean";
}

shared_ptr<Value> BooleanValue::eq(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<BooleanValue>(value == other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::eq(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value == (other->getValue() != 0), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::eq(const shared_ptr<NullValue> &other) const
{
    return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::ne(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<BooleanValue>(value != other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::ne(const shared_ptr<NumberValue> &other) const
{
    return make_shared<BooleanValue>(value != (other->getValue() != 0), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::ne(const shared_ptr<NullValue> &other) const
{
    return make_shared<BooleanValue>(true, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::lt(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<BooleanValue>(value < other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::le(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<BooleanValue>(value <= other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::gt(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<BooleanValue>(value > other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::ge(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<BooleanValue>(value >= other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::logicalAnd(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<BooleanValue>(value && other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::logicalOr(const shared_ptr<BooleanValue> &other) const
{
    return make_shared<BooleanValue>(value || other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> BooleanValue::unaryNot() const
{
    return make_shared<BooleanValue>(!value, getRange());
}

shared_ptr<Value> BooleanValue::add(const shared_ptr<StringValue> &other) const
{
    return make_shared<StringValue>(toString() + other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}
#include "NullValue.h"

#include "Values.h"

NullValue::NullValue(Range range):
    Value(Type::null, range) // set the type to null
{ }

string NullValue::toString() const
{
    return "null";
}

shared_ptr<Value> NullValue::add(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<StringValue>(toString() + other->getValue());
}

shared_ptr<Value> NullValue::eq(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(true);
}

shared_ptr<Value> NullValue::ne(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(false);
}

shared_ptr<Value> NullValue::eq(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(false);
}

shared_ptr<Value> NullValue::ne(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(true);
}

shared_ptr<Value> NullValue::eq(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(false);
}

shared_ptr<Value> NullValue::ne(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(true);
}

shared_ptr<Value> NullValue::eq(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(false);
}

shared_ptr<Value> NullValue::ne(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(true);
}

shared_ptr<Value> NullValue::eq(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(false);
}

shared_ptr<Value> NullValue::ne(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(true);
}
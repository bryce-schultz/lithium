#include "Values.h"
#include "Utils.h"
#include "ExpressionNode.h"
#include "Value.h"

string Value::typeAsString() const
{
    switch (type)
    {
        case Type::null: return "null";
        case Type::number: return "number";
        case Type::string: return "string";
        case Type::boolean: return "boolean";
        case Type::function: return "function";
        case Type::builtin: return "builtin";
        default: return "unknown";
    }
}

Value::Value(Type type, Range range):
    type(type),
    range(range)
{ }

Value::Type Value::getType() const
{
    return type;
}

Range Value::getRange() const
{
    return range;
}

bool Value::toBoolean() const
{
    return type != Type::null;
}

shared_ptr<Value> Value::add(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return add(static_pointer_cast<NullValue>(other));
        case Type::number:
            return add(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return add(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return add(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return add(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for addition
    }
}

shared_ptr<Value> Value::sub(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return sub(static_pointer_cast<NullValue>(other));
        case Type::number:
            return sub(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return sub(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return sub(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return sub(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for subtraction
    }
}

shared_ptr<Value> Value::mul(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return mul(static_pointer_cast<NullValue>(other));
        case Type::number:
            return mul(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return mul(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return mul(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return mul(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for multiplication
    }
}

shared_ptr<Value> Value::div(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return div(static_pointer_cast<NullValue>(other));
        case Type::number:
            return div(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return div(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return div(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return div(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for division
    }
}

shared_ptr<Value> Value::mod(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return mod(static_pointer_cast<NullValue>(other));
        case Type::number:
            return mod(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return mod(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return mod(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return mod(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for modulo
    }
}

shared_ptr<Value> Value::eq(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return eq(static_pointer_cast<NullValue>(other));
        case Type::number:
            return eq(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return eq(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return eq(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return eq(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for equality check
    }
}

shared_ptr<Value> Value::ne(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return ne(static_pointer_cast<NullValue>(other));
        case Type::number:
            return ne(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return ne(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return ne(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return ne(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for inequality check
    }
}

shared_ptr<Value> Value::lt(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return lt(static_pointer_cast<NullValue>(other));
        case Type::number:
            return lt(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return lt(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return lt(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return lt(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for less than check
    }
}

shared_ptr<Value> Value::le(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return le(static_pointer_cast<NullValue>(other));
        case Type::number:
            return le(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return le(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return le(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return le(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for less than or equal check
    }
}

shared_ptr<Value> Value::gt(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return gt(static_pointer_cast<NullValue>(other));
        case Type::number:
            return gt(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return gt(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return gt(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return gt(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for greater than check
    }
}

shared_ptr<Value> Value::ge(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return ge(static_pointer_cast<NullValue>(other));
        case Type::number:
            return ge(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return ge(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return ge(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return ge(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for greater than or equal check
    }
}

shared_ptr<Value> Value::logicalAnd(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return logicalAnd(static_pointer_cast<NullValue>(other));
        case Type::number:
            return logicalAnd(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return logicalAnd(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return logicalAnd(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return logicalAnd(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for logical AND
    }
}

shared_ptr<Value> Value::logicalOr(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return logicalOr(static_pointer_cast<NullValue>(other));
        case Type::number:
            return logicalOr(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return logicalOr(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return logicalOr(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return logicalOr(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for logical OR
    }
}

shared_ptr<Value> Value::comma(const shared_ptr<Value> &other) const
{
    if (!other) return nullptr;

    switch (other->getType())
    {
        case Type::null:
            return comma(static_pointer_cast<NullValue>(other));
        case Type::number:
            return comma(static_pointer_cast<NumberValue>(other));
        case Type::string:
            return comma(static_pointer_cast<StringValue>(other));
        case Type::boolean:
            return comma(static_pointer_cast<BooleanValue>(other));
        case Type::function:
            return comma(static_pointer_cast<FunctionValue>(other));
        default:
            return nullptr; // Unsupported type for comma operation
    }
}

shared_ptr<Value> Value::add(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::add(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::add(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::add(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::add(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::add(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::add(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::sub(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::sub(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::sub(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::sub(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::sub(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::sub(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::sub(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mul(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mul(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mul(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mul(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mul(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mul(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mul(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::div(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::div(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::div(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::div(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::div(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::div(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::div(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mod(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}
shared_ptr<Value> Value::mod(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mod(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mod(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mod(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mod(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::mod(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::eq(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::eq(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::eq(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::eq(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::eq(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::eq(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::eq(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ne(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ne(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ne(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ne(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ne(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ne(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ne(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::lt(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::lt(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::lt(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::lt(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::lt(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::lt(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::lt(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::le(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::le(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::le(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::le(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::le(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::le(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::le(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::gt(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::gt(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::gt(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::gt(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::gt(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::gt(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::gt(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ge(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ge(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ge(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ge(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ge(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ge(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ge(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalAnd(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalAnd(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalAnd(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalAnd(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalAnd(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalAnd(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalAnd(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalOr(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalOr(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalOr(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalOr(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalOr(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalOr(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::logicalOr(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::comma(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::comma(const shared_ptr<NumberValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::comma(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::comma(const shared_ptr<BooleanValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::comma(const shared_ptr<FunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::comma(const shared_ptr<BuiltinFunctionValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::comma(const shared_ptr<ArrayValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::unaryMinus() const
{
    return nullptr;
}

shared_ptr<Value> Value::unaryNot() const
{
    return nullptr;
}
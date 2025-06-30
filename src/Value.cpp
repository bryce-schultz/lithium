#include "Values.h"
#include "Utils.h"
#include "ExpressionNode.h"
#include "Value.h"

string Value::typeAsString() const
{
    return "value";
    // subclasses should override this method to return their specific type as a string
}

Value::Value(Type type, const Range &range):
    type(type), range(range)
{ }

Value::~Value()
{ }

bool Value::toBoolean() const
{
    return type != Type::null;
}

shared_ptr<Value> Value::getMember(const string &name) const
{
    // default implementation just returns whatever is in the properties map
    // subclasses can override this to provide specific behavior for example strings
    // intercept the length property to return the dynamic length of the string
    auto it = members.find(name);
    if (it != members.end())
    {
        return it->second;
    }
    // if the property is not found, return nullptr
    return nullptr;
}

bool Value::addMember(const string &name, const shared_ptr<Value> &value, bool isConst)
{
    // check if the member already exists
    if (members.find(name) != members.end())
    {
        return false; // member already exists
    }

    // add the member to the map
    members[name] = value;

    // if it's a constant, we can track it separately if needed
    // (for dynamic properties that aren't in the map)
    if (isConst)
    {
        constants.insert(name);
    }

    return true;
}

Result<Value> Value::setMember(const string &name, const shared_ptr<Value> &value)
{
    // check if the member is constant first since we can't gaurentee that the member
    // is stored in the property map.
    if (constants.find(name) != constants.end())
    {
        return { ResultStatus::MEMBER_IS_CONSTANT, nullptr }; // member is constant
    }

    // check if the member exists
    auto it = members.find(name);
    if (it == members.end())
    {
        return { ResultStatus::MEMBER_NOT_FOUND, nullptr }; // member not found
    }

    // set the member value
    it->second = value;
    return { ResultStatus::SUCCESS, value };
}

const std::map<string, shared_ptr<Value>>& Value::getMembers() const
{
    return members;
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
        case Type::builtin:
            return add(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return add(static_pointer_cast<ArrayValue>(other));
        case Type::class_:
            return add(static_pointer_cast<ClassValue>(other));
        case Type::object:
            return add(static_pointer_cast<ObjectValue>(other));
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
        case Type::builtin:
            return sub(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return sub(static_pointer_cast<ArrayValue>(other));
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
        case Type::builtin:
            return mul(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return mul(static_pointer_cast<ArrayValue>(other));
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
        case Type::builtin:
            return div(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return div(static_pointer_cast<ArrayValue>(other));
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
        case Type::builtin:
            return mod(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return mod(static_pointer_cast<ArrayValue>(other));
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
        case Type::builtin:
            return eq(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return eq(static_pointer_cast<ArrayValue>(other));
        case Type::class_:
            return eq(static_pointer_cast<ClassValue>(other));
        case Type::object:
            return eq(static_pointer_cast<ObjectValue>(other));
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
        case Type::builtin:
            return ne(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return ne(static_pointer_cast<ArrayValue>(other));
        case Type::class_:
            return ne(static_pointer_cast<ClassValue>(other));
        case Type::object:
            return ne(static_pointer_cast<ObjectValue>(other));
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
        case Type::builtin:
            return lt(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return lt(static_pointer_cast<ArrayValue>(other));
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
        case Type::builtin:
            return le(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return le(static_pointer_cast<ArrayValue>(other));
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
        case Type::builtin:
            return gt(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return gt(static_pointer_cast<ArrayValue>(other));
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
        case Type::builtin:
            return ge(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return ge(static_pointer_cast<ArrayValue>(other));
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
        case Type::builtin:
            return logicalAnd(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return logicalAnd(static_pointer_cast<ArrayValue>(other));
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
        case Type::builtin:
            return logicalOr(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return logicalOr(static_pointer_cast<ArrayValue>(other));
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
        case Type::builtin:
            return comma(static_pointer_cast<BuiltinFunctionValue>(other));
        case Type::array:
            return comma(static_pointer_cast<ArrayValue>(other));
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

shared_ptr<Value> Value::add(const shared_ptr<ClassValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::add(const shared_ptr<ObjectValue> &other) const
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

shared_ptr<Value> Value::eq(const shared_ptr<ClassValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::eq(const shared_ptr<ObjectValue> &other) const
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

shared_ptr<Value> Value::ne(const shared_ptr<ClassValue> &other) const
{
    if (!other) return nullptr;
    return nullptr;
}

shared_ptr<Value> Value::ne(const shared_ptr<ObjectValue> &other) const
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
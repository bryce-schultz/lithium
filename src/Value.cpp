#include "Value.h"
#include "Utils.h"
#include "ExpressionNode.h"

Value::Type Value::getType() const
{
    return type;
}

bool Value::toBoolean() const
{
    return type != Type::null;
}

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
        return std::to_string(static_cast<int>(value)); // Return as integer if it is an integer
    }
    return std::to_string(value); // Otherwise return as float
}

bool NumberValue::toBoolean() const
{
    return value != 0.0; // Non-zero numbers are true, zero is false
}

StringValue::StringValue(const string &value):
    value(value)
{
    type = Type::string;
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
    return !value.empty(); // Non-empty strings are true, empty strings are false
}

BooleanValue::BooleanValue(bool value):
    value(value)
{
    type = Type::boolean;
}

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
    return value; // true is true, false is false
}

FunctionValue::FunctionValue(const std::string &name,
    std::shared_ptr<ParamListNode> params,
    std::shared_ptr<StatementNode> body,
    std::shared_ptr<Environment> closureEnv):
    name(name),
    params(params),
    body(body),
    closureEnv(closureEnv)
{
    type = Type::function;
}

const string &FunctionValue::getName() const
{
    return name;
}

std::shared_ptr<ParamListNode> FunctionValue::getParameters() const
{
    return params;
}

std::shared_ptr<StatementNode> FunctionValue::getBody() const
{
    return body;
}

std::shared_ptr<Environment> FunctionValue::getEnvironment() const
{
    return closureEnv;
}

string FunctionValue::toString() const
{
    return "<function " + name + ">";
}

ObjectValue::ObjectValue()
{
    type = Type::object;
}

void ObjectValue::setProperty(const string &name, ExpressionNode *value)
{
    properties[name] = value;
}

ExpressionNode *ObjectValue::getProperty(const string &name) const
{
    auto it = properties.find(name);
    if (it != properties.end())
    {
        return it->second;
    }
    return nullptr; // or throw an error if property not found
}

bool ObjectValue::hasProperty(const string &name) const
{
    return properties.find(name) != properties.end();
}

string ObjectValue::toString() const
{
    string result = "{";
    for (const auto &pair : properties)
    {
        if (result.length() > 1) // not the first property
        {
            result += ", ";
        }
        result += pair.first;
    }
    result += "}";
    return result;
}

NullValue::NullValue()
{
    type = Type::null;
}

string NullValue::toString() const
{
    return "null";
}

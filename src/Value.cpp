#include "Value.h"
#include "Utils.h"
#include "ExpressionNode.h"

Value::Type Value::getType() const
{
    return type;
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

string NumberValue::toString() const
{
    return std::to_string(value);
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
    return "\"" + value + "\"";
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

FunctionValue::FunctionValue(const string &name,
    const vector<string> &parameters,
    Environment *environment,
    StatementsNode *body):
    name(name),
    parameters(parameters),
    environment(environment),
    body(body)
{
    type = Type::function;
}

const string &FunctionValue::getName() const
{
    return name;
}

void FunctionValue::setName(const string &name)
{
    this->name = name;
}

const vector<string> &FunctionValue::getParameters() const
{
    return parameters;
}

void FunctionValue::setParameters(const vector<string> &parameters)
{
    this->parameters = parameters;
}

void FunctionValue::addParameter(const string &parameter)
{
    parameters.push_back(parameter);
}

Environment *FunctionValue::getEnvironment() const
{
    return environment;
}

void FunctionValue::setEnvironment(Environment *environment)
{
    this->environment = environment;
}

StatementsNode *FunctionValue::getBody() const
{
    return body;
}

void FunctionValue::setBody(StatementsNode *body)
{
    this->body = body;
}

string FunctionValue::toString() const
{
    string params = Utils::join(parameters, ", ");
    return "function " + name + "(" + params + ")";
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
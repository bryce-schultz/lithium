#include "ClassValue.h"

ClassValue::ClassValue(const std::string &name, shared_ptr<StatementNode> body):
    Value(Value::Type::class_),
    name(name),
    body(body)
{ }

const string &ClassValue::getName() const
{
    return name;
}

void ClassValue::setName(const string &name)
{
    this->name = name;
}

shared_ptr<StatementNode> ClassValue::getBody() const
{
    return body;
}

string ClassValue::toString() const
{
    return "<class " + name + ">";
}
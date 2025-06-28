#include "ClassValue.h"
#include "Values.h"

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

string ClassValue::typeAsString() const
{
    return "class";
}

shared_ptr<Value> ClassValue::eq(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ClassValue::ne(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(true, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ClassValue::eq(const shared_ptr<ClassValue> &other) const
{
    if (!other) return nullptr;
    // Classes are equal if they are the same instance
    return make_shared<BooleanValue>(this == other.get(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ClassValue::ne(const shared_ptr<ClassValue> &other) const
{
    if (!other) return nullptr;
    // Classes are not equal if they are different instances
    return make_shared<BooleanValue>(this != other.get(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ClassValue::add(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<StringValue>(toString() + other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}
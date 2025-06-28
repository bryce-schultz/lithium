#include "ObjectValue.h"
#include "Values.h"

ObjectValue::ObjectValue(const string &name, shared_ptr<Environment> env):
    Value(Type::object),
    name(name),
    env(env)
{ }

ObjectValue::~ObjectValue()
{
    // Clear the environment to break cycles
    if (env) {
        env->clear();
    }
    env.reset();
}

const string &ObjectValue::getClassName() const
{
    return name;
}

shared_ptr<Value> ObjectValue::getMember(const string &name) const
{
    auto member = env->lookup(name);
    if (member)
    {
        return member;
    }

    return nullptr;
}

Result<Value> ObjectValue::setMember(const string &name, const shared_ptr<Value> &value)
{
    // check if the member exists
    if (!env->hasVariable(name))
    {
        return { ResultStatus::MEMBER_NOT_FOUND, nullptr }; // member not found
    }

    // check if the member is constant
    if (env->hasConstant(name))
    {
        return { ResultStatus::MEMBER_IS_CONSTANT, nullptr }; // member is constant
    }

    // set the member value
    env->assign(name, value);
    return { ResultStatus::SUCCESS, value };
}

bool ObjectValue::addMember(const string &name, const shared_ptr<Value> &value, bool isConst)
{
    // check if the member already exists
    if (env->hasVariable(name))
    {
        return false; // member already exists
    }

    // add the member to the environment
    env->declare(name, value, isConst);
    return true;
}

const std::map<string, shared_ptr<Value>> &ObjectValue::getMembers() const
{
    return env->getMembers();
}

string ObjectValue::toString() const
{
    string result = "{ ";
    bool first = true;
    for (const auto &pair : env->getMembers())
    {
        if (pair.second->getType() == Type::function)
        {
            continue;
        }

        if (pair.first == "LINE") continue;
        if (pair.first == name) continue;

        if (!first)
        {
            result += ", ";
        }
        first = false;
        result += pair.first + ": " + pair.second->toString();
    }
    result += " }";
    return result;
}

shared_ptr<Value> ObjectValue::eq(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ObjectValue::ne(const shared_ptr<NullValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<BooleanValue>(true, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ObjectValue::eq(const shared_ptr<ObjectValue> &other) const
{
    if (!other) return nullptr;
    // Objects are equal if they are the same instance (pointer equality)
    return make_shared<BooleanValue>(this == other.get(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ObjectValue::ne(const shared_ptr<ObjectValue> &other) const
{
    if (!other) return nullptr;
    // Objects are not equal if they are different instances
    return make_shared<BooleanValue>(this != other.get(), Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ObjectValue::add(const shared_ptr<StringValue> &other) const
{
    if (!other) return nullptr;
    return make_shared<StringValue>(toString() + other->getValue(), Range(getRange().getStart(), other->getRange().getEnd()));
}

string ObjectValue::typeAsString() const
{
    return name;
}
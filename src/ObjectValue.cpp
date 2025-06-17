#include "ObjectValue.h"

ObjectValue::ObjectValue(const string &name, shared_ptr<Environment> env):
    Value(Type::object),
    name(name),
    env(env)
{ }

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
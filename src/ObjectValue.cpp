#include "ObjectValue.h"

ObjectValue::ObjectValue(const string &name, shared_ptr<Environment> env):
    Value(Type::object)
{
    if (!env)
    {
        return;
    }

    // Move all variables from the environment to the object members
    for (const auto &pair : env->getMembers())
    {
        bool isConstant = env->hasConstant(pair.first);
        // skip the LINE variable, as it is not a member of the object
        // and is declared on every line.
        if (pair.first == "LINE") continue;

        // skip the constructor if it has one.
        if (pair.first == name) continue;
        addMember(pair.first, pair.second, isConstant);
    }
}

string ObjectValue::toString() const
{
    string result = "{ ";
    bool first = true;
    for (const auto &pair : getMembers())
    {
        if (pair.second->getType() == Type::function)
        {
            continue;
        }

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
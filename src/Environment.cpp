#include <iostream>

#include "Environment.h"
#include "Value.h"
#include "Error.h"

using std::cout;

Environment::Environment(Environment *parent):
    parent(parent),
    variables(),
    constants()
{
}

Value *Environment::declare(const string &name, Value *value, bool constant)
{
    if (hasVariable(name))
    {
        // TODO: Handle error for variable already declared
        return nullptr; // Variable already declared
    }

    variables[name] = value;

    if (constant)
    {
        constants.insert(name);
    }

    return value;
}

Value *Environment::assign(const string &name, Value *value)
{
    Environment *env = resolve(name);
    if (!env)
    {
        return nullptr; // Variable not found in any environment
    }

    if (env->hasConstant(name))
    {
        // TODO: Handle error for assigning to a constant
        return nullptr; // Cannot assign to a constant
    }

    env->variables[name] = value;

    return value; // Return the assigned value
}

Value *Environment::lookup(const string &name) const
{
    const Environment *env = resolve(name);
    return env ? env->variables.at(name) : nullptr;
}

Environment *Environment::resolve(const string &name) const
{
    if (hasVariable(name))
    {
        return const_cast<Environment*>(this);
    }

    if (!parent)
    {
        // TODO: Handle error for unresolved variable
        return nullptr; // No parent environment to resolve to
    }

    return parent->resolve(name);
}

void Environment::dump() const
{
    cout << "Environment dump:\n";
    cout << "Variables:\n";
    for (const auto &pair : variables)
    {
        cout << "  " << pair.first << ": " << (pair.second ? pair.second->toString() : "null") << "\n";
    }
    cout << "Constants:\n";
    for (const auto &name : constants)
    {
        cout << "  " << name << "\n";
    }
    if (parent)
    {
        cout << "Parent Environment:\n";
        parent->dump();
    }
    else
    {
        cout << "No parent environment.\n";
    }
    cout << "End of Environment dump.\n";
}

bool Environment::hasVariable(const string &name) const
{
    if (variables.find(name) != variables.end())
    {
        return true;
    }
    return false;
}

bool Environment::hasConstant(const string &name) const
{
    if (constants.find(name) != constants.end())
    {
        return true;
    }
    return false;
}

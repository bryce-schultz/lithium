#include <iostream>

#include "Environment.h"
#include "Value.h"
#include "Error.h"

using std::cout;

Environment::Environment(std::shared_ptr<Environment> parent):
    parent(parent),
    variables(),
    constants()
{
}

std::shared_ptr<Value> Environment::declare(const string &name, std::shared_ptr<Value> value, bool constant)
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

std::shared_ptr<Value> Environment::assign(const string &name, std::shared_ptr<Value> value)
{
    std::shared_ptr<Environment> env = resolve(name);
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

std::shared_ptr<Value> Environment::lookup(const string &name) const
{
    std::shared_ptr<Environment> env = resolve(name);
    return env ? env->variables.at(name) : nullptr;
}

std::shared_ptr<Environment> Environment::resolve(const string &name) const
{
    if (hasVariable(name))
    {
        return std::const_pointer_cast<Environment>(shared_from_this());
    }

    if (!parent)
    {
        // TODO: Handle error for unresolved variable
        return nullptr; // No parent environment to resolve to
    }

    return parent->resolve(name);
}

std::shared_ptr<Environment> Environment::getParent() const
{
    return parent;
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

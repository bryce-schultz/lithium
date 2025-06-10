#include <iostream>

#include "Environment.h"
#include "Value.h"
#include "Error.h"

using std::cout;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::enable_shared_from_this;
using std::const_pointer_cast;
using std::dynamic_pointer_cast;

Environment::Environment(shared_ptr<Environment> parent):
    parent(parent),
    variables(),
    constants()
{
}

shared_ptr<Value> Environment::declare(const string &name, shared_ptr<Value> value, bool constant)
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

shared_ptr<Value> Environment::redeclare(const string &name, shared_ptr<Value> value, bool constant)
{
    variables[name] = value;

    if (constant)
    {
        constants.insert(name);
    }

    return value;
}

Result<Value> Environment::assign(const string &name, shared_ptr<Value> value)
{
    shared_ptr<Environment> env = resolve(name);
    if (!env)
    {
        return { VARIABLE_NOT_FOUND, nullptr };
    }

    if (env->hasConstant(name))
    {
        return { VARIABLE_IS_CONSTANT, nullptr };
    }

    env->variables[name] = value;

    return { SUCCESS, value };
}

shared_ptr<Value> Environment::lookup(const string &name) const
{
    shared_ptr<Environment> env = resolve(name);
    return env ? env->variables.at(name) : nullptr;
}

shared_ptr<Value> Environment::lookupLocal(const string &name) const
{
    auto it = variables.find(name);
    if (it != variables.end())
    {
        return it->second;
    }
    return nullptr;
}

shared_ptr<Environment> Environment::resolve(const string &name) const
{
    if (hasVariable(name))
    {
        return const_pointer_cast<Environment>(shared_from_this());
    }

    if (!parent)
    {
        return nullptr;
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

    //auto parentPtr = parent.lock();

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

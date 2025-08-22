#include <iostream>

#include "Environment.h"
#include "Value.h"
#include "FunctionValue.h"
#include "ArrayValue.h"
#include "Error.h"

using std::cout;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::enable_shared_from_this;
using std::const_pointer_cast;
using std::dynamic_pointer_cast;

Environment::Environment(shared_ptr<Environment> parent):
    parent(parent)
{ }

Environment::~Environment()
{
    // Break cycles by clearing function closures
    for (auto &pair : variables)
    {
        if (pair.second && pair.second->getType() == Value::Type::function)
        {
            auto func = std::dynamic_pointer_cast<FunctionValue>(pair.second);
            if (func)
            {
                func->clearClosureEnv();
            }
        }
    }
    variables.clear();
    constants.clear();
    parent.reset();
}

shared_ptr<Value> Environment::declare(const string &name, shared_ptr<Value> value, bool constant)
{
    if (hasVariable(name))
    {
        // TODO: Handle error for variable already declared
        return nullptr; // Variable already declared
    }

    variables[name] = std::move(value);

    if (constant)
    {
        constants.insert(name);
    }

    return variables[name];
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

shared_ptr<Value> Environment::remove(const string & name)
{
    shared_ptr<Environment> env = resolve(name);
    if (!env)
    {
        return nullptr; // Variable not found
    }

    auto it = env->variables.find(name);
    if (it != env->variables.end())
    {
        shared_ptr<Value> value = it->second;
        env->variables.erase(it);
        return value;
    }

    return nullptr; // Variable not found
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

shared_ptr<Environment> Environment::getParent() const
{
    return parent;
}

const map<string, shared_ptr<Value>> &Environment::getMembers() const
{
    return variables;
}

void Environment::clear()
{
    // First, clear closure environments in functions and arrays to break cycles
    for (auto& pair : variables)
    {
        if (!pair.second)
        {
            continue;
        }

        if (pair.second->getType() == Value::Type::function)
        {
            auto func = dynamic_pointer_cast<FunctionValue>(pair.second);
            if (func)
            {
                // Only clear closure environments if they reference this environment
                // to avoid breaking legitimate closures
                if (func->getEnvironment().get() == this)
                {
                    func->clearClosureEnv();
                }
            }
            continue;
        }

        if (pair.second->getType() != Value::Type::array)
        {
            continue;
        }

        auto array = dynamic_pointer_cast<ArrayValue>(pair.second);
        if (!array)
        {
            continue;
        }

        // Clear function closures in the array elements to break cycles
        for (int i = 0; i < array->getElementCount(); ++i)
        {
            auto element = array->getElement(i);
            if (!element || element->getType() != Value::Type::function)
            {
                continue;
            }

            auto func = dynamic_pointer_cast<FunctionValue>(element);
            if (func)
            {
                // Only clear closure environments if they reference this environment
                if (func->getEnvironment().get() == this)
                {
                    func->clearClosureEnv();
                }
            }
        }
    }

    // Clear all variables to break potential cycles
    variables.clear();
    constants.clear();
    // Don't clear parent - let it be cleaned up naturally
}

void Environment::dump() const
{
    static int id = 0;
    if (parent)
    {
        parent->dump();
        cout << "────────────────────────────────────────\nEnvironment - " << id++ << "\n────────────────────────────────────────\n";
        if (variables.empty())
        {
            cout << "  empty\n";
        }
    }
    else
    {
        id = 0;
        cout << "────────────────────────────────────────\nRoot Environment - " << id++ << "\n────────────────────────────────────────\n";
        if (variables.empty())
        {
            cout << "  root empty!\n";
        }
    }

    for (const auto &pair : variables)
    {
        bool isConstant = hasConstant(pair.first);
        string constIndicator = isConstant ? " [const]" : "";
        cout << "  " << pair.first << ": " << (pair.second ? pair.second->toString() : "null") << constIndicator << "\n";
    }
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

bool Environment::hasFunctions() const
{
    for (const auto &pair : variables)
    {
        if (pair.second && pair.second->getType() == Value::Type::function)
        {
            return true;
        }
    }
    return false;
}

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "Builtins.h"
#include "Environment.h"
#include "Values.h"
#include "Utils.h"
#include "Error.h"

using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::dynamic_pointer_cast;

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__)

// returns nullptr, this will prevent printing "null" in interactive mode
shared_ptr<Value> Builtins::printall(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);
    UNUSED(range);

    if (args.empty())
    {
        return nullptr;
    }

    for (const auto &arg : args)
    {
        // print the argument's string representation
        if (arg)
        {
            cout << arg->toString();
        }

        // print space between arguments
        if (arg != args.back())
        {
            cout << " ";
        }
    }
    cout.flush();
    return nullptr;
}

shared_ptr<Value> Builtins::type(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 1)
    {
        error("type() expects exactly 1 argument, but got " + std::to_string(args.size()), range);
        return nullptr;
    }

    const auto &arg = args[0];
    if (!arg)
    {
        return make_shared<StringValue>("undefined", range);
    }

    return make_shared<StringValue>(arg->typeAsString(), arg->getRange());
}

shared_ptr<Value> Builtins::exit(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() > 1)
    {
        error("exit() expects at most 1 argument, but got " + std::to_string(args.size()), range);
        return nullptr;
    }

    int exitCode = 0;
    if (args.size() == 1)
    {
        if (args[0]->getType() != Value::Type::number)
        {
            error("exit() expects a number argument, but got " + args[0]->typeAsString(), args[0]->getRange());
            return nullptr;
        }
        exitCode = dynamic_pointer_cast<NumberValue>(args[0])->getValue();
    }

    throw ExitException(exitCode, range);
}

shared_ptr<Value> Builtins::input(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() > 1)
    {
        error("input() expects at most 1 argument, but got " + std::to_string(args.size()), range);
        return nullptr;
    }

    string prompt;
    if (args.size() == 1 && args[0])
    {
        if (args[0]->getType() != Value::Type::string)
        {
            error("input() expects a string argument, but got " + args[0]->typeAsString(), args[0]->getRange());
            return nullptr;
        }
        prompt = dynamic_pointer_cast<StringValue>(args[0])->getValue();
    }

    cout << prompt;
    cout.flush();

    string userInput;
    std::getline(std::cin, userInput);

    return make_shared<StringValue>(userInput, range);
}

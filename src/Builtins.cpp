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
shared_ptr<Value> Builtins::print(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);
    UNUSED(range);

    if (args.empty())
    {
        return nullptr;
    }

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (args[i])
        {
            cout << args[i]->toString();
        }

        if (i != args.size() - 1)
        {
            cout << " ";
        }
    }
    cout.flush();
    return nullptr;
}

shared_ptr<Value> Builtins::println(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);
    UNUSED(range);

    for (size_t i = 0; i < args.size(); ++i)
    {
        if (args[i])
        {
            cout << args[i]->toString();
        }

        if (i != args.size() - 1)
        {
            cout << " ";
        }
    }
    cout << endl;
    return nullptr;
}

shared_ptr<Value> Builtins::printf(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.empty())
    {
        error("printf() expects at least 1 argument, but got 0", range);
        return nullptr;
    }

    if (args[0]->getType() != Value::Type::string)
    {
        error("printf() expects a string format as the first argument, but got " + args[0]->typeAsString(), args[0]->getRange());
        return nullptr;
    }

    const auto &format = dynamic_pointer_cast<StringValue>(args[0])->getValue();

    vector<string> formatArgs;
    for (size_t i = 1; i < args.size(); ++i)
    {
        if (args[i])
        {
            formatArgs.push_back(args[i]->toString());
        }
    }
    string result = Utils::formatString(format, formatArgs);

    cout << result;
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

shared_ptr<Value> Builtins::len(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 1)
    {
        error("len() expects exactly 1 argument, but got " + std::to_string(args.size()), range);
        return nullptr;
    }

    const auto &arg = args[0];
    if (!arg)
    {
        return make_shared<NullValue>(range);
    }

    switch (arg->getType())
    {
        case Value::Type::string:
            return make_shared<NumberValue>(dynamic_pointer_cast<StringValue>(arg)->length(), range);
        case Value::Type::array:
            return make_shared<NumberValue>(dynamic_pointer_cast<ArrayValue>(arg)->getElementCount(), range);
        default:
            error("len() expects a string or an array, but got " + arg->typeAsString(), arg->getRange());
            return nullptr;
    }
}

// attemtps to conver the first argument to a number if it fails it returns NullValue, but not nullptr
shared_ptr<Value> Builtins::toNumber(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 1)
    {
        error("toNumber() expects exactly 1 argument, but got " + std::to_string(args.size()), range);
        return make_shared<NullValue>(range);
    }

    const auto &arg = args[0];
    if (!arg)
    {
        return make_shared<NullValue>(range);
    }

    if (arg->getType() == Value::Type::number)
    {
        return arg; // already a number
    }

    try
    {
        double value = std::stod(arg->toString());
        return make_shared<NumberValue>(value, range);
    }
    catch (const std::invalid_argument &)
    {
        return make_shared<NullValue>(range); // conversion failed
    }
}

// this should function like random() in c
shared_ptr<Value> Builtins::randomNumber(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)
{
    UNUSED(env);

    if (args.size() != 0)
    {
        error("randomNumber() expects no arguments, but got " + std::to_string(args.size()), range);
        return nullptr;
    }

    return make_shared<NumberValue>(static_cast<double>(random()), range);
}

#include "FunctionValue.h"
#include "Values.h"
#include "Utils.h"
#include "ParamListNode.h"
#include "StatementNode.h"
#include "Environment.h"

FunctionValue::FunctionValue(const std::string &name,
    std::shared_ptr<ParamListNode> params,
    std::shared_ptr<StatementNode> body,
    std::shared_ptr<Environment> closureEnv,
    Range range):
    Value(Type::function, range), // set the type to function
    name(name),
    params(params),
    body(body),
    closureEnv(closureEnv)
{ }

const string &FunctionValue::getName() const
{
    return name;
}

std::shared_ptr<ParamListNode> FunctionValue::getParameters() const
{
    return params;
}

std::shared_ptr<StatementNode> FunctionValue::getBody() const
{
    return body;
}

std::shared_ptr<Environment> FunctionValue::getEnvironment() const
{
    return closureEnv;
}

string FunctionValue::toString() const
{
    return "<function " + name + ">";
}

shared_ptr<Value> FunctionValue::eq(const shared_ptr<FunctionValue> &other) const
{
    if (!other)
    {
        return make_shared<NullValue>();
    }
    return make_shared<BooleanValue>(this == other.get());
}

shared_ptr<Value> FunctionValue::eq(const shared_ptr<NullValue> &other) const
{
    UNUSED(other);
    return make_shared<BooleanValue>(false);
}

shared_ptr<Value> FunctionValue::ne(const shared_ptr<FunctionValue> &other) const
{
    if (!other)
    {
        return make_shared<NullValue>();
    }
    return make_shared<BooleanValue>(this != other.get());
}

shared_ptr<Value> FunctionValue::ne(const shared_ptr<NullValue> &other) const
{
    UNUSED(other);
    return make_shared<BooleanValue>(true);
}

BuiltinFunctionValue::BuiltinFunctionValue(BuiltinFunction func, Range range):
    Value(Type::builtin, range), // set the type to builtin
    func(func)
{ }

shared_ptr<Value> BuiltinFunctionValue::call(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range) const
{
    if (!func)
    {
        return nullptr;
    }

    return func(args, env, range);
}

shared_ptr<Value> BuiltinFunctionValue::bind(const shared_ptr<Value> &thisPtr)
{
    if (!thisPtr || thisPtr->getType() == Type::null)
    {
        return make_shared<NullValue>(getRange());
    }

    auto boundFunction = make_shared<BuiltinFunctionValue>(func, getRange());
    boundFunction->thisPtr = thisPtr;
    return boundFunction;
}

string BuiltinFunctionValue::toString() const
{
    return "<builtin function>";
}

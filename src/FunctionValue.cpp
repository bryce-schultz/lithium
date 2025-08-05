#include "FunctionValue.h"
#include "Values.h"
#include "Utils.h"
#include "ParamListNode.h"
#include "StatementNode.h"
#include "Environment.h"

using std::string;
using std::shared_ptr;

FunctionValue::FunctionValue(const string &name,
    shared_ptr<ParamListNode> params,
    shared_ptr<StatementNode> body,
    shared_ptr<Environment> closureEnv,
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

shared_ptr<ParamListNode> FunctionValue::getParameters() const
{
    return params;
}

shared_ptr<StatementNode> FunctionValue::getBody() const
{
    return body;
}

shared_ptr<Environment> FunctionValue::getEnvironment() const
{
    return closureEnv;
}

void FunctionValue::clearClosureEnv()
{
    closureEnv.reset();
}

void FunctionValue::rewriteClosureEnv(shared_ptr<Environment> newEnv)
{
    closureEnv = newEnv;
}

string FunctionValue::toString() const
{
    return "<function " + name + ">";
}

string FunctionValue::typeAsString() const
{
    return "function";
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

shared_ptr<Value> BuiltinFunctionValue::call(Interpreter &interpreter, const vector<shared_ptr<Value>> &args, const shared_ptr<Environment> &env, const Range &range) const
{
    if (!func)
    {
        return nullptr;
    }

    return func(interpreter, args, env, range);
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

string BuiltinFunctionValue::typeAsString() const
{
    return "builtin";
}

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

#include "Value.h"
#include "Environment.h"
#include "Range.h"

using std::shared_ptr;
using std::string;
using std::make_shared;
using std::function;

typedef function<shared_ptr<Value>(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range)> BuiltinFunction;

class FunctionValue : public Value
{
public:
    FunctionValue(const std::string &name,
        shared_ptr<ParamListNode> params,
        shared_ptr<StatementNode> body,
        shared_ptr<Environment> closureEnv,
        Range range = {});

    const string &getName() const;
    shared_ptr<ParamListNode> getParameters() const;
    shared_ptr<StatementNode> getBody() const;
    shared_ptr<Environment> getEnvironment() const;

    string toString() const override;
public:
    virtual shared_ptr<Value> eq(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<NullValue> &other) const;

    virtual shared_ptr<Value> ne(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<NullValue> &other) const;
private:
    string name;
    shared_ptr<ParamListNode> params;
    shared_ptr<StatementNode> body;
    shared_ptr<Environment> closureEnv;
};

class BuiltinFunctionValue : public Value
{
public:
    BuiltinFunctionValue(BuiltinFunction func, Range range = {});

    shared_ptr<Value> call(const vector<shared_ptr<Value>> &args, shared_ptr<Environment> env, const Range &range = {}) const;
    shared_ptr<Value> bind(const shared_ptr<Value> &thisPtr);

    string toString() const override;
public:
private:
    BuiltinFunction func;
    shared_ptr<Value> thisPtr; // allows function to be bound to an object.
};
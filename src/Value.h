#pragma once

#include <string>
#include <memory>

#include "StatementsNode.h"
#include "Environment.h"
#include "ParamListNode.h"

using std::string;
using std::static_pointer_cast;

class NullValue;
class NumberValue;
class StringValue;
class BooleanValue;
class FunctionValue;
class BuiltinFunctionValue;

class Value
{
public:
    enum class Type
    {
        null,
        number,
        string,
        boolean,
        function,
        builtin,
    };

    string typeAsString() const;
public:
    virtual ~Value() = default;
    Type getType() const;

    virtual string toString() const = 0; // makes this class abstract
    virtual bool toBoolean() const; // Default implementation for converting to boolean
public:
    // dispatchers
    shared_ptr<Value> add(const shared_ptr<Value> &other) const;
    shared_ptr<Value> sub(const shared_ptr<Value> &other) const;
    shared_ptr<Value> mul(const shared_ptr<Value> &other) const;
    shared_ptr<Value> div(const shared_ptr<Value> &other) const;
    shared_ptr<Value> mod(const shared_ptr<Value> &other) const;
    shared_ptr<Value> eq(const shared_ptr<Value> &other) const;
    shared_ptr<Value> ne(const shared_ptr<Value> &other) const;
    shared_ptr<Value> lt(const shared_ptr<Value> &other) const;
    shared_ptr<Value> le(const shared_ptr<Value> &other) const;
    shared_ptr<Value> gt(const shared_ptr<Value> &other) const;
    shared_ptr<Value> ge(const shared_ptr<Value> &other) const;
    shared_ptr<Value> logicalAnd(const shared_ptr<Value> &other) const;
    shared_ptr<Value> logicalOr(const shared_ptr<Value> &other) const;

    // + operator overloads
    virtual shared_ptr<Value> add(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<BuiltinFunctionValue> &other) const;

    // - operator overloads
    virtual shared_ptr<Value> sub(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<BuiltinFunctionValue> &other) const;

    // * operator overloads
    virtual shared_ptr<Value> mul(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<BuiltinFunctionValue> &other) const;

    // / operator overloads
    virtual shared_ptr<Value> div(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<BuiltinFunctionValue> &other) const;

    // % operator overloads
    virtual shared_ptr<Value> mod(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<BuiltinFunctionValue> &other) const;

    // == operator overloads
    virtual shared_ptr<Value> eq(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<BuiltinFunctionValue> &other) const;

    // != operator overloads
    virtual shared_ptr<Value> ne(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<BuiltinFunctionValue> &other) const;

    // < operator overloads

    virtual shared_ptr<Value> lt(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<BuiltinFunctionValue> &other) const;

    // <= operator overloads

    virtual shared_ptr<Value> le(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<BuiltinFunctionValue> &other) const;

    // > operator overloads
    virtual shared_ptr<Value> gt(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<BuiltinFunctionValue> &other) const;

    // >= operator overloads
    virtual shared_ptr<Value> ge(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<BuiltinFunctionValue> &other) const;

    // && operator overloads
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<BuiltinFunctionValue> &other) const;

    // || operator overloads
    virtual shared_ptr<Value> logicalOr(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<BuiltinFunctionValue> &other) const;

    // Unary operators
    virtual shared_ptr<Value> unaryMinus() const;
    virtual shared_ptr<Value> unaryNot() const;
protected:
    Type type;
};
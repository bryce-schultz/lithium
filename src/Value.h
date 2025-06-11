#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include "StatementsNode.h"
#include "Environment.h"
#include "ParamListNode.h"
#include "Range.h"
#include "Location.h"

using std::string;
using std::static_pointer_cast;
using std::shared_ptr;
using std::make_shared;

// forward declarations of the Value subclasses
class NullValue;
class NumberValue;
class StringValue;
class BooleanValue;
class FunctionValue;
class BuiltinFunctionValue;
class ArrayValue;

class Value
{
public:
    enum class Type
    {
        null,
        number,
        string,
        boolean,
        array,
        function,
        builtin,
    };

    string typeAsString() const;
public:
    Value(Type type, Range range = {});
    virtual ~Value() = default;
    Type getType() const;
    Range getRange() const;

    virtual string toString() const = 0; // makes this class abstract
    virtual bool toBoolean() const;
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
    shared_ptr<Value> comma(const shared_ptr<Value> &other) const;

    // + operator overloads
    virtual shared_ptr<Value> add(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> add(const shared_ptr<ArrayValue> &other) const;

    // - operator overloads
    virtual shared_ptr<Value> sub(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> sub(const shared_ptr<ArrayValue> &other) const;

    // * operator overloads
    virtual shared_ptr<Value> mul(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> mul(const shared_ptr<ArrayValue> &other) const;

    // / operator overloads
    virtual shared_ptr<Value> div(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> div(const shared_ptr<ArrayValue> &other) const;

    // % operator overloads
    virtual shared_ptr<Value> mod(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> mod(const shared_ptr<ArrayValue> &other) const;

    // == operator overloads
    virtual shared_ptr<Value> eq(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> eq(const shared_ptr<ArrayValue> &other) const;

    // != operator overloads
    virtual shared_ptr<Value> ne(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> ne(const shared_ptr<ArrayValue> &other) const;

    // < operator overloads

    virtual shared_ptr<Value> lt(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> lt(const shared_ptr<ArrayValue> &other) const;

    // <= operator overloads

    virtual shared_ptr<Value> le(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> le(const shared_ptr<ArrayValue> &other) const;

    // > operator overloads
    virtual shared_ptr<Value> gt(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> gt(const shared_ptr<ArrayValue> &other) const;

    // >= operator overloads
    virtual shared_ptr<Value> ge(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> ge(const shared_ptr<ArrayValue> &other) const;

    // && operator overloads
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> logicalAnd(const shared_ptr<ArrayValue> &other) const;

    // || operator overloads
    virtual shared_ptr<Value> logicalOr(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> logicalOr(const shared_ptr<ArrayValue> &other) const;

    // , operator overloads
    virtual shared_ptr<Value> comma(const shared_ptr<NullValue> &other) const;
    virtual shared_ptr<Value> comma(const shared_ptr<NumberValue> &other) const;
    virtual shared_ptr<Value> comma(const shared_ptr<StringValue> &other) const;
    virtual shared_ptr<Value> comma(const shared_ptr<BooleanValue> &other) const;
    virtual shared_ptr<Value> comma(const shared_ptr<FunctionValue> &other) const;
    virtual shared_ptr<Value> comma(const shared_ptr<BuiltinFunctionValue> &other) const;
    virtual shared_ptr<Value> comma(const shared_ptr<ArrayValue> &other) const;

    // Unary operators
    virtual shared_ptr<Value> unaryMinus() const;
    virtual shared_ptr<Value> unaryNot() const;
protected:
    Type type;
    Range range;
};
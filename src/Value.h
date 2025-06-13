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
#include "Result.h"

using std::string;
using std::static_pointer_cast;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::cout;
using std::endl;

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
    enum ResultStatus
    {
        SUCCESS = 0,
        MEMBER_NOT_FOUND = 1,
        MEMBER_ALREADY_DECLARED = 2,
        MEMBER_IS_CONSTANT = 3,
    };
public:
    Value(Type type, Range range = {});
    virtual ~Value();

    Type getType() const;
    Range getRange() const;

    virtual string toString() const = 0; // makes this class abstract
    virtual bool toBoolean() const;

    // it's recommended to call this base implementation in any subclasses that override this method
    // to avoid duplicate code for property retrieval, then if it returns nullptr, you can implement
    // specific behavior in the subclass (see string length property for example)
    virtual shared_ptr<Value> getMember(const string &name) const;

    // add a member, returns false if the member already exists
    bool addMember(const string &name, const shared_ptr<Value> &value, bool isConst);

    // set a member, returns false if the member does not exist
    Result<Value> setMember(const string &name, const shared_ptr<Value> &value);

    const std::map<string, shared_ptr<Value>>& getMembers() const;
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
    map<string, shared_ptr<Value>> members;
    set<string> constants;
};
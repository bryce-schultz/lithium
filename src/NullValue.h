#pragma once

#include "Value.h"

class NullValue : public Value
{
public:
    NullValue(Range range = {});

    virtual string toString() const override;
    virtual bool toBoolean() const override;
    virtual string typeAsString() const override;
public:
    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<NullValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<StringValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<BooleanValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<FunctionValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<FunctionValue> &other) const override;

    // unsupported for now as this shouldn't be the case.
    //virtual shared_ptr<Value> eq(const shared_ptr<BuiltinFunctionValue> &other) const override;
    //virtual shared_ptr<Value> ne(const shared_ptr<BuiltinFunctionValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<ArrayValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<ArrayValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<ClassValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<ClassValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<ObjectValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<ObjectValue> &other) const override;

    virtual shared_ptr<Value> unaryNot() const override;
};
#pragma once

#include "Value.h"

class NullValue : public Value
{
public:
    NullValue();

    string toString() const override;
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
};
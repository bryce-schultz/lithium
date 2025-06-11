#pragma once

#include <memory>

#include "Values.h"
#include "Range.h"

class NumberValue : public Value
{
public:
    NumberValue(double value, Range range = {});

    double getValue() const;
    void setValue(double value);

    bool isInteger() const;

    virtual string toString() const override;
    virtual bool toBoolean() const override;
public:
    virtual shared_ptr<Value> add(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> sub(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> mul(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> div(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> mod(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> eq(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> ne(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> lt(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> le(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> gt(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> ge(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> logicalAnd(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> logicalOr(const shared_ptr<NumberValue> &other) const override;

    virtual shared_ptr<Value> unaryMinus() const override;
private:
    double value;
};
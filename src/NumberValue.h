#pragma once

#include <memory>

#include "Values.h"
#include "Range.h"

class NumberValue : public Value
{
public:
    NumberValue(int value, const Range &range = {});
    NumberValue(unsigned long value, const Range &range = {});
    NumberValue(double value, const Range &range = {});

    inline double getValue() const { return value; }
    inline void setValue(double v) { value = v; }
    inline bool isInteger() const { return value == static_cast<int>(value); }

    virtual string toString() const override;
    virtual bool toBoolean() const override;
    virtual string typeAsString() const override;
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

    virtual shared_ptr<Value> unaryNot() const override;
private:
    double value;
};
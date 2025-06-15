#pragma once

#include <memory>

#include "Value.h"

class BooleanValue : public Value
{
public:
    BooleanValue(bool value, Range range = {});

    bool getValue() const;
    void setValue(bool value);

    string toString() const override;
    bool toBoolean() const override;
public:
    virtual shared_ptr<Value> eq(const shared_ptr<BooleanValue> &other) const override;
    virtual shared_ptr<Value> eq(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> eq(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> ne(const shared_ptr<BooleanValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> lt(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> le(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> gt(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> ge(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> logicalAnd(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> logicalOr(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> unaryNot() const override;

    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const override;
private:
    bool value;
};
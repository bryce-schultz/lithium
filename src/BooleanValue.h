#pragma once

#include <memory>

#include "Value.h"

class BooleanValue : public Value
{
public:
    BooleanValue(bool value);

    bool getValue() const;
    void setValue(bool value);

    string toString() const override;
    bool toBoolean() const override;
public:
    virtual shared_ptr<Value> eq(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> ne(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> lt(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> le(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> gt(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> ge(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> logicalAnd(const shared_ptr<BooleanValue> &other) const override;

    virtual shared_ptr<Value> logicalOr(const shared_ptr<BooleanValue> &other) const override;
private:
    bool value;
};
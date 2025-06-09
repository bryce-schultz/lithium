#pragma once

#include "Value.h"

class StringValue : public Value
{
public:
    StringValue(const string &value, Range range = {});

    const string &getValue() const;
    void setValue(const string &value);

    string toString() const override;
    bool toBoolean() const override; // Override toBoolean for string values
public:
    virtual shared_ptr<Value> add(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<BooleanValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> ne(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> lt(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> le(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> gt(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> ge(const shared_ptr<StringValue> &other) const override;
private:
    void replaceEscapeSequences();
private:
    string value;
};
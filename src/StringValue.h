#pragma once

#include "Value.h"

class StringValue : public Value
{
public:
    StringValue(char c, Range range = {});
    StringValue(const string &value, Range range = {});

    const string &getValue() const;
    void setValue(const string &value);

    char getCharAt(int index) const;
    size_t length() const;
    bool isEmpty() const;

    string toString() const override;
    bool toBoolean() const override; // Override toBoolean for string values

    virtual shared_ptr<Value> getMember(const string &name) const override;
public:
    virtual shared_ptr<Value> add(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<BooleanValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<ArrayValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> eq(const shared_ptr<StringValue> &other) const override;
    virtual shared_ptr<Value> eq(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> eq(const shared_ptr<BooleanValue> &other) const override;
    virtual shared_ptr<Value> eq(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> ne(const shared_ptr<StringValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<BooleanValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> lt(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> le(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> gt(const shared_ptr<StringValue> &other) const override;

    virtual shared_ptr<Value> ge(const shared_ptr<StringValue> &other) const override;
private:
    void replaceEscapeSequences();
private:
    string value;
};
#pragma once

#include "Value.h"

class StringValue : public Value
{
public:
    StringValue(char c, Range range = {});
    StringValue(const string &value, Range range = {});

    inline const string &getValue() const { return value; }
    inline void setValue(const string &v) { value = v; }
    inline char getCharAt(int index) const { return value[index]; }
    inline size_t length() const { return value.length(); }
    inline bool isEmpty() const { return value.empty(); }

    string toString() const override;
    bool toBoolean() const override; // Override toBoolean for string values

    virtual shared_ptr<Value> getMember(const string &name) const override;
    virtual string typeAsString() const override;
public:
    virtual shared_ptr<Value> add(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<BooleanValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<ArrayValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<NullValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<ClassValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<ObjectValue> &other) const override;

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
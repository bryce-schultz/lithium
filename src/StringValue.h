#pragma once

#include "Value.h"

class StringValue : public Value
{
public:
    StringValue(char c, const Range &range = {});
    StringValue(const string &value, const Range &range = {});
    ~StringValue() override;

    // Factory method for creating string values with potential caching
    static shared_ptr<StringValue> create(const string &value, const Range &range = {});

    void registerBuiltins();

    inline const string &getValue() const { return value; }
    inline void setValue(const string &v) { value = v; }
    inline char getCharAt(int index) const { return value[index]; }
    inline size_t length() const { return value.length(); }
    inline bool isEmpty() const { return value.empty(); }

    string toString() const override;
    bool toBoolean() const override; // Override toBoolean for string values

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
    string value;
};
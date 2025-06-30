#pragma once

#include "Value.h"

class ClassValue : public Value
{
public:
    ClassValue(const string &name, const shared_ptr<StatementNode> &body);

    inline const string &getName() const { return name; }
    inline void setName(const string &n) { name = n; }
    inline shared_ptr<StatementNode> getBody() const { return body; }

    // Addition operations (for string concatenation)
    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const override;

    // Equality operations
    virtual shared_ptr<Value> eq(const shared_ptr<NullValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<NullValue> &other) const override;
    virtual shared_ptr<Value> eq(const shared_ptr<ClassValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<ClassValue> &other) const override;

    virtual string toString() const override;
    virtual string typeAsString() const override;
public:
private:
    string name;
    shared_ptr<StatementNode> body;
};
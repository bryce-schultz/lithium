#pragma once

#include <memory>
#include <algorithm>

#include "Value.h"

using std::shared_ptr;
using std::string;

class ArrayValue : public Value
{
public:
    ArrayValue(const vector<shared_ptr<Value>> &arr, const Range &range = {});

    inline const vector<shared_ptr<Value>> &getElements() const { return elements; }
    inline shared_ptr<Value> getElement(int index) const { return elements[index]; }
    inline void addElement(const shared_ptr<Value> &element) { elements.push_back(element); }
    inline void removeElement(int index) { elements.erase(elements.begin() + index); }
    inline int getElementCount() const { return static_cast<int>(elements.size()); }
    inline bool isEmpty() const { return elements.empty(); }
    inline void clear() { elements.clear(); }
    inline void setElement(int index, const shared_ptr<Value> &value) { elements[index] = value; }
    inline int find(const shared_ptr<Value> &value) const
    {
        for (size_t i = 0; i < elements.size(); ++i)
        {
            if (elements[i]->eq(value)->toBoolean())
            {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    string toString() const override;
    bool toBoolean() const override;

    virtual string typeAsString() const override;

public:
    virtual shared_ptr<Value> eq(const shared_ptr<ArrayValue> &other) const override;
    virtual shared_ptr<Value> eq(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> ne(const shared_ptr<ArrayValue> &other) const override;
    virtual shared_ptr<Value> ne(const shared_ptr<NullValue> &other) const override;

    virtual shared_ptr<Value> add(const shared_ptr<NumberValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<StringValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<BooleanValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<ArrayValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<FunctionValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<BuiltinFunctionValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<NullValue> &other) const override;
    virtual shared_ptr<Value> add(const shared_ptr<ObjectValue> &other) const override;

private:
    vector<shared_ptr<Value>> elements;
};
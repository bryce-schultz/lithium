#pragma once

#include <memory>

#include "Value.h"

using std::shared_ptr;
using std::string;

class ArrayValue : public Value
{
public:
    ArrayValue(const vector<shared_ptr<Value>> &arr, Range range = {});

    const vector<shared_ptr<Value>> &getElements() const;
    shared_ptr<Value> getElement(int index) const;
    void addElement(const shared_ptr<Value> &element);
    void removeElement(int index);
    int getElementCount() const;
    bool isEmpty() const;
    void clear();
    void setElement(int index, const shared_ptr<Value> &value);

    string toString() const override;
    bool toBoolean() const override;
private:
    vector<shared_ptr<Value>> elements;
};
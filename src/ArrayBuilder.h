#pragma once

#include "Values.h"

class ArrayBuilder
{
public:
    ArrayBuilder() = default;

    void add(int number)
    {
        elements.push_back(make_shared<NumberValue>(number));
    }

    void add(double number)
    {
        elements.push_back(make_shared<NumberValue>(number));
    }

    void add(const string &str)
    {
        elements.push_back(make_shared<StringValue>(str));
    }

    void add(bool boolean)
    {
        elements.push_back(make_shared<BooleanValue>(boolean));
    }

    void add(const shared_ptr<Value> &value)
    {
        if (value)
        {
            elements.push_back(value);
        }
        else
        {
            elements.push_back(make_shared<NullValue>());
        }
    }

    void add(const vector<shared_ptr<Value>> &values)
    {
        for (const auto &value : values)
        {
            add(value);
        }
    }

    shared_ptr<ArrayValue> build()
    {
        return make_shared<ArrayValue>(elements);
    }
private:
    vector<shared_ptr<Value>> elements;
};
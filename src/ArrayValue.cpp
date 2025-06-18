#include <vector>

#include "ArrayValue.h"
#include "Values.h"
#include "Error.h"
#include "Environment.h"
#include "Utils.h"

using std::shared_ptr;
using std::vector;
using std::make_shared;
using std::string;
using std::dynamic_pointer_cast;

#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__)

#define errorAt(msg, location, range) \
    locationRangeError(msg, location, range, __FILE__, __LINE__)

ArrayValue::ArrayValue(const vector<shared_ptr<Value>> &arr, Range range)
    : Value(Type::array, range), elements(arr)
{
    addMember("push", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(env);
            for (const auto &arg : args)
            {
                if (!arg)
                {
                    errorAt("push() expects non-null arguments", arg->getRange().getStart(), range);
                    return nullptr;
                }
            }
            elements.insert(elements.end(), args.begin(), args.end());
            return make_shared<NullValue>(range);
        },
        getRange()
    ), true);

    addMember("pop", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("pop() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }
            if (elements.empty())
            {
                return make_shared<NullValue>(range); // Return null if the array is empty
            }
            auto lastElement = elements.back();
            elements.pop_back();
            return lastElement;
        },
        getRange()
    ), true);

    addMember("length", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("length() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }
            return make_shared<NumberValue>(static_cast<double>(elements.size()), range);
        },
        getRange()
    ), true);

    addMember("clear", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("clear() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }
            elements.clear();
            return make_shared<NullValue>(range);
        },
        getRange()
    ), true);

    addMember("empty", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("empty() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }
            return make_shared<BooleanValue>(elements.empty(), range);
        },
        getRange()
    ), true);

    addMember("get", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(env);
            if (args.size() != 1)
            {
                errorAt("get() expects exactly one argument", range.getStart(), range);
                return nullptr;
            }
            auto index = dynamic_pointer_cast<NumberValue>(args[0]);
            if (!index)
            {
                errorAt("get() expects a number as the first argument", args[0]->getRange().getStart(), range);
                return nullptr;
            }
            int idx = static_cast<int>(index->getValue());
            return getElement(idx);
        },
        getRange()
    ), true);

    addMember("set", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(env);
            if (args.size() != 2)
            {
                errorAt("set() expects exactly two arguments", range.getStart(), range);
                return nullptr;
            }
            auto index = dynamic_pointer_cast<NumberValue>(args[0]);
            if (!index)
            {
                errorAt("set() expects a number as the first argument", args[0]->getRange().getStart(), range);
                return nullptr;
            }
            int idx = static_cast<int>(index->getValue());
            setElement(idx, args[1]);
            return make_shared<NullValue>(range);
        },
        getRange()
    ), true);

    addMember("remove", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(env);
            if (args.size() != 1)
            {
                errorAt("remove() expects exactly one argument", range.getStart(), range);
                return nullptr;
            }
            auto index = dynamic_pointer_cast<NumberValue>(args[0]);
            if (!index)
            {
                errorAt("remove() expects a number as the first argument", args[0]->getRange().getStart(), range);
                return nullptr;
            }
            int idx = static_cast<int>(index->getValue());
            removeElement(idx);
            return make_shared<NullValue>(range);
        },
        getRange()
    ), true);

    addMember("find", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(env);
            if (args.size() != 1)
            {
                errorAt("find() expects exactly one argument", range.getStart(), range);
                return nullptr;
            }
            int index = find(args[0]);
            return make_shared<NumberValue>(static_cast<double>(index), range);
        },
        getRange()
    ), true);

    addMember("contains", make_shared<BuiltinFunctionValue>(
        [this](const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(env);
            if (args.size() != 1)
            {
                errorAt("contains() expects exactly one argument", range.getStart(), range);
                return nullptr;
            }
            bool found = find(args[0]) != -1;
            return make_shared<BooleanValue>(found, range);
        },
        getRange()
    ), true);
}

const vector<shared_ptr<Value>> &ArrayValue::getElements() const {
    return elements;
}

shared_ptr<Value> ArrayValue::getElement(int index) const {
    if (index < 0 || index >= static_cast<int>(elements.size())) {
        return nullptr; // Index out of bounds
    }
    return elements[index];
}

void ArrayValue::addElement(const shared_ptr<Value> &element) {
    elements.push_back(element);
}

void ArrayValue::removeElement(int index) {
    if (index < 0 || index >= static_cast<int>(elements.size())) {
        return; // Index out of bounds, do nothing
    }
    elements.erase(elements.begin() + index);
}

int ArrayValue::getElementCount() const {
    return static_cast<int>(elements.size());
}

bool ArrayValue::isEmpty() const {
    return elements.empty();
}

void ArrayValue::clear() {
    elements.clear();
}

void ArrayValue::setElement(int index, const shared_ptr<Value> &value) {
    if (index < 0 || index >= static_cast<int>(elements.size())) {
        return; // Index out of bounds, do nothing
    }
    elements[index] = value;
}

int ArrayValue::find(const shared_ptr<Value> &value) const
{
    for (size_t i = 0; i < elements.size(); ++i)
    {
        if (elements[i]->eq(value)->toBoolean())
        {
            return static_cast<int>(i); // Return the index of the first matching element
        }
    }
    return -1; // Return -1 if not found
}

string ArrayValue::toString() const
{
    string result = "[";
    for (size_t i = 0; i < elements.size(); ++i)
    {
        result += elements[i]->toString();
        if (i + 1 < elements.size())
        {
            result += ", ";
        }
    }
    result += "]";
    return result;
}

bool ArrayValue::toBoolean() const
{
    return !elements.empty();
}

shared_ptr<Value> ArrayValue::eq(const shared_ptr<ArrayValue> &other) const
{
    if (elements.size() != (size_t)other->getElementCount())
    {
        return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
    }

    for (size_t i = 0; i < elements.size(); ++i)
    {
        if (!elements[i]->eq(other->getElement(i)))
        {
            return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
        }
    }

    return make_shared<BooleanValue>(true, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::eq(const shared_ptr<NullValue> &other) const
{
    return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::ne(const shared_ptr<ArrayValue> &other) const
{
    if (elements.size() != (size_t)other->getElementCount())
    {
        return make_shared<BooleanValue>(true, Range(getRange().getStart(), other->getRange().getEnd()));
    }

    for (size_t i = 0; i < elements.size(); ++i)
    {
        if (!elements[i]->ne(other->getElement(i)))
        {
            return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
        }
    }

    return make_shared<BooleanValue>(false, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::ne(const shared_ptr<NullValue> &other) const
{
    return make_shared<BooleanValue>(true, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<NumberValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.push_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<StringValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.push_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<BooleanValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.push_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<ArrayValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    //newElements.push_back(other); // uncomment this for the += to do nesting arrays ( [1, 2] += [3, 4] would result in [1, 2, [3, 4]] )
    // if you want to flatten the array, use the line below instead ( [1, 2] += [3, 4] would result in [1, 2, 3, 4] )
    newElements.insert(newElements.end(), other->getElements().begin(), other->getElements().end());
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<FunctionValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.push_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<BuiltinFunctionValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.push_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<NullValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.push_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}
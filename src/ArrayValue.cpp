#include <vector>

#include "ArrayValue.h"
#include "Values.h"

using std::shared_ptr;
using std::vector;
using std::make_shared;

ArrayValue::ArrayValue(const vector<shared_ptr<Value>> &arr, Range range)
    : Value(Type::array, range), elements(arr) {}

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
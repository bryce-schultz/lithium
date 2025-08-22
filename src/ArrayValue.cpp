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

ArrayValue::ArrayValue(const vector<shared_ptr<Value>> &arr, const Range &range)
    : Value(Type::array, range), elements(arr)
{
    addMember("push", make_shared<BuiltinFunctionValue>(
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
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
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
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
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
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
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
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
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
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
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
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
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
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
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
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
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
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
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
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

    addMember("join", make_shared<BuiltinFunctionValue>(
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (args.size() > 1)
            {
                errorAt("join() expects at most one argument", range.getStart(), range);
                return nullptr;
            }

            string separatorStr = "";
            if (args.size() == 1)
            {
                auto separator = dynamic_pointer_cast<StringValue>(args[0]);
                if (!separator)
                {
                    errorAt("join() expects a string as the first argument", args[0]->getRange().getStart(), range);
                    return nullptr;
                }
                separatorStr = separator->getValue();
            }

            string result;
            for (size_t i = 0; i < elements.size(); ++i)
            {
                result += elements[i]->toString();
                if (i + 1 < elements.size())
                {
                    result += separatorStr;
                }
            }
            return make_shared<StringValue>(result, range);
        },
        getRange()
    ), true);

    addMember("sort", make_shared<BuiltinFunctionValue>(
        [this](Interpreter &interpreter, const vector<shared_ptr<Value>>& args, shared_ptr<Environment> env, const Range &range = {}) -> shared_ptr<Value>
        {
            UNUSED(interpreter);
            UNUSED(env);
            if (!args.empty())
            {
                errorAt("sort() does not take any arguments", args[0]->getRange().getStart(), range);
                return nullptr;
            }

            if (elements.empty())
            {
                return make_shared<NullValue>(range);
            }

            // Check that all elements are of the same type
            Value::Type firstType = elements[0]->getType();
            for (const auto& element : elements)
            {
                if (element->getType() != firstType)
                {
                    errorAt("sort() requires all array elements to be of the same type", range.getStart(), range);
                    return nullptr;
                }
            }

            // Only allow sorting for numbers and strings
            if (firstType != Value::Type::number && firstType != Value::Type::string_)
            {
                errorAt("sort() only works with arrays of numbers or strings", range.getStart(), range);
                return nullptr;
            }

            if (firstType == Value::Type::number)
            {
                std::sort(elements.begin(), elements.end(),
                          [](const shared_ptr<Value>& a, const shared_ptr<Value>& b) 
                          {
                              auto numA = dynamic_pointer_cast<NumberValue>(a);
                              auto numB = dynamic_pointer_cast<NumberValue>(b);
                              return numA->getValue() < numB->getValue();
                          });
            }
            else // firstType == Value::Type::string_
            {
                std::sort(elements.begin(), elements.end(),
                          [](const shared_ptr<Value>& a, const shared_ptr<Value>& b) 
                          {
                              auto strA = dynamic_pointer_cast<StringValue>(a);
                              auto strB = dynamic_pointer_cast<StringValue>(b);
                              return strA->getValue() < strB->getValue();
                          });
            }

            return nullptr; // Return null to indicate success
        },
        getRange()
    ), true);
}

string ArrayValue::toString() const
{
    if (elements.empty())
    {
        return "[]";
    }

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

string ArrayValue::typeAsString() const
{
    return "array";
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
    newElements.emplace_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<StringValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.emplace_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<BooleanValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.emplace_back(other);
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
    newElements.emplace_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<BuiltinFunctionValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.emplace_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<NullValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.emplace_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}

shared_ptr<Value> ArrayValue::add(const shared_ptr<ObjectValue> &other) const
{
    vector<shared_ptr<Value>> newElements = elements;
    newElements.emplace_back(other);
    return make_shared<ArrayValue>(newElements, Range(getRange().getStart(), other->getRange().getEnd()));
}
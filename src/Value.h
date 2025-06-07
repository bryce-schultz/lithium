#pragma once

#include <string>
#include <memory>

#include "StatementsNode.h"
#include "Environment.h"
#include "ParamListNode.h"

using std::string;

class Value
{
public:
    enum class Type
    {
        null,
        number,
        string,
        boolean,
        function,
        object,
        array
    };
public:
    virtual ~Value() = default;
    Type getType() const;

    virtual string toString() const = 0; // makes this class abstract
    virtual bool toBoolean() const; // Default implementation for converting to boolean
protected:
    Type type;
};

class NullValue : public Value
{
public:
    NullValue();

    string toString() const override;
};

class NumberValue : public Value
{
public:
    NumberValue(double value);

    double getValue() const;
    void setValue(double value);

    bool isInteger() const; // Check if the number is an integer

    virtual string toString() const override;
    virtual bool toBoolean() const override;
private:
    double value;
};

class StringValue : public Value
{
public:
    StringValue(const string &value);

    const string &getValue() const;
    void setValue(const string &value);

    string toString() const override;
    bool toBoolean() const override; // Override toBoolean for string values
private:
    string value;
};

class BooleanValue : public Value
{
public:
    BooleanValue(bool value);

    bool getValue() const;
    void setValue(bool value);

    string toString() const override;
    bool toBoolean() const override; // Override toBoolean for boolean values
private:
    bool value;
};

class FunctionValue : public Value
{
public:
    FunctionValue(const std::string &name,
        std::shared_ptr<ParamListNode> params,
        std::shared_ptr<StatementNode> body,
        std::shared_ptr<Environment> closureEnv);
    const std::string &getName() const;
    std::shared_ptr<ParamListNode> getParameters() const;
    std::shared_ptr<StatementNode> getBody() const;
    std::shared_ptr<Environment> getEnvironment() const;
    std::string toString() const override;
private:
    std::string name;
    std::shared_ptr<ParamListNode> params;
    std::shared_ptr<StatementNode> body;
    std::shared_ptr<Environment> closureEnv; // changed to weak_ptr
};

class ObjectValue : public Value
{
public:
    ObjectValue();

    void setProperty(const string &name, ExpressionNode* value);
    ExpressionNode *getProperty(const string &name) const;
    bool hasProperty(const string &name) const;

    string toString() const override;
private:
    std::map<string, ExpressionNode*> properties; // properties of the object
};

/*class ArrayValue : public Value
{
public:
    ArrayValue();

    void addElement(ExpressionNode *element);
    ExpressionNode *getElement(int index) const;
    int getElementCount() const;

    string toString() const override;
private:
    std::vector<ExpressionNode*> elements; // elements of the array
};*/
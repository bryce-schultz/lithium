#pragma once

#include <string>

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
    FunctionValue(const string &name, ParamListNode *parameters, StatementNode *body, Environment *environment);
    FunctionValue(const FunctionValue &other) = delete; // Disable copy constructor
    FunctionValue &operator=(const FunctionValue &other) = delete; // Disable copy assignment operator
    FunctionValue(FunctionValue &&other) = default; // Enable move constructor
    FunctionValue &operator=(FunctionValue &&other) = default; // Enable move assignment operator

    const string &getName() const;
    void setName(const string &name);

    ParamListNode *getParameters() const;
    void setParameters(ParamListNode *parameters);

    Environment *getEnvironment() const;
    void setEnvironment(Environment *environment);

    StatementNode *getBody() const;
    void setBody(StatementNode *body);

    string toString() const override;
private:
    string name;
    ParamListNode *parameters; // List of parameter names
    StatementNode *body; // The function body as a list of statements
    Environment *environment; // The environment where the function is declared
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
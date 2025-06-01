#pragma once

#include <string>

#include "StatementsNode.h"
#include "Environment.h"

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
protected:
    Type type;
};

class NumberValue : public Value
{
public:
    NumberValue(double value);

    double getValue() const;
    void setValue(double value);

    string toString() const override;
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
private:
    bool value;
};

class FunctionValue : public Value
{
public:
    FunctionValue(const string &name, const vector<string> &parameters, Environment *environment, StatementsNode *body);

    const string &getName() const;
    void setName(const string &name);

    const vector<string> &getParameters() const;
    void setParameters(const vector<string> &parameters);
    void addParameter(const string &parameter);

    Environment *getEnvironment() const;
    void setEnvironment(Environment *environment);

    StatementsNode *getBody() const;
    void setBody(StatementsNode *body);

    string toString() const override;
private:
    string name;
    vector<string> parameters;
    Environment *environment; // The environment where the function is declared
    StatementsNode *body; // The function body as a list of statements
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
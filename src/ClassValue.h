#pragma once

#include "Value.h"

class ClassValue : public Value
{
public:
    ClassValue(const string &name, shared_ptr<StatementNode> body);

    const string &getName() const;
    void setName(const string &name);

    shared_ptr<StatementNode> getBody() const;

    string toString() const override;
public:
private:
    string name;
    shared_ptr<StatementNode> body;
    //shared_ptr<Environment> environment;
};
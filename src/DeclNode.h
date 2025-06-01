#pragma once

#include "StatementNode.h"

class DeclNode : public StatementNode
{
public:
    DeclNode();

    virtual string getName() const = 0;

    virtual bool isConst() const = 0;

    virtual void visit(Visitor *visitor);
protected:
};
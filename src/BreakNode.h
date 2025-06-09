#pragma once

#include "StatementNode.h"
#include "Visitor.h"
#include "Token.h"

class BreakNode : public StatementNode
{
public:
    BreakNode(const Token &token);

    virtual void visit(Visitor *visitor) override;
};
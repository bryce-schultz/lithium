#pragma once

#include "Node.h"
#include "StatementNode.h"
#include "Visitor.h"

class StatementsNode : public Node
{
public:
    StatementsNode(StatementNode *statement = nullptr);

    void addStatement(StatementNode *statement);

    void visit(Visitor *visitor) override;
};
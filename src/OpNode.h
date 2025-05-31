#pragma once

#include "Node.h"
#include "Token.h"
#include "Visitor.h"

class OpNode : public Node
{
public:
    OpNode(Token op);

    int getType() const;

    void visit(Visitor *visitor) override;
private:
    Token op;
};
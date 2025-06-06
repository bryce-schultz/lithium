//**************************************************
// File: OpNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the OpNode class, representing
// operator nodes in the AST for expressions.
//**************************************************

#pragma once

#include <memory>

#include "Node.h"
#include "Token.h"
#include "Visitor.h"

using std::shared_ptr;

class OpNode : public Node
{
public:
    using Ptr = shared_ptr<OpNode>;
public:
    OpNode(Token op);

    int getType() const;

    const Token &getToken() const;

    void visit(Visitor *visitor) override;
private:
    Token op;
};

using OpNodePtr = shared_ptr<OpNode>;
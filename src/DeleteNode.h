//**************************************************
// File: DeleteNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the DeleteNode class, which
// represents a delete statement in the AST.
//**************************************************

#pragma once

#include "StatementNode.h"
#include "Visitor.h"
#include "Token.h"

class DeleteNode : public StatementNode
{
public:
    DeleteNode(const Token &identifier);

    virtual void visit(Visitor *visitor) override;
    
    const Token &getIdentifier() const;
    const string &getName() const;

private:
    Token identifier;
};

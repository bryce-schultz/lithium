//***********************************************
// File: VarExprNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the VarExprNode class, which represents
// a variable expression in the abstract syntax tree (AST).
//***********************************************

#include <string>

#include "VarExprNode.h"
#include "Visitor.h"

using std::string;

VarExprNode::VarExprNode(const Token &token):
    token(token)
{
    setRange(token.getRange());
}

const Token &VarExprNode::getToken() const
{
    return token;
}

const string &VarExprNode::getName() const
{
    return token.getValue();
}

bool VarExprNode::isLval() const
{
    // this node is an lvalue because it
    // represents a variable which can be assigned to
    return true;
}

void VarExprNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}
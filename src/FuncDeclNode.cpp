//***********************************************
// File: FuncDeclNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the FuncDeclNode class, which represents
// a function declaration in the abstract syntax tree (AST).
//**************************************************

#include <memory>
#include <string>

#include "FuncDeclNode.h"

FuncDeclNode::FuncDeclNode(const Token &token,
    std::shared_ptr<ParamListNode> params,
    std::shared_ptr<StatementNode> body):
    token(token),
    params(params),
    body(body)
{
    setRange(token.getRange());

    if (params)
    {
        setRangeStart(params->getRange().getStart());
    }

    if (body)
    {
        setRangeEnd(body->getRange().getEnd());
    }
}

const Token &FuncDeclNode::getToken() const
{
    return token;
}

const string &FuncDeclNode::getName() const
{
    return token.getValue();
}

std::shared_ptr<ParamListNode> FuncDeclNode::getParams() const
{
    return params;
}

std::shared_ptr<StatementNode> FuncDeclNode::getBody() const
{
    return body;
}

void FuncDeclNode::setParams(std::shared_ptr<ParamListNode> p)
{
    params = p;
}

void FuncDeclNode::setBody(std::shared_ptr<StatementNode> b)
{
    body = b;
}

bool FuncDeclNode::isConst() const
{
    return false;
}

void FuncDeclNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}

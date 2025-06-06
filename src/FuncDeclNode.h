//***********************************************
// File: FuncDeclNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the FuncDeclNode class, which represents
// a function declaration in the abstract syntax tree (AST).
//**************************************************

#pragma once

#include <memory>

#include "DeclNode.h"
#include "Token.h"
#include "ParamListNode.h"
#include "BlockNode.h"

using std::shared_ptr;
using std::string;

class FuncDeclNode : public DeclNode
{
public:
    FuncDeclNode(const Token &token, shared_ptr<ParamListNode> params, shared_ptr<StatementNode> body);

    const Token &getToken() const;

    virtual const string &getName() const override;

    shared_ptr<ParamListNode> getParams() const;

    shared_ptr<StatementNode> getBody() const;

    void setParams(shared_ptr<ParamListNode> params);

    void setBody(shared_ptr<StatementNode> body);

    virtual bool isConst() const override;

    virtual void visit(Visitor *visitor) override;
private:
    Token token;
    shared_ptr<ParamListNode> params;
    shared_ptr<StatementNode> body;
};
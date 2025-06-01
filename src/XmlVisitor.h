//**************************************************
// File: XmlVisitor.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the XmlVisitor class, which
// traverses the AST and outputs XML representation.
//**************************************************

#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include "Visitor.h"

using std::string;
using std::vector;

class XmlVisitor : public Visitor
{
public:
    virtual void visitAllChildren(Node *node) override;
    std::string getOutput() const;
public:
    void visit(StatementNode *node) override;
    void visit(StatementsNode *node) override;
    void visit(ExpressionNode *node) override;
    void visit(ReturnStatementNode *node) override;
    void visit(VarExprNode *node) override;
    void visit(VarDeclNode *node) override;
    void visit(PrintStatementNode *node) override;
    void visit(NumberNode *node) override;
    void visit(CallNode *node) override;
    void visit(BinaryExprNode *node) override;
    void visit(ArgListNode *node) override;
    void visit(OpNode *node) override;
    void visit(BlockNode *node) override;
    void visit(MemberAccessNode *node) override;
    void visit(StringNode *node) override;
    void visit(AssignNode *node) override;
    void visit(IfStatementNode *node) override;
    void visit(FuncDeclNode *node) override;
private:
    void openTag(const string &tagName, vector<string> attributes = {}, bool selfClosing = false);
    void closeTag(const string &tagName);
    std::string indentString() const;
private:
    int indent;

    std::ostringstream outputStream;
};
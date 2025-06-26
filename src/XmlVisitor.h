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
using std::ostringstream;

class XmlVisitor : public Visitor
{
public:
    virtual void visitAllChildren(Node *node) override;
    string getOutput() const;
public:
    void visit(StatementsNode *node) override;
    void visit(ReturnStatementNode *node) override;
    void visit(VarExprNode *node) override;
    void visit(VarDeclNode *node) override;
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
    void visit(WhileNode *node) override;
    void visit(ForStatementNode *node) override;
    void visit(NullNode *node) override;
    void visit(BreakNode *node) override;
private:
    void openTag(const string &tagName, vector<string> attributes = {}, bool selfClosing = false);
    void closeTag(const string &tagName);
    string indentString() const;
private:
    int indent;
    ostringstream outputStream;
};
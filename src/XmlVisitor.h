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
    void visit(BinaryExpressionNode *node) override;
    void visit(ArgListNode *node) override;
    void visit(OpNode *node) override;
    void visit(BlockNode *node) override;
private:
    void openTag(const string &tagName, vector<string> attributes = {}, bool selfClosing = false);
    void closeTag(const string &tagName);
    std::string indentString() const;
private:
    int indent;

    std::ostringstream outputStream;
};
#pragma once

#include <string>
#include <iostream>
#include <memory>

#include "Visitor.h"
#include "Environment.h"
#include "Nodes.h"
#include "Value.h"

using std::string;
using std::cout;
using std::endl;

class Interpreter : public Visitor
{
public:
    Interpreter(Environment *env = nullptr);
    virtual void visitAllChildren(Node *node) override;

    // Override visit methods for different node types
    virtual void visit(StatementNode *node) override;
    virtual void visit(StatementsNode *node) override;
    virtual void visit(PrintStatementNode *node) override;
    virtual void visit(NumberNode *node) override;
    virtual void visit(StringNode *node) override;
    virtual void visit(BinaryExpressionNode *node) override;
    virtual void visit(CallNode *node) override;
    virtual void visit(VarDeclNode *node) override;
    virtual void visit(VarExprNode *node) override;
    virtual void visit(AssignNode *node) override;
    virtual void visit(BlockNode *node) override;
    virtual void visit(IfStatementNode *node) override;
    virtual void visit(FuncDeclNode *node) override;
private:
    std::shared_ptr<Value> evalBinaryExpression(ExpressionNode *left, OpNode *opNode, ExpressionNode *right);
    std::shared_ptr<Value> evalNumericBinaryExpression(std::shared_ptr<NumberValue> left, OpNode *opNode, std::shared_ptr<NumberValue> right);
    std::shared_ptr<Value> evalStringBinaryExpression(std::shared_ptr<Value> left, OpNode *opNode, std::shared_ptr<Value> right);
    std::shared_ptr<Value> evalBooleanBinaryExpression(std::shared_ptr<Value> left, OpNode *opNode, std::shared_ptr<Value> right);

    std::shared_ptr<Value> evalUnaryExpression(ExpressionNode *expression, OpNode *opNode, bool prefix = false);
    std::shared_ptr<Value> evalNumericUnaryExpression(std::shared_ptr<NumberValue> value, OpNode *opNode, bool prefix = false);
    std::shared_ptr<Value> evalVariableUnaryExpression(VarExprNode *expression, OpNode *opNode, bool prefix = false);
private:
    Environment *env;
    std::shared_ptr<Value> returnValue;
};
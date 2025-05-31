#pragma once

class Node;
class StatementNode;
class StatementsNode;
class ExpressionNode;
class ReturnStatementNode;
class VarExprNode;
class VarDeclNode;
class PrintStatementNode;
class NumberNode;
class CallNode;
class BinaryExpressionNode;
class ArgListNode;
class OpNode;

class Visitor
{
public:
    // call this method to use the visitor pattern on a node
    virtual void visitAllChildren(Node *node) = 0;
    virtual ~Visitor() = default;

public:
    // visit methods for different node types
    virtual void visit(Node *node);
    virtual void visit(StatementNode *node);
    virtual void visit(StatementsNode *node);
    virtual void visit(ExpressionNode *node);
    virtual void visit(ReturnStatementNode *node);
    virtual void visit(VarExprNode *node);
    virtual void visit(VarDeclNode *node);
    virtual void visit(PrintStatementNode *node);
    virtual void visit(NumberNode *node);
    virtual void visit(CallNode *node);
    virtual void visit(BinaryExpressionNode *node);
    virtual void visit(ArgListNode *node);
    virtual void visit(OpNode *node);
};
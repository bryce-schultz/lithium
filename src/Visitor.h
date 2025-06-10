//**************************************************
// File: Visitor.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the Visitor base class for
// traversing AST nodes using the visitor pattern.
//**************************************************

#pragma once

class Node;
class StatementNode;
class StatementsNode;
class ExpressionNode;
class ReturnStatementNode;
class VarExprNode;
class VarDeclNode;
class NumberNode;
class CallNode;
class BinaryExprNode;
class UnaryExprNode;
class ArgListNode;
class OpNode;
class BlockNode;
class MemberAccessNode;
class ArrayAccessNode;
class StringNode;
class AssignNode;
class IfStatementNode;
class WhileNode;
class FuncDeclNode;
class ForStatementNode;
class NullNode;
class BreakNode;
class ImportNode;
class ClassNode;
class ArrayNode;

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
    virtual void visit(NumberNode *node);
    virtual void visit(CallNode *node);
    virtual void visit(BinaryExprNode *node);
    virtual void visit(UnaryExprNode *node);
    virtual void visit(ArgListNode *node);
    virtual void visit(OpNode *node);
    virtual void visit(BlockNode *node);
    virtual void visit(MemberAccessNode *node);
    virtual void visit(ArrayAccessNode *node);
    virtual void visit(StringNode *node);
    virtual void visit(AssignNode *node);
    virtual void visit(IfStatementNode *node);
    virtual void visit(WhileNode *node);
    virtual void visit(FuncDeclNode *node);
    virtual void visit(ForStatementNode *node);
    virtual void visit(NullNode *node);
    virtual void visit(BreakNode *node);
    virtual void visit(ImportNode *node);
    virtual void visit(ClassNode *node);
    virtual void visit(ArrayNode *node);
};
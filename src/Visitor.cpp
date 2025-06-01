//**************************************************
// File: Visitor.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the Visitor base class for
// traversing AST nodes using the visitor pattern.
//**************************************************

#include "Visitor.h"

#include "Nodes.h"
#include "XmlVisitor.h"

void Visitor::visit(Node *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(StatementNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(StatementsNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(ExpressionNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(ReturnStatementNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(VarExprNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(VarDeclNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(PrintStatementNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(NumberNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(CallNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(BinaryExprNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(ArgListNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(OpNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(BlockNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(MemberAccessNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(StringNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(AssignNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(IfStatementNode *node)
{
    node->visitAllChildren(this);
}

void Visitor::visit(FuncDeclNode *node)
{
    node->visitAllChildren(this);
}

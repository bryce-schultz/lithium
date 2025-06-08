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
#include "Utils.h"

void Visitor::visit(Node *node)
{
    UNUSED(node);
}

void Visitor::visit(StatementNode *node)
{
    UNUSED(node);
}

void Visitor::visit(StatementsNode *node)
{
    for (const auto &statement : node->getStatements())
    {
        if (statement)
        {
            statement->visit(this);
        }
    }
}

void Visitor::visit(ExpressionNode *node)
{
    UNUSED(node);
}

void Visitor::visit(ReturnStatementNode *node)
{
    UNUSED(node);
}

void Visitor::visit(VarExprNode *node)
{
    UNUSED(node);
}

void Visitor::visit(VarDeclNode *node)
{
    UNUSED(node);
}

void Visitor::visit(PrintStatementNode *node)
{
    UNUSED(node);
}

void Visitor::visit(NumberNode *node)
{
    UNUSED(node);
}

void Visitor::visit(CallNode *node)
{
    UNUSED(node);
}

void Visitor::visit(BinaryExprNode *node)
{
    UNUSED(node);
}

void Visitor::visit(ArgListNode *node)
{
    UNUSED(node);
}

void Visitor::visit(OpNode *node)
{
    UNUSED(node);
}

void Visitor::visit(BlockNode *node)
{
    UNUSED(node);
}

void Visitor::visit(MemberAccessNode *node)
{
    UNUSED(node);
}

void Visitor::visit(StringNode *node)
{
    UNUSED(node);
}

void Visitor::visit(AssignNode *node)
{
    UNUSED(node);
}

void Visitor::visit(IfStatementNode *node)
{
    UNUSED(node);
}

void Visitor::visit(WhileNode *node)
{
    UNUSED(node);
}

void Visitor::visit(FuncDeclNode *node)
{
    UNUSED(node);
}

void Visitor::visit(ForStatementNode *node)
{
    UNUSED(node);
}

void Visitor::visit(NullNode *node)
{
    UNUSED(node);
}

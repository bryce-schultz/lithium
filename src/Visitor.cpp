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

void Visitor::visit(ArgListNode *node)
{
    for (const auto &arg : node->getArgs())
    {
        if (arg)
        {
            arg->visit(this);
        }
    }
}

void Visitor::visit(ArrayAccessNode *node)
{
    if (node->getArray())
    {
        node->getArray()->visit(this);
    }
    if (node->getIndex())
    {
        node->getIndex()->visit(this);
    }
}

void Visitor::visit(ArrayNode *node)
{
    for (const auto &element : node->getElements())
    {
        if (element)
        {
            element->visit(this);
        }
    }
}

void Visitor::visit(AssignNode *node)
{
    if (node->getAsignee())
    {
        node->getAsignee()->visit(this);
    }
    if (node->getExpr())
    {
        node->getExpr()->visit(this);
    }
}

void Visitor::visit(BinaryExprNode *node)
{
    if (node->getLeft())
    {
        node->getLeft()->visit(this);
    }
    if (node->getOperator())
    {
        node->getOperator()->visit(this);
    }
    if (node->getRight())
    {
        node->getRight()->visit(this);
    }
}

void Visitor::visit(BlockNode *node)
{
    if (node->getStatements())
    {
        node->getStatements()->visit(this);
    }
}

void Visitor::visit(BreakNode *node)
{
    UNUSED(node);
}

void Visitor::visit(CallNode *node)
{
    if (node->getCallee())
    {
        node->getCallee()->visit(this);
    }
    if (node->getArgs())
    {
        node->getArgs()->visit(this);
    }
}

void Visitor::visit(ClassNode *node)
{
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }
}

void Visitor::visit(ContinueNode *node)
{
    UNUSED(node);
}

void Visitor::visit(ExpressionNode *node)
{
    UNUSED(node);
}

void Visitor::visit(ForEachNode *node)
{
    if (node->getKeyDecl())
    {
        node->getKeyDecl()->visit(this);
    }
    if (node->getValueDecl())
    {
        node->getValueDecl()->visit(this);
    }
    if (node->getIterable())
    {
        node->getIterable()->visit(this);
    }
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }
}

void Visitor::visit(ForStatementNode *node)
{
    if (node->getInit())
    {
        node->getInit()->visit(this);
    }
    if (node->getCondition())
    {
        node->getCondition()->visit(this);
    }
    if (node->getIncrement())
    {
        node->getIncrement()->visit(this);
    }
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }
}

void Visitor::visit(FuncDeclNode *node)
{
    if (node->getParams())
    {
        node->getParams()->visit(this);
    }
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }
}

void Visitor::visit(IfStatementNode *node)
{
    if (node->getCondition())
    {
        node->getCondition()->visit(this);
    }
    if (node->getThenBranch())
    {
        node->getThenBranch()->visit(this);
    }
    if (node->getElseBranch())
    {
        node->getElseBranch()->visit(this);
    }
}

void Visitor::visit(ImportNode *node)
{
    UNUSED(node);
}

void Visitor::visit(MemberAccessNode *node)
{
    if (node->getExpression())
    {
        node->getExpression()->visit(this);
    }
}

void Visitor::visit(Node *node)
{
    UNUSED(node);
}

void Visitor::visit(NullNode *node)
{
    UNUSED(node);
}

void Visitor::visit(NumberNode *node)
{
    UNUSED(node);
}

void Visitor::visit(OpNode *node)
{
    UNUSED(node);
}

void Visitor::visit(ReturnStatementNode *node)
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

void Visitor::visit(StringNode *node)
{
    UNUSED(node);
}

void Visitor::visit(UnaryExprNode *node)
{
    if (node->getOperator())
    {
        node->getOperator()->visit(this);
    }
    if (node->getExpression())
    {
        node->getExpression()->visit(this);
    }
}

void Visitor::visit(VarDeclNode *node)
{
    UNUSED(node);
}

void Visitor::visit(VarExprNode *node)
{
    UNUSED(node);
}

void Visitor::visit(WhileNode *node)
{
    if (node->getCondition())
    {
        node->getCondition()->visit(this);
    }
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }
}
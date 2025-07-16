//**************************************************
// File: XmlVisitor.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the XmlVisitor class, which
// traverses the AST and generates XML output.
//**************************************************

#include "XmlVisitor.h"
#include "Nodes.h"
#include "Utils.h"

void XmlVisitor::visitAllChildren(Node *node)
{
    indent = 0; // Reset indentation for each new root node
    outputStream.clear(); // Clear any error flags on the stream
    node->visit(this);
}

std::string XmlVisitor::getOutput() const
{
    return outputStream.str();
}


void XmlVisitor::openTag(const string &tagName, vector<string> attributes, bool selfClosing)
{
    outputStream << indentString() << "<" << tagName;
    for (const auto &attr : attributes)
    {
        outputStream << " " << attr;
    }
    if (selfClosing)
    {
        outputStream << " />\n";
    }
    else
    {
        outputStream << ">\n";
        indent++;
    }
}

void XmlVisitor::closeTag(const string &tagName)
{
    indent--;
    outputStream << indentString() << "</" << tagName << ">\n";
}

std::string XmlVisitor::indentString() const
{
    return std::string(indent * 2, ' ');
}

void XmlVisitor::visit(StatementsNode *node)
{
    openTag("Statements");
    for (const auto &statement : node->getStatements())
    {
        if (statement)
        {
            statement->visit(this);
        }
    }
    closeTag("Statements");
}

void XmlVisitor::visit(ReturnStatementNode *node)
{
    openTag("ReturnStatement");
    if (node->getExpression())
    {
        node->getExpression()->visit(this);
    }
    closeTag("ReturnStatement");
}

void XmlVisitor::visit(VarExprNode *node)
{
    openTag("VarExpr", {"name=\"" + node->getToken().getValue() + "\""}, true);
}

void XmlVisitor::visit(VarDeclNode *node)
{
    openTag("VarDecl", {
        "name=\"" + node->getToken().getValue() + "\"",
        "is_const=\"" + std::string(node->isConst() ? "true" : "false") + "\""
    }, false);
    if (node->getExpr())
    {
        node->getExpr()->visit(this);
    }
    closeTag("VarDecl");
}

void XmlVisitor::visit(NumberNode *node)
{
    openTag("Number", {"value=\"" + std::to_string(node->getValue()) + "\""}, true);
}

void XmlVisitor::visit(CallNode *node)
{
    openTag("Call");
    if (node->getCallee())
    {
        node->getCallee()->visit(this);
    }
    if (node->getArgs())
    {
        node->getArgs()->visit(this);
    }
    closeTag("Call");
}

void XmlVisitor::visit(BinaryExprNode *node)
{
    if (node->isUnary())
    {
        openTag("UnaryExpression", {"prefix=\"" + std::string(node->isPrefix() ? "true" : "false") + "\""});
        node->getRight()->visit(this);
        node->getOperator()->visit(this);
        closeTag("UnaryExpression");
    }
    else
    {
        openTag("BinaryExpression", {"left_is_lval=\"" + std::string(node->getLeft()->isLval() ? "true" : "false") + "\""});
        node->getLeft()->visit(this);
        node->getOperator()->visit(this);
        node->getRight()->visit(this);
        closeTag("BinaryExpression");
    }
}

void XmlVisitor::visit(BooleanNode *node)
{
    openTag("Boolean", {"value=\"" + std::string(node->getValue() ? "true" : "false") + "\""}, true);
}

void XmlVisitor::visit(ArgListNode *node)
{
    openTag("ArgList");
    for (const auto &arg : node->getArgs())
    {
        if (arg)
        {
            arg->visit(this);
        }
    }
    closeTag("ArgList");
}

void XmlVisitor::visit(OpNode *node)
{
    openTag("Op", {"type=\"" + Token::tokenTypeToString(node->getType()) + "\""}, true);
}

void XmlVisitor::visit(BlockNode *node)
{
    openTag("Block");
    if (node->getStatements())
    {
        node->getStatements()->visit(this);
    }
    closeTag("Block");
}

void XmlVisitor::visit(MemberAccessNode *node)
{
    openTag("MemberAccess", {"identifier=\"" + node->getIdentifier().getValue() + "\""}, false);
    if (node->getExpression())
    {
        node->getExpression()->visit(this);
    }
    closeTag("MemberAccess");
}

void XmlVisitor::visit(AssignNode *node)
{
    openTag("Assign");
    if (node->getAsignee())
    {
        node->getAsignee()->visit(this);
    }
    if (node->getExpr())
    {
        node->getExpr()->visit(this);
    }
    closeTag("Assign");
}

void XmlVisitor::visit(StringNode *node)
{
    openTag("String", {"value=\"" + node->getValue() + "\""}, true);
}

void XmlVisitor::visit(IfStatementNode *node)
{
    openTag("IfStatement");
    if (node->getCondition())
    {
        node->getCondition()->visit(this);
    }
    if (node->getThenBranch())
    {
        openTag("ThenBranch");
        node->getThenBranch()->visit(this);
        closeTag("ThenBranch");
    }
    if (node->getElseBranch())
    {
        openTag("ElseBranch");
        node->getElseBranch()->visit(this);
        closeTag("ElseBranch");
    }
    closeTag("IfStatement");
}

void XmlVisitor::visit(FuncDeclNode *node)
{
    openTag("FuncDecl", {"name=\"" + node->getName() + "\""}, false);
    if (node->getParams())
    {
        node->getParams()->visit(this);
    }
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }
    closeTag("FuncDecl");
}

void XmlVisitor::visit(WhileNode *node)
{
    openTag("WhileStatement");
    if (node->getCondition())
    {
        node->getCondition()->visit(this);
    }
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }
    closeTag("WhileStatement");
}

void XmlVisitor::visit(ForStatementNode *node)
{
    openTag("ForStatement");
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
    closeTag("ForStatement");
}

void XmlVisitor::visit(NullNode *node)
{
    UNUSED(node);
    openTag("Null", {}, true);
}

void XmlVisitor::visit(BreakNode *node)
{
    UNUSED(node);
    openTag("Break", {}, true);
}

void XmlVisitor::visit(DeleteNode *node)
{
    openTag("Delete", {"identifier=\"" + node->getName() + "\""}, true);
}
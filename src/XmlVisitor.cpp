#include "XmlVisitor.h"

#include "Nodes.h"

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

void XmlVisitor::visit(StatementNode *node)
{
    openTag("Statement");
    node->visitAllChildren(this);
    closeTag("Statement");
}

void XmlVisitor::visit(StatementsNode *node)
{
    openTag("Statements");
    node->visitAllChildren(this);
    closeTag("Statements");
}

void XmlVisitor::visit(ExpressionNode *node)
{
    openTag("Expression");
    node->visitAllChildren(this);
    closeTag("Expression");
}

void XmlVisitor::visit(ReturnStatementNode *node)
{
    openTag("ReturnStatement");
    node->visitAllChildren(this);
    closeTag("ReturnStatement");
}

void XmlVisitor::visit(VarExprNode *node)
{
    openTag("VarExpr", {"name=\"" + node->getToken().getValue() + "\""}, true);
}

void XmlVisitor::visit(VarDeclNode *node)
{
    openTag("VarDecl", {"name=\"" + node->getToken().getValue() + "\""}, false);
    node->visitAllChildren(this);
    closeTag("VarDecl");
}

void XmlVisitor::visit(PrintStatementNode *node)
{
    openTag("PrintStatement");
    node->visitAllChildren(this);
    closeTag("PrintStatement");
}

void XmlVisitor::visit(NumberNode *node)
{
    openTag("Number", {"value=\"" + std::to_string(node->getValue()) + "\""}, true);
}

void XmlVisitor::visit(CallNode *node)
{
    openTag("Call");
    node->visitAllChildren(this);
    closeTag("Call");
}

void XmlVisitor::visit(BinaryExpressionNode *node)
{
    openTag("BinaryExpression");
    node->visitAllChildren(this);
    closeTag("BinaryExpression");
}

void XmlVisitor::visit(ArgListNode *node)
{
    openTag("ArgList");
    node->visitAllChildren(this);
    closeTag("ArgList");
}

void XmlVisitor::visit(OpNode *node)
{
    openTag("Op", {"type=\"" + std::to_string(node->getType()) + "\""}, true);
}
#include "SemanticErrorVisitor.h"
#include "Nodes.h"
#include "Error.h"

// define a custom error macro for semantic errors
#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__); \
    errorCount++

SemanticErrorVisitor::SemanticErrorVisitor():
    errorCount(0),
    functionDepth(0)
{
}

void SemanticErrorVisitor::visitAllChildren(Node *node)
{
    node->visit(this);
}

bool SemanticErrorVisitor::hasErrors() const
{
    return errorCount > 0;
}

void SemanticErrorVisitor::visit(FuncDeclNode *node)
{
    functionDepth++;
    node->getBody()->visit(this);
    functionDepth--;
}

void SemanticErrorVisitor::visit(ReturnStatementNode *node)
{
    if (functionDepth == 0)
    {
        error("return statement outside of a function", node->getRange());
    }
}
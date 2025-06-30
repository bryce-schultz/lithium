#include "SemanticErrorVisitor.h"
#include "Nodes.h"
#include "Error.h"

// define a custom error macro for semantic errors
#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__); \
    errorCount++

#define errorAt(msg, location, range) \
    locationRangeError(msg, location, range, __FILE__, __LINE__); \
    errorCount++;

SemanticErrorVisitor::SemanticErrorVisitor():
    errorCount(0),
    functionDepth(0),
    loopDepth(0),
    blockDepth(0),
    currentFunctionName("")
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

    // Track function name for recursion detection
    std::string prevFunctionName = currentFunctionName;
    currentFunctionName = node->getName();

    // Check for immediate self-recursion in function body
    if (currentFunctionStack.find(currentFunctionName) != currentFunctionStack.end())
    {
        // This would be detected during call analysis, but let's be extra safe
        // error("potential infinite recursion detected in function '" + currentFunctionName + "'", node->getRange());
    }

    currentFunctionStack.insert(currentFunctionName);

    // Check if function has a body
    if (!node->getBody()) {
        errorAt("function '" + currentFunctionName + "' has no body", node->getToken().getRange().getStart(), node->getRange());
    } else {
        node->getBody()->visit(this);
    }

    currentFunctionStack.erase(currentFunctionName);
    currentFunctionName = prevFunctionName;
    functionDepth--;
}

void SemanticErrorVisitor::visit(ReturnStatementNode *node)
{
    if (functionDepth == 0)
    {
        error("'return' used outside of a function", node->getRange());
    }
}

void SemanticErrorVisitor::visit(ForStatementNode *node)
{
    loopDepth++;
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
    loopDepth--;
}

void SemanticErrorVisitor::visit(WhileNode *node)
{
    loopDepth++;
    if (node->getCondition())
    {
        node->getCondition()->visit(this);
    }
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }
    loopDepth--;
}

void SemanticErrorVisitor::visit(BreakNode *node)
{
    if (loopDepth == 0)
    {
        error("'break' used outside of a loop", node->getRange());
    }
}

void SemanticErrorVisitor::visit(CallNode *node)
{
    // visit arguments and callee as normal
    if (node->getArgs())
    {
        for (auto &arg : node->getArgs()->getArgs())
        {
            arg->visit(this);
        }
    }
    if (node->getCallee())
        node->getCallee()->visit(this);
}

void SemanticErrorVisitor::visit(ClassNode *node)
{
    // Check if class is defined inside a block or function
    if (blockDepth > 0 || functionDepth > 0)
    {
        errorAt("class '" + node->getName() + "' must be declared at global scope", node->getToken().getRange().getStart(), node->getRange());
    }

    // Track that this class is declared
    declaredClasses.insert(node->getName());

    // Visit the class body
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }
}

void SemanticErrorVisitor::visit(BlockNode *node)
{
    blockDepth++;

    // Visit all statements in the block
    Visitor::visit(node);

    blockDepth--;
}

void SemanticErrorVisitor::visit(AssignNode *node)
{
    // For assignment like "let x = Test", track what's being assigned
    if (node->getExpr())
    {
        node->getExpr()->visit(this);
    }

    // Track variable declarations
    if (auto varExpr = dynamic_cast<VarExprNode*>(node->getAsignee().get()))
    {
        declaredVariables.insert(varExpr->getName());
    }
}

void SemanticErrorVisitor::visit(MemberAccessNode *node)
{
    // visit the object being accessed
    if (!node->getExpression())
        return;
    node->getExpression()->visit(this);
    // for class instances and dynamic types, member checks must be done at runtime
}

void SemanticErrorVisitor::visit(ArrayAccessNode *node)
{
    // Visit the array and index expressions
    if (node->getArray())
    {
        node->getArray()->visit(this);
    }

    if (node->getIndex())
    {
        node->getIndex()->visit(this);
    }
}
#include "SemanticErrorVisitor.h"
#include "Nodes.h"
#include "Error.h"

// define a custom error macro for semantic errors
#define error(msg, range) \
    rangeError(msg, range, __FILE__, __LINE__); \
    errorCount++

SemanticErrorVisitor::SemanticErrorVisitor():
    errorCount(0),
    functionDepth(0),
    loopDepth(0),
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
    
    if (node->getBody()) {
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
    // First visit the callee to see what we're trying to call
    auto calleeNode = node->getCallee();
    
    // Check if it's a simple variable call (most common case for direct recursion)
    if (auto varExpr = dynamic_cast<VarExprNode*>(calleeNode.get()))
    {
        std::string calleeName = varExpr->getName();
        
        // Check for direct self-recursion
        if (calleeName == currentFunctionName && functionDepth > 0)
        {
            // This is a direct recursive call - warn but don't error (recursion can be valid)
            // We could add a warning system later, for now just track it
        }
        
        // Check if trying to call a class without proper instantiation
        if (declaredClasses.find(calleeName) != declaredClasses.end())
        {
            // This is calling a class - warn that it might not have a constructor
            // For now, we can't determine if it has a constructor without more analysis
            // but we can at least flag it for review
        }
        
        // Check for mutual recursion patterns (simple case: A calls B, B calls A)
        // This is more complex to detect perfectly, but we can catch simple cases
    }
    
    // Visit arguments and callee as normal
    if (node->getArgs())
    {
        for (auto &arg : node->getArgs()->getArgs())
        {
            arg->visit(this);
        }
    }
    calleeNode->visit(this);
}

void SemanticErrorVisitor::visit(VarExprNode *node)
{
    // This is where we could add undefined variable checking
    // For now, just let the default visitor handle it
}

void SemanticErrorVisitor::visit(ClassNode *node)
{
    // Track that this class is declared
    declaredClasses.insert(node->getName());
    
    // Visit the class body
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }
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
    // Visit the object being accessed
    if (node->getExpression())
    {
        node->getExpression()->visit(this);
    }
    
    // We could add type checking here if we had type information
    // For now, just ensure we visit all children
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
    
    // We could add bounds checking and type validation here
}
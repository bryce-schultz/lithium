#include "SemanticErrorVisitor.h"
#include "Nodes.h"
#include "Error.h"
#include "Utils.h"

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

void SemanticErrorVisitor::resetErrorCount()
{
    errorCount = 0;
}

void SemanticErrorVisitor::visit(FuncDeclNode *node)
{
    functionDepth++;

    std::string funcName = node->getName();

    // Check for function redeclaration in the same immediate scope
    if (localFunctions.find(funcName) != localFunctions.end())
    {
        errorAt("function '" + funcName + "' is already declared in this scope", node->getToken().getRange().getStart(), node->getRange());
    }

    // Track this function in the current scope
    localFunctions.insert(funcName);
    declaredFunctions.insert(funcName);

    // Track function name for recursion detection
    std::string prevFunctionName = currentFunctionName;
    currentFunctionName = funcName;

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
        // Create new scope for function body
        auto prevLocalVars = localVariables;
        auto prevLocalFuncs = localFunctions;

        // Clear local scope trackers for the function body
        localVariables.clear();
        localFunctions.clear();

        // Parameters are in the function's local scope
        if (node->getParams()) {
            for (int i = 0; i < node->getParams()->getParamCount(); i++) {
                std::string paramName = node->getParams()->getParam(i)->getName();
                localVariables.insert(paramName);
            }
        }

        node->getBody()->visit(this);

        // Restore previous scope
        localVariables = prevLocalVars;
        localFunctions = prevLocalFuncs;
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

    // Create new scope for the for loop (including the loop variable)
    auto prevLocalVars = localVariables;
    auto prevLocalFuncs = localFunctions;

    // Clear local scope for the for loop
    localVariables.clear();
    localFunctions.clear();

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

    // Restore previous scope
    localVariables = prevLocalVars;
    localFunctions = prevLocalFuncs;

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

void SemanticErrorVisitor::visit(ContinueNode *node)
{
    if (loopDepth == 0)
    {
        error("'continue' used outside of a loop", node->getRange());
    }
}

void SemanticErrorVisitor::visit(ForEachNode *node)
{
    loopDepth++;

    // Create new scope for the foreach loop
    auto prevLocalVars = localVariables;
    auto prevLocalFuncs = localFunctions;

    // Clear local scope for the foreach loop
    localVariables.clear();
    localFunctions.clear();

    // Visit the iterable expression
    if (node->getIterable())
    {
        node->getIterable()->visit(this);
    }

    // Visit the body
    if (node->getBody())
    {
        node->getBody()->visit(this);
    }

    // Restore previous scope
    localVariables = prevLocalVars;
    localFunctions = prevLocalFuncs;

    loopDepth--;
}

void SemanticErrorVisitor::visit(CallNode *node)
{
    // Check for excessive recursion depth in static analysis
    if (currentFunctionStack.size() > 50)  // Arbitrary deep recursion threshold
    {
        error("potential stack overflow: excessive recursion depth detected", node->getRange());
    }

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

    // Check for class redeclaration
    std::string className = node->getName();
    if (declaredClasses.find(className) != declaredClasses.end())
    {
        errorAt("class '" + className + "' is already declared", node->getToken().getRange().getStart(), node->getRange());
    }

    // Track that this class is declared
    declaredClasses.insert(className);

    // Visit the class body with fresh scope
    if (node->getBody())
    {
        auto prevLocalVars = localVariables;
        auto prevLocalFuncs = localFunctions;

        node->getBody()->visit(this);

        localVariables = prevLocalVars;
        localFunctions = prevLocalFuncs;
    }
}

void SemanticErrorVisitor::visit(BlockNode *node)
{
    blockDepth++;

    // Create new scope for block-level variables and functions
    auto prevLocalVars = localVariables;
    auto prevLocalFuncs = localFunctions;

    // Start with fresh local scope for this block
    localVariables.clear();
    localFunctions.clear();

    // Visit all statements in the block
    Visitor::visit(node);

    // Restore previous scope
    localVariables = prevLocalVars;
    localFunctions = prevLocalFuncs;

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

    // Note: Array bounds checking must be done at runtime
    // but we could add static analysis for obvious cases like negative literals
}

void SemanticErrorVisitor::visit(VarDeclNode *node)
{
    std::string varName = node->getName();

    // Note: Variable redeclaration checking moved to runtime (Environment::declare)
    // since semantic analysis can't detect runtime errors that prevent declaration
    
    // Track this variable in the current scope
    localVariables.insert(varName);
    declaredVariables.insert(varName);

    // Visit the initializer expression if present
    if (node->getExpr())
    {
        node->getExpr()->visit(this);
    }
}

void SemanticErrorVisitor::visit(ImportNode *node)
{
    // Check if import is at global scope
    if (functionDepth > 0 || blockDepth > 0)
    {
        error("import statements must be at global scope", node->getRange());
    }

    // Track imported modules to prevent duplicates
    std::string moduleName = node->getModuleName();
    if (importedModules.find(moduleName) != importedModules.end())
    {
        error("module '" + moduleName + "' is already imported", node->getRange());
    }

    importedModules.insert(moduleName);
    importDepth++;

    importDepth--;
}

void SemanticErrorVisitor::visit(DeleteNode *node)
{
    // Basic semantic analysis for delete statements
    // Note: Most delete validation happens at runtime in the interpreter
    
    // For now, we don't have specific semantic errors to check for delete
    // Future enhancements could include:
    // - Static analysis of whether variable exists in current scope
    // - Warning about deleting variables that might be used later
    // - Check for deleting constants (though this is also a runtime check)
    
    UNUSED(node);
}
//**************************************************
// File: StatementsNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the StatementsNode class,
// representing a sequence of statements in the AST.
//**************************************************

#include "StatementsNode.h"

StatementsNode::StatementsNode(StatementNode *statement)
{
    if (statement) 
    {
        addChild(statement);
        // set the range to the statement's range if it exists
        setRange(statement->getRange());
    }
}

void StatementsNode::addStatement(StatementNode *statement)
{
    if (!statement) return; 
    
    addChild(statement);
    
    if (getChildCount() == 1)
    {
        // if this is the first statement, set the range to its range
        setRange(statement->getRange());
    } 
    else 
    {
        // update the range to include the new statement
        setRangeEnd(statement->getRange().getEnd());
    }
}

void StatementsNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}
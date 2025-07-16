//**************************************************
// File: DeleteNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the DeleteNode class, which
// represents a delete statement in the AST.
//**************************************************

#include "DeleteNode.h"

DeleteNode::DeleteNode(const Token &identifier) : identifier(identifier)
{
    setRangeStart(identifier.getRange().getStart());
    setRangeEnd(identifier.getRange().getEnd());
}

void DeleteNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}

const Token &DeleteNode::getIdentifier() const
{
    return identifier;
}

const string &DeleteNode::getName() const
{
    return identifier.getValue();
}

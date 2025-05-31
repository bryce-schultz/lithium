//**************************************************
// File: ArgListNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the ArgListNode class, which
// represents a list of arguments in function calls.
//**************************************************

#pragma once

#include "ExpressionNode.h"

class ArgListNode : public Node
{
public:
    ArgListNode(ExpressionNode *arg = nullptr);

    void addArg(ExpressionNode *arg);
    void addAllArgs(ArgListNode *args);
    ExpressionNode *getArg(int index) const;
    int getArgCount() const;
    void visit(Visitor *visitor) override;
};
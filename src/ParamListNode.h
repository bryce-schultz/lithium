//***********************************************
// File: ParamListNode.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the ParamListNode class, which represents
// a list of parameters in function declarations.
//***********************************************

#pragma once

#include <memory>
#include <vector>

#include "VarDeclNode.h"
#include "Node.h"

using std::shared_ptr;
using std::vector;
using std::string;

class ParamListNode : public Node
{
public:
    using Ptr = shared_ptr<ParamListNode>;
public:
    ParamListNode(shared_ptr<VarDeclNode> param = nullptr);

    void addParam(shared_ptr<VarDeclNode> param);

    void addAllParams(shared_ptr<ParamListNode> params);

    shared_ptr<VarDeclNode> getParam(int index) const;

    int getParamCount() const;

    void visit(Visitor *visitor) override;
private:
    vector<shared_ptr<VarDeclNode>> parameters;
};

using ParamListNodePtr = shared_ptr<ParamListNode>;
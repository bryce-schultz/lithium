//***********************************************
// File: ParamListNode.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the ParamListNode class, which represents
// a list of parameters in function declarations.
//***********************************************

#include <memory>
#include <vector>
#include <string>

#include "ParamListNode.h"

ParamListNode::ParamListNode(std::shared_ptr<VarDeclNode> param)
{
    if (param)
    {
        addParam(param);
    }
}

void ParamListNode::addParam(shared_ptr<VarDeclNode> param)
{
    if (param)
    {
        parameters.push_back(param);
        if (parameters.size() == 1)
        {
            setRangeStart(param->getRange().getStart());
        }
        setRangeEnd(param->getRange().getEnd());
    }
}

void ParamListNode::addAllParams(shared_ptr<ParamListNode> params)
{
    if (params)
    {
        for (const auto &param : params->parameters)
        {
            addParam(param);
        }
    }
}

shared_ptr<VarDeclNode> ParamListNode::getParam(int index) const
{
    if (index < 0 || index >= static_cast<int>(parameters.size()))
    {
        return nullptr;
    }
    return parameters[index];
}

int ParamListNode::getParamCount() const
{
    return static_cast<int>(parameters.size());
}

void ParamListNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}

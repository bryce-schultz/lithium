#include "ParamListNode.h"

ParamListNode::ParamListNode(VarDeclNode *param)
{
    if (param)
    {
        addParam(param);
        setRangeStart(param->getRange().getStart());
    }
}

void ParamListNode::addParam(VarDeclNode *param)
{
    if (param)
    {
        addChild(param);
        setRangeEnd(param->getRange().getEnd());
    }
}

void ParamListNode::addAllParams(ParamListNode *params)
{
    if (!params) return;

    for (int i = 0; i < params->getParamCount(); ++i)
    {
        VarDeclNode *param = params->getParam(i);
        if (param)
        {
            addParam(param);
            setRangeEnd(param->getRange().getEnd());
        }
    }
}

VarDeclNode *ParamListNode::getParam(int index) const
{
    if (index < 0 || index >= getChildCount())
    {
        return nullptr;
    }
    return dynamic_cast<VarDeclNode*>(getChild(index));
}

int ParamListNode::getParamCount() const
{
    return getChildCount();
}

void ParamListNode::visit(Visitor *visitor)
{
    visitor->visit(this);
}

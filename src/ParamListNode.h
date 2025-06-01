#pragma once

#include "VarDeclNode.h"
#include "Node.h"

class ParamListNode : public Node
{
public:
    ParamListNode(VarDeclNode *param = nullptr);

    void addParam(VarDeclNode *param);
    void addAllParams(ParamListNode *params);
    VarDeclNode *getParam(int index) const;
    int getParamCount() const;

    virtual void visit(Visitor *visitor) override;
};
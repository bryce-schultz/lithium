//**************************************************
// File: Nodes.h
//
// Author: Bryce Schultz
//
// Purpose: Aggregates all AST node class headers
// for convenient inclusion in the parser and visitors.
//**************************************************

#pragma once

#include "Node.h"
#include "ExpressionNode.h"
#include "NumberNode.h"
#include "StringNode.h"
#include "VarExprNode.h"
#include "OpNode.h"
#include "BinaryExpressionNode.h"
#include "AssignNode.h"
#include "CallNode.h"
#include "ArgListNode.h"
#include "ParamListNode.h"
#include "MemberAccessNode.h"

#include "StatementNode.h"
#include "ReturnStatementNode.h"
#include "PrintStatementNode.h"
#include "VarDeclNode.h"
#include "DeclNode.h"
#include "BlockNode.h"
#include "StatementsNode.h"
#include "IfStatementNode.h"
#include "FuncDeclNode.h"
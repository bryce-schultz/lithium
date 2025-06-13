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
#include "BinaryExprNode.h"
#include "UnaryExprNode.h"
#include "AssignNode.h"
#include "CallNode.h"
#include "ArgListNode.h"
#include "ArrayNode.h"
#include "ParamListNode.h"
#include "MemberAccessNode.h"
#include "ArrayAccessNode.h"
#include "NullNode.h"

#include "StatementNode.h"
#include "ReturnStatementNode.h"
#include "VarDeclNode.h"
#include "DeclNode.h"
#include "BlockNode.h"
#include "StatementsNode.h"
#include "IfStatementNode.h"
#include "WhileNode.h"
#include "ForStatementNode.h"
#include "FuncDeclNode.h"
#include "BreakNode.h"
#include "ImportNode.h"
#include "ClassNode.h"
#include "ContinueNode.h"

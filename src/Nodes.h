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

// expression nodes
#include "ArgListNode.h"
#include "ArrayAccessNode.h"
#include "ArrayNode.h"
#include "AssignNode.h"
#include "BinaryExprNode.h"
#include "BooleanNode.h"
#include "CallNode.h"
#include "ExpressionNode.h"
#include "MemberAccessNode.h"
#include "NullNode.h"
#include "NumberNode.h"
#include "OpNode.h"
#include "ParamListNode.h"
#include "StringNode.h"
#include "UnaryExprNode.h"
#include "VarExprNode.h"

// statement nodes
#include "AssertNode.h"
#include "BlockNode.h"
#include "BreakNode.h"
#include "ClassNode.h"
#include "ContinueNode.h"
#include "DeclNode.h"
#include "DeleteNode.h"
#include "ForEachNode.h"
#include "ForStatementNode.h"
#include "FuncDeclNode.h"
#include "IfStatementNode.h"
#include "ImportNode.h"
#include "ReturnStatementNode.h"
#include "StatementNode.h"
#include "StatementsNode.h"
#include "VarDeclNode.h"
#include "WhileNode.h"

//**************************************************
// File: Nodes.h
//
// Author: Bryce Schultz
//
// Purpose: Aggregates all AST node class headers
// for convenient inclusion in the parser and visitors.
//**************************************************

#pragma once

#include <memory>

#include "Node.h"
#include "ExpressionNode.h"
#include "NumberNode.h"
#include "StringNode.h"
#include "VarExprNode.h"
#include "OpNode.h"
#include "BinaryExprNode.h"
#include "AssignNode.h"
#include "CallNode.h"
#include "ArgListNode.h"
#include "ParamListNode.h"
#include "MemberAccessNode.h"
#include "NullNode.h"

#include "StatementNode.h"
#include "ReturnStatementNode.h"
#include "PrintStatementNode.h"
#include "VarDeclNode.h"
#include "DeclNode.h"
#include "BlockNode.h"
#include "StatementsNode.h"
#include "IfStatementNode.h"
#include "WhileNode.h"
#include "ForStatementNode.h"
#include "FuncDeclNode.h"

// All AST node classes will now use std::shared_ptr for children and relationships.

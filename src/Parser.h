//**************************************************
// File: Parser.h
//
// Author: Bryce Schultz
//
// Purpose: Declares the Parser class, which parses
// tokens into an abstract syntax tree (AST).
//**************************************************

#pragma once

#include <memory>
#include <string>
#include <istream>
#include <set>

#include "Tokenizer.h"
#include "Nodes.h"
#include "Result.h"

using std::string;
using std::istream;
using std::set;
using std::shared_ptr;
using std::make_shared;

class Parser
{
public:
    Parser();
    Result<Node> parse(const string &input, const string &filename = "");
    Result<Node> parseFromPosition(const string &input, size_t startPos, const string &filename = "");
private:
    Token peekToken() const;
    void advanceToken();

    bool inSet(const Token &token, const set<int> &firstSet) const;
private:
    //******************************************************************************************
    // Statements
    //******************************************************************************************

    // stmts -> stmt stmts
    //        | ε
    Result<StatementsNode> parseStmts();

    // stmt -> assertStmt
    //       | exprStmt
    //       | forStmt
    //       | whileStmt
    //       | ifStmt
    //       | block
    //       | funcDecl
    //       | classDecl
    //       | returnStmt
    //       | breakStmt
    //       | continueStmt
    //       | deleteStmt
    //       | importStmt
    Result<StatementNode> parseStmt();

    // assertStmt -> ASSERT expr ;
    //             | ASSERT expr , expr ;
    Result<AssertNode> parseAssert();

    // exprStmt -> expr ;
    //           | letStmt
    //           | constStmt
    //           | ;
    Result<StatementNode> parseExprStmt();

    // forStmt -> FOR ( exprStmt exprStmt expr ) stmt
    Result<ForStatementNode> parseForStmt();

    // forEachStmt -> FOREACH ( IDENT : expr ) stmt
    //              | FOREACH ( IDENT , IDENT : expr ) stmt
    Result<ForEachNode> parseForEachStmt();

    // whileStmt -> WHILE ( expr ) stmt
    Result<WhileNode> parseWhileStmt();

    // ifStmt -> IF ( expr ) stmt
    //         | IF ( expr ) stmt ELSE stmt
    Result<IfStatementNode> parseIfStmt();

    // block -> { stmts }
    //        | { }
    Result<BlockNode> parseBlock();

    // funcDecl -> FN IDENT ( paramList ) stmt;
    //           | FN IDENT ( ) stmt;
    Result<FuncDeclNode> parseFuncDecl();

    // classDecl -> CLASS IDENT { classBody }
    //            | CLASS IDENT { }
    Result<ClassNode> parseClassDecl();

    // classBody -> classStmt classBody
    //            | ε
    Result<BlockNode> parseClassBody();

    // classStmt -> funcDecl
    //            | letStmt
    //            | constStmt
    Result<StatementNode> parseClassStmt();

    // paramList -> IDENT paramList'
    //            | ε
    Result<ParamListNode> parseParamList();
    // paramList' -> , IDENT paramList'
    //             | ε
    Result<ParamListNode> parseParamListP(shared_ptr<VarDeclNode> lhs);

    // returnStmt -> RETURN expr ;
    //             | RETURN ;
    Result<ReturnStatementNode> parseReturnStmt();

    // breakStmt -> BREAK ;
    Result<BreakNode> parseBreakStmt();

    // continueStmt -> CONTINUE ;
    Result<ContinueNode> parseContinueStmt();

    // deleteStmt -> DELETE IDENT ;
    Result<DeleteNode> parseDeleteStmt();

    // importStmt -> IMPORT < moduleName > ;
    Result<ImportNode> parseImportStmt();

    // moduleName -> IDENT . IDENT
    //             | IDENT
    Result<StringNode> parseModuleName();

    // letStmt -> LET IDENT = expr ;
    Result<VarDeclNode> parseLetStmt();

    //***************************************************
    // constStmt -> CONST IDENT = expr ;
    //***************************************************
    // firsts: CONST
    Result<VarDeclNode> parseConstStmt();

    //******************************************************************************************
    // Expressions
    //******************************************************************************************

    //***************************************************
    // expr -> assign expr'
    //***************************************************
    // firsts: INC, DEC, +, -, !, (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parseExpr();

    //***************************************************
    // expr' -> , assign expr'
    //        | ϵ
    //***************************************************
    // firsts: ,
    Result<ExpressionNode> parseExprP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // assign -> or assign'
    //***************************************************
    // firsts: INC, DEC, +, -, !, (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parseAssign();

    //***************************************************
    // assign' -> = or assign'
    //          | ϵ
    //***************************************************
    // firsts: =
    Result<ExpressionNode> parseAssignP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // or -> and or'
    //***************************************************
    // firsts: INC, DEC, +, -, !, (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parseOr();

    //***************************************************
    // or' -> OR and or'
    //      | ϵ
    //***************************************************
    // firsts: OR
    Result<ExpressionNode> parseOrP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // and -> equality and'
    //***************************************************
    // firsts: INC, DEC, +, -, !, (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parseAnd();

    //***************************************************
    // and' -> AND equality and'
    //       | ϵ
    //***************************************************
    // firsts: AND
    Result<ExpressionNode> parseAndP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // equality -> relation equality'
    //***************************************************
    // firsts: INC, DEC, +, -, !, (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parseEquality();

    //***************************************************
    // equality' -> EQ relation equality'
    //            | NE relation equality'
    //            | ϵ
    //***************************************************
    // firsts: EQ, NE
    Result<ExpressionNode> parseEqualityP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // relation -> addit relation'
    //***************************************************
    // firsts: INC, DEC, +, -, !, (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parseRelation();

    //***************************************************
    // relation' -> > addit relation'
    //            | < addit relation'
    //            | GE addit relation'
    //            | LE addit relation'
    //            | ϵ
    //***************************************************
    // firsts: >, <, GE, LE
    Result<ExpressionNode> parseRelationP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // addit -> mult addit'
    //***************************************************
    // firsts: INC, DEC, +, -, !, (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parseAddit();

    //***************************************************
    // addit' -> + mult addit'
    //         | - mult addit'
    //         | ϵ
    //***************************************************
    // firsts: +, -
    Result<ExpressionNode> parseAdditP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // mult -> unary mult'
    //***************************************************
    // firsts: INC, DEC, +, -, !, (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parseMult();

    //***************************************************
    // mult' -> * unary mult'
    //        | / unary mult'
    //        | % unary mult'
    //        | ϵ
    //***************************************************
    // firsts: *, /, %
    Result<ExpressionNode> parseMultP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // unary -> INC unary
    //        | DEC unary
    //        | + mult
    //        | - mult
    //        | ! mult
    //        | post
    //***************************************************
    // firsts: INC, DEC, +, -, !, (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parseUnary();

    //***************************************************
    // argList -> assign argList'
    //***************************************************
    // firsts: INC, DEC, +, -, !, (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ArgListNode> parseArgList();

    //****************************************************
    // argList' -> , assign argList'
    //           | ϵ
    //***************************************************
    // firsts: ,
    Result<ArgListNode> parseArgListP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // post -> primary post''
    //***************************************************
    // firsts: (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parsePost();

    //***************************************************
    // post' -> [ expr ]
    //        | ( argList )
    //        | . IDENT
    //        | INC
    //        | DEC
    //        | ?
    //***************************************************
    // firsts: [, (, ., INC, DEC, ?
    Result<ExpressionNode> parsePostP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // post'' -> post' post''
    //         | ϵ
    //***************************************************
    // firsts: (, [, ., INC, DEC
    Result<ExpressionNode> parsePostPP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // primary -> ( expr )
    //          | [ argList ]
    //          | IDENT
    //          | NUMBER
    //          | STRING
    //          | TRUE
    //          | FALSE
    //          | NULL
    //***************************************************
    // firsts: (, [, IDENT, NUMBER, STRING, TRUE, FALSE, NULL
    Result<ExpressionNode> parsePrimary();
private:
    static set<int> additFirsts;
    static set<int> andFirsts;
    static set<int> argListFirsts;
    static set<int> assertFirsts;
    static set<int> assignFirsts;
    static set<int> blockFirsts;
    static set<int> breakStmtFirsts;
    static set<int> classDeclFirsts;
    static set<int> constStmtFirsts;
    static set<int> continueStmtFirsts;
    static set<int> deleteStmtFirsts;
    static set<int> equalityFirsts;
    static set<int> exprFirsts;
    static set<int> exprStmtFirsts;
    static set<int> forEachStmtFirsts;
    static set<int> forStmtFirsts;
    static set<int> funcDeclFirsts;
    static set<int> ifStmtFirsts;
    static set<int> importFirsts;
    static set<int> letStmtFirsts;
    static set<int> multFirsts;
    static set<int> postFirsts;
    static set<int> postPFirsts;
    static set<int> printStmtFirsts;
    static set<int> relationFirsts;
    static set<int> returnStmtFirsts;
    static set<int> unaryFirsts;
    static set<int> whileStmtFirsts;
    static set<int> assignPFirsts;
private:
    Tokenizer tokenizer;
    Token currentToken;
    bool hadError;
    int depth;
};
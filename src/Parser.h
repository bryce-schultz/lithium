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

using std::string;
using std::istream;
using std::set;
using std::shared_ptr;
using std::make_shared;

template<typename T>
struct Result
{
    bool success;
    shared_ptr<T> node;
};

class Parser
{
public:
    Parser();
    Result<Node> parse(const string &input, const string &filename = "");
private:
    Token peekToken() const;
    void advanceToken();

    bool isInFirstSet(const Token &token, const set<int> &firstSet) const;
private:
    // stmts -> stmt stmts
    //        | ε
    Result<StatementsNode> parseStmts();

    // stmt -> exprStmt     - firsts: NUMBER, IDENT, STRING, (, [, PRINT, LET, CONST
    //       | forStmt      - firsts: FOR
    //       | whileStmt    - firsts: WHILE
    //       | ifStmt       - firsts: IF
    //       | block        - firsts: {
    //       | funcDecl     - firsts: FN
    //       | returnStmt   - firsts: RETURN
    Result<StatementNode> parseStmt();

    // exprStmt -> expr ;
    //           | letStmt
    //           | constStmt
    //           | printStmt
    //           | ;
    Result<StatementNode> parseExprStmt();

    // forStmt -> FOR ( exprStmt exprStmt expr ) stmt
    //          | FOR ( exprStmt exprStmt epxr ) stmt ELSE stmt
    //Result<ForStatementNode> parseForStmt();

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

    // paramList -> IDENT paramList'
    //            | ε
    Result<ParamListNode> parseParamList();
    // paramList' -> , IDENT paramList'
    //             | ε
    Result<ParamListNode> parseParamListP(shared_ptr<VarDeclNode> lhs);

    // returnStmt -> RETURN expr ;
    //             | RETURN ;
    Result<ReturnStatementNode> parseReturnStmt();

    // letStmt -> LET IDENT = expr ;
    Result<VarDeclNode> parseLetStmt();

    // constStmt -> CONST IDENT = expr ;
    Result<VarDeclNode> parseConstStmt();

    // printStmt -> PRINT ( expr ) ;
    Result<PrintStatementNode> parsePrintStmt();

    //******************************************************************************************

    //***************************************************
    // expr -> assign expr'
    Result<ExpressionNode> parseExpr();
    // expr' -> , assign expr'
    //        | nothing
    Result<ExpressionNode> parseExprP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // assign -> or assign'
    Result<ExpressionNode> parseAssign();
    // assign' -> = or assign'
    //          | nothing
    Result<ExpressionNode> parseAssignP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // or -> and or'
    Result<ExpressionNode> parseOr();
    // or' -> OR and or'
    //      | nothing
    Result<ExpressionNode> parseOrP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // and -> equality and'
    Result<ExpressionNode> parseAnd();
    // and' -> AND equality and'
    //       | nothing
    Result<ExpressionNode> parseAndP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // equality -> relation equality'
    Result<ExpressionNode> parseEquality();
    // equality' -> EQ relation equality'
    //            | NE relation equality'
    //            | nothing
    Result<ExpressionNode> parseEqualityP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // relation -> addit relation'
    Result<ExpressionNode> parseRelation();
    // relation' -> > addit relation'
    //            | < addit relation'
    //            | GE addit relation'
    //            | LE addit relation'
    //            | nothing
    Result<ExpressionNode> parseRelationP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // addit -> mult addit'
    Result<ExpressionNode> parseAddit();
    // addit' -> + mult addit'
    //         | - mult addit'
    //         | nothing
    Result<ExpressionNode> parseAdditP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // mult -> unary mult'
    Result<ExpressionNode> parseMult();
    // mult' -> * uanry mult'
    //        | / uanry mult'
    //        | % unary mult'
    //        | nothing
    Result<ExpressionNode> parseMultP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // unary -> INC unary
    //        | DEC unary
    //        | + mult
    //        | - mult
    //        | ! mult
    //        | post
    Result<ExpressionNode> parseUnary();

    //***************************************************
    // argList -> assign argList'
    Result<ArgListNode> parseArgList();

    //****************************************************
    // argList' -> , assign argList'
    //           | nothing
    Result<ArgListNode> parseArgListP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // post -> primary post''
    Result<ExpressionNode> parsePost();
    // post' -> [ expr ]
    //        | ( argList )
    //        | . IDENT
    //        | INC
    //        | DEC
    Result<ExpressionNode> parsePostP(shared_ptr<ExpressionNode> lhs);
    // post'' -> post' post''
    //         | ϵ
    Result<ExpressionNode> parsePostPP(shared_ptr<ExpressionNode> lhs);

    //***************************************************
    // primary -> ( expr )
    //          | [ expr ]
    //          | IDENT
    //          | NUMBER
    //          | STRING
    Result<ExpressionNode> parsePrimary();

    //******************************************************************************************
private:
    void initFirstSets();
    set<int> exprStmtFirsts;
    set<int> forStmtFirsts;
    set<int> whileStmtFirsts;
    set<int> ifStmtFirsts;
    set<int> blockFirsts;
    set<int> funcDeclFirsts;
    set<int> returnStmtFirsts;
    set<int> letStmtFirsts;
    set<int> constStmtFirsts;
    set<int> printStmtFirsts;
    set<int> exprFirsts;
    set<int> assignFirsts;
    set<int> orFirsts;
    set<int> andFirsts;
    set<int> equalityFirsts;
    set<int> relationFirsts;
    set<int> additFirsts;
    set<int> multFirsts;
    set<int> unaryFirsts;
    set<int> argListFirsts;
    set<int> postFirsts;
    set<int> postPFirsts;
private:
    Tokenizer tokenizer;
    Token currentToken;
    bool hadError = false;
    int depth = 0;
};
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
    ~Parser(); // Add explicit destructor
    Result<Node> parse(const string &input, const string &filename = "");
private:
    Token peekToken() const;
    void advanceToken();

    bool isInFirstSet(const Token &token, const set<int> &firstSet) const;
private:
    // stmts -> stmt stmts
    //        | ε
    Result<StatementsNode> parseStmts();

    // stmt -> assertStmt   - firsts: ASSERT
    //       | exprStmt     - firsts: NUMBER, IDENT, STRING, (, [, PRINT, LET, CONST
    //       | forStmt      - firsts: FOR
    //       | whileStmt    - firsts: WHILE
    //       | ifStmt       - firsts: IF
    //       | block        - firsts: {
    //       | funcDecl     - firsts: FN
    //       | classDecl    - firsts: CLASS
    //       | returnStmt   - firsts: RETURN
    //       | breakStmt    - firsts: BREAK
    //       | continueStmt - firsts: CONTINUE
    //       | importStmt   - firsts: IMPORT
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

    // importStmt -> IMPORT < moduleName > ;
    Result<ImportNode> parseImportStmt();

    // moduleName -> IDENT . IDENT
    //             | IDENT
    Result<StringNode> parseModuleName();

    // letStmt -> LET IDENT = expr ;
    Result<VarDeclNode> parseLetStmt();

    // constStmt -> CONST IDENT = expr ;
    Result<VarDeclNode> parseConstStmt();

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
    static set<int> orFirsts;
    static set<int> postFirsts;
    static set<int> postPFirsts;
    static set<int> printStmtFirsts;
    static set<int> relationFirsts;
    static set<int> returnStmtFirsts;
    static set<int> unaryFirsts;
    static set<int> whileStmtFirsts;
private:
    Tokenizer tokenizer;
    Token currentToken;
    bool hadError = false;
    int depth = 0;
};
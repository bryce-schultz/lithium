//**************************************************
// File: Parser.cpp
//
// Author: Bryce Schultz
//
// Purpose: Implements the Parser class, which
// parses tokens into an abstract syntax tree (AST).
//**************************************************

#include <iostream>

#include "Parser.h"
#include "Error.h"
#include "Utils.h"
#include "Nodes.h"
#include "Tokenizer.h"

#define accept(x) return { true, x }
#define reject() return { false, nullptr }
#define acceptNode(x) \
    { \
        auto result = x; \
        if (!result.status) \
        { \
            reject(); \
        } \
        accept(result.value); \
    }

#define error(msg) \
    tokenError(msg, currentToken, __FILE__, __LINE__); \
    hadError = true; \
    reject()

#define expected(expected) \
    if (currentToken == Token::END) \
    { \
        error(string("unexpected end of file, expected '") + expected + string("'")); \
    } \
    else \
    { \
        error(string("expected '") + expected + string("' but found '") + currentToken.toString() + "'"); \
    }

#define expectToken(x) \
    peekToken(); \
    if (peekToken() != x) \
    { \
        expected(Token::tokenTypeToString(x)); \
    } \
    advanceToken()

Parser::Parser():
    tokenizer(),
    currentToken()
{ }

Parser::~Parser()
{
    // Reset the tokenizer to ensure proper cleanup
    tokenizer = Tokenizer();
    currentToken = Token();
}

set<int> Parser::additFirsts = { '+', '-' };
set<int> Parser::andFirsts = { Token::AND };
set<int> Parser::argListFirsts = exprFirsts;
set<int> Parser::assertFirsts = { Token::ASSERT };
set<int> Parser::assignFirsts = { Token::NUMBER, Token::IDENT, Token::STRING,Token::LET, Token::CONST, Token::INC, Token::DEC, '(', '[', ';', '+', '!' };
set<int> Parser::blockFirsts = { '{' };
set<int> Parser::breakStmtFirsts = { Token::BREAK };
set<int> Parser::classDeclFirsts = { Token::CLASS };
set<int> Parser::constStmtFirsts = { Token::CONST };
set<int> Parser::continueStmtFirsts = { Token::CONTINUE };
set<int> Parser::equalityFirsts = { Token::EQ, Token::NE };
set<int> Parser::exprFirsts = { Token::NUMBER, Token::IDENT, Token::STRING, Token::LET, Token::CONST, Token::INC, Token::DEC, '(', '[', '-', '+', '!' };
set<int> Parser::exprStmtFirsts = { Token::NUMBER, Token::IDENT, Token::STRING, Token::LET, Token::CONST, Token::INC, Token::DEC, '(', '[', ';', '-', '+', '!' };
set<int> Parser::forEachStmtFirsts = { Token::FOREACH };
set<int> Parser::forStmtFirsts = { Token::FOR };
set<int> Parser::funcDeclFirsts = { Token::FN };
set<int> Parser::ifStmtFirsts = { Token::IF };
set<int> Parser::importFirsts = { Token::IMPORT };
set<int> Parser::letStmtFirsts = { Token::LET };
set<int> Parser::multFirsts = { '*', '/', '%' };
set<int> Parser::orFirsts = { Token::OR };
set<int> Parser::postFirsts = { Token::NUMBER, Token::IDENT, Token::STRING, '(', '[', '-', '+', '!' };
set<int> Parser::postPFirsts = { '(', '[', '.', Token::INC, Token::DEC };
set<int> Parser::relationFirsts = { '>', '<', Token::LE, Token::GE };
set<int> Parser::returnStmtFirsts = { Token::RETURN };
set<int> Parser::unaryFirsts = { '+', '-', '!', '~' };
set<int> Parser::whileStmtFirsts = { Token::WHILE };

Token Parser::peekToken() const
{
    return currentToken;
}

void Parser::advanceToken()
{
    currentToken = tokenizer.lex();
}

bool Parser::isInFirstSet(const Token &token, const set<int> &firstSet) const
{
    return firstSet.count(token.getType()) > 0;
}


Result<Node> Parser::parse(const string &input, const string &filename)
{
    tokenizer = Tokenizer(input, filename);
    hadError = false;
    currentToken = tokenizer.lex();

    auto result = parseStmts();
    if (!result.status || hadError)
    {
        // Clear tokenizer before returning error
        tokenizer = Tokenizer();
        reject();
    }

    // Clear tokenizer before successful return
    tokenizer = Tokenizer();
    accept(result.value);
}

// stmts -> stmts stmt
//        | stmt
Result<StatementsNode> Parser::parseStmts()
{
    auto statements = make_shared<StatementsNode>();
    Token token = peekToken();
    while (token != Token::END)
    {
        // parse a single statement
        auto result = parseStmt();
        if (!result.status)
        {
            break;
        }

        statements->addStatement(result.value);
        token = peekToken();
    }

    accept(statements);
}

// stmt -> exprStmt     - firsts: NUMBER, IDENT, STRING, (, [, PRINT, LET, CONST, ;
//       | forStmt      - firsts: FOR
//       | whileStmt    - firsts: WHILE
//       | ifStmt       - firsts: IF
//       | block        - firsts: {
//       | fnDecl       - firsts: FN
//       | classDecl    - firsts: CLASS
//       | returnStmt   - firsts: RETURN
//       | breakStmt    - firsts: BREAK
//       | importStmt   - firsts: IMPORT
Result<StatementNode> Parser::parseStmt()
{
    Token token = peekToken();

    if (token == Token::END)
    {
        accept(nullptr); // No more statements to parse
    }
    else if (isInFirstSet(token, assertFirsts))
    {
        acceptNode(parseAssert());
    }
    else if (isInFirstSet(token, exprStmtFirsts))
    {
        acceptNode(parseExprStmt());
    }
    else if (isInFirstSet(token, forStmtFirsts))
    {
        acceptNode(parseForStmt());
    }
    else if (isInFirstSet(token, forEachStmtFirsts))
    {
        acceptNode(parseForEachStmt());
    }
    else if (isInFirstSet(token, whileStmtFirsts))
    {
        acceptNode(parseWhileStmt());
    }
    else if (isInFirstSet(token, ifStmtFirsts))
    {
        acceptNode(parseIfStmt());
    }
    else if (isInFirstSet(token, funcDeclFirsts))
    {
        acceptNode(parseFuncDecl());
    }
    else if (isInFirstSet(token, blockFirsts))
    {
        acceptNode(parseBlock());
    }
    else if (isInFirstSet(token, returnStmtFirsts))
    {
        acceptNode(parseReturnStmt());
    }
    else if (isInFirstSet(token, breakStmtFirsts))
    {
        acceptNode(parseBreakStmt());
    }
    else if (isInFirstSet(token, continueStmtFirsts))
    {
        acceptNode(parseContinueStmt());
    }
    else if (isInFirstSet(token, importFirsts))
    {
        acceptNode(parseImportStmt());
    }
    else if (isInFirstSet(token, classDeclFirsts))
    {
        acceptNode(parseClassDecl());
    }
    else if (token == '}' && depth != 0)
    {
        reject();
    }

    error("unexpected token");
}

// assertStmt -> ASSERT expr ;
//             | ASSERT expr , expr ;
Result<AssertNode> Parser::parseAssert()
{
    Token assertToken = expectToken(Token::ASSERT);

    auto exprResult = parseAssign();
    if (!exprResult.status)
    {
        reject();
    }

    shared_ptr<ExpressionNode> message = nullptr;
    Token token = peekToken();
    if (token == ',')
    {
        advanceToken(); // consume ','
        auto messageResult = parseExpr();
        if (!messageResult.status)
        {
            reject();
        }
        message = messageResult.value;
    }

    Token semiColonToken = expectToken(';');

    shared_ptr<AssertNode> assertNode = make_shared<AssertNode>(exprResult.value, message);
    assertNode->setRangeStart(assertToken.getRange().getStart());

    accept(assertNode);
}

// exprStmt -> expr ;                  - firsts: NUMBER, IDENT, STRING, (, [, -, +, !, LET, CONST, INC, DEC
//           | LET IDENT = expr ;      - firsts: LET
//           | CONST IDENT = expr ;    - firsts: CONST
//           | PRINT ( exprList ) ;    - firsts: PRINT
//           | ;
Result<StatementNode> Parser::parseExprStmt()
{
    Token token = peekToken();

    if (token == ';')
    {
        advanceToken(); // consume ';'
        accept(nullptr); // Empty statement
    }
    else if (isInFirstSet(token, letStmtFirsts))
    {
        acceptNode(parseLetStmt());
    }
    else if (isInFirstSet(token, constStmtFirsts))
    {
        acceptNode(parseConstStmt());
    }
    else if (isInFirstSet(token, exprFirsts))
    {
        auto exprResult = parseExpr();
        if (!exprResult.status)
        {
            reject();
        }

        expectToken(';');

        accept(exprResult.value);
    }

    reject();
}

// forStmt -> FOR ( exprStmt exprStmt expr ) stmt
Result<ForStatementNode> Parser::parseForStmt()
{
    Token forToken = expectToken(Token::FOR);

    Token openParenToken = expectToken('(');

    auto initResult = parseExprStmt();
    if (!initResult.status)
    {
        reject();
    }

    auto conditionResult = parseExprStmt();
    if (!conditionResult.status)
    {
        reject();
    }

    auto incrementResult = parseExpr();
    if (!incrementResult.status)
    {
        reject();
    }

    Token closeParenToken = expectToken(')');

    auto bodyResult = parseStmt();
    if (!bodyResult.status)
    {
        reject();
    }

    auto forNode = make_shared<ForStatementNode>(initResult.value, conditionResult.value, incrementResult.value, bodyResult.value);
    forNode->setRangeStart(forToken.getRange().getStart());

    accept(forNode);
}

// forEachStmt -> FOREACH ( IDENT : expr ) stmt
//              | FOREACH ( IDENT , IDENT : expr ) stmt
Result<ForEachNode> Parser::parseForEachStmt()
{
    Token forEachToken = expectToken(Token::FOREACH);

    Token openParenToken = expectToken('(');

    Token keyIdentifier = expectToken(Token::IDENT);
    Token valueIdentifier;

    Token token = peekToken();
    if (token == ',')
    {
        advanceToken(); // consume ','
        valueIdentifier = expectToken(Token::IDENT);
    }

    Token colonToken = expectToken(':');

    auto iterableResult = parseExpr();
    if (!iterableResult.status)
    {
        reject();
    }

    Token closeParenToken = expectToken(')');

    auto bodyResult = parseStmt();
    if (!bodyResult.status)
    {
        reject();
    }

    shared_ptr<VarDeclNode> keyDecl = make_shared<VarDeclNode>(keyIdentifier);
    shared_ptr<VarDeclNode> valueDecl = valueIdentifier != Token::NONE ? make_shared<VarDeclNode>(valueIdentifier) : nullptr;

    auto forEachNode = make_shared<ForEachNode>(keyDecl, valueDecl, iterableResult.value, bodyResult.value);
    forEachNode->setRangeStart(forEachToken.getRange().getStart());

    accept(forEachNode);
}

// whileStmt -> WHILE ( expr ) stmt
Result<WhileNode> Parser::parseWhileStmt()
{
    Token whileToken = expectToken(Token::WHILE);

    Token openParenToken = expectToken('(');

    auto exprResult = parseExpr();
    if (!exprResult.status)
    {
        reject();
    }

    Token closeParenToken = expectToken(')');

    auto bodyResult = parseStmt();
    if (!bodyResult.status)
    {
        reject();
    }

    auto whileNode = make_shared<WhileNode>(exprResult.value, bodyResult.value);
    whileNode->setRangeStart(whileToken.getRange().getStart());

    accept(whileNode);
}

// ifStmt -> IF ( expr ) stmt
//         | IF ( expr ) stmt ELSE stmt
Result<IfStatementNode> Parser::parseIfStmt()
{
    Token ifToken = expectToken(Token::IF);

    Token openParenToken = expectToken('(');

    auto exprResult = parseExpr();
    if (!exprResult.status)
    {
        reject();
    }

    Token closeParenToken = expectToken(')');

    auto thenStmtResult = parseStmt();
    if (!thenStmtResult.status)
    {
        reject();
    }

    shared_ptr<StatementNode> elseBranch = nullptr;
    Token token = peekToken();
    if (token == Token::ELSE)
    {
        advanceToken(); // consume 'else'
        auto elseStmtResult = parseStmt();
        if (!elseStmtResult.status)
        {
            reject();
        }
        elseBranch = elseStmtResult.value;
    }

    auto ifStmt = make_shared<IfStatementNode>(exprResult.value, thenStmtResult.value, elseBranch);
    ifStmt->setRangeStart(ifToken.getRange().getStart());

    accept(ifStmt);
}

// block -> { stmts }
//        | { }
Result<BlockNode> Parser::parseBlock()
{
    Token openCurlyToken = expectToken('{');

    depth++;

    Token token = peekToken();
    if (token == '}')
    {
        advanceToken();

        depth--;

        auto emptyBlock = make_shared<BlockNode>();
        emptyBlock->setRangeStart(openCurlyToken.getRange().getStart());
        emptyBlock->setRangeEnd(token.getRange().getEnd());
        accept(emptyBlock);
    }

    auto stmtsResult = parseStmts();
    if (!stmtsResult.status)
    {
        reject();
    }

    Token closeCurlyToken = expectToken('}');

    depth--;

    auto blockNode = make_shared<BlockNode>(stmtsResult.value);
    blockNode->setRangeStart(openCurlyToken.getRange().getStart());
    blockNode->setRangeEnd(closeCurlyToken.getRange().getEnd());

    accept(blockNode);
}

// funcDecl -> FN IDENT ( argList ) stmt ;
//           | FN IDENT ( ) stmt;
Result<FuncDeclNode> Parser::parseFuncDecl()
{
    Token fnToken = expectToken(Token::FN);

    Token identifier = expectToken(Token::IDENT);

    Token openParenToken = expectToken('(');

    shared_ptr<ParamListNode> params = nullptr;

    Token token = peekToken();
    if (token != ')')
    {
        auto result = parseParamList();
        if (!result.status)
        {
            reject();
        }
        params = result.value;
    }

    Token closeParenToken = expectToken(')');

    auto bodyResult = parseStmt();
    if (!bodyResult.status)
    {
        reject();
    }

    auto funcDecl = make_shared<FuncDeclNode>(identifier, params, bodyResult.value);
    funcDecl->setRangeStart(fnToken.getRange().getStart());
    //funcDecl->setRangeEnd(closeParenToken.getRange().getEnd());

    accept(funcDecl);
}

// classDecl -> CLASS IDENT { classBody }
//            | CLASS IDENT { }
Result<ClassNode> Parser::parseClassDecl()
{
    Token classToken = expectToken(Token::CLASS);

    Token identifier = expectToken(Token::IDENT);

    Token openCurlyToken = expectToken('{');

    auto bodyResult = parseClassBody();
    if (!bodyResult.status)
    {
        reject();
    }

    Token closeCurlyToken = expectToken('}');

    auto classNode = make_shared<ClassNode>(identifier, bodyResult.value);
    classNode->setRangeStart(classToken.getRange().getStart());
    classNode->setRangeEnd(closeCurlyToken.getRange().getEnd());

    accept(classNode);
}

// classBody -> classStmt classBody
//            | ε
Result<BlockNode> Parser::parseClassBody()
{
    auto block = make_shared<BlockNode>();

    Token token = peekToken();
    while (token != '}')
    {
        // parse a single class statement
        auto result = parseClassStmt();
        if (!result.status)
        {
            break;
        }

        block->addStatement(result.value);
        token = peekToken();
    }

    accept(block);
}

// classStmt -> funcDecl
//            | letStmt
//            | constStmt
Result<StatementNode> Parser::parseClassStmt()
{
    Token token = peekToken();

    if (isInFirstSet(token, funcDeclFirsts))
    {
        acceptNode(parseFuncDecl());
    }
    else if (isInFirstSet(token, letStmtFirsts))
    {
        acceptNode(parseLetStmt());
    }
    else if (isInFirstSet(token, constStmtFirsts))
    {
        acceptNode(parseConstStmt());
    }
    else
    {
        error("unexpected token in class body");
    }
}

// paramList -> IDENT paramList'
//            | ε
Result<ParamListNode> Parser::parseParamList()
{
    Token identifier = expectToken(Token::IDENT);

    auto param = make_shared<VarDeclNode>(identifier);
    param->setRangeStart(identifier.getRange().getStart());

    auto result = parseParamListP(param);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}

// paramList' -> , IDENT paramList'
//             | ε
Result<ParamListNode> Parser::parseParamListP(shared_ptr<VarDeclNode> lhs)
{
    Token token = peekToken();
    if (token != ',')
    {
        auto params = make_shared<ParamListNode>(lhs);
        params->setRangeEnd(lhs->getRange().getEnd());
        accept(params);
    }
    advanceToken(); // consume ','

    Token identifier = expectToken(Token::IDENT);

    auto param = make_shared<VarDeclNode>(identifier);
    param->setRangeStart(identifier.getRange().getStart());

    auto result = parseParamListP(param);
    if (!result.status)
    {
        reject();
    }

    auto params = make_shared<ParamListNode>(lhs);
    params->addAllParams(result.value);
    params->setRangeEnd(param->getRange().getEnd());

    accept(params);
}

// returnStmt -> RETURN expr ;
//             | RETURN ;
Result<ReturnStatementNode> Parser::parseReturnStmt()
{
    Token returnToken = expectToken(Token::RETURN);

    Token token = peekToken();
    if (token == ';')
    {
        advanceToken(); // consume ';'

        auto returnStmt = make_shared<ReturnStatementNode>();
        returnStmt->setRangeStart(returnToken.getRange().getStart());
        accept(returnStmt);
    }

    auto exprResult = parseExpr();
    if (!exprResult.status)
    {
        reject();
    }

    Token semicolonToken = expectToken(';');

    auto returnStmt = make_shared<ReturnStatementNode>(exprResult.value);
    returnStmt->setRangeStart(returnToken.getRange().getStart());

    accept(returnStmt);
}

Result<BreakNode> Parser::parseBreakStmt()
{
    Token breakToken = expectToken(Token::BREAK);

    Token semicolonToken = expectToken(';');

    auto breakNode = make_shared<BreakNode>(breakToken);

    accept(breakNode);
}

// continueStmt -> CONTINUE ;
Result<ContinueNode> Parser::parseContinueStmt()
{
    Token continueToken = expectToken(Token::CONTINUE);

    Token semicolonToken = expectToken(';');

    auto continueNode = make_shared<ContinueNode>(continueToken);

    accept(continueNode);
}

// importStmt -> IMPORT < moduleName >
Result<ImportNode> Parser::parseImportStmt()
{
    Token importToken = expectToken(Token::IMPORT);

    Token openAngleToken = expectToken('<');

    auto moduleNameResult = parseModuleName();
    if (!moduleNameResult.status)
    {
        reject();
    }
    Token moduleName = moduleNameResult.value->getToken();

    Token closeAngleToken = expectToken('>');

    auto importNode = make_shared<ImportNode>(moduleName);
    importNode->setRangeStart(importToken.getRange().getStart());
    importNode->setRangeEnd(closeAngleToken.getRange().getEnd());

    accept(importNode);
}

// moduleName -> IDENT . IDENT
//             | IDENT
Result<StringNode> Parser::parseModuleName()
{
    Token identifier = expectToken(Token::IDENT);

    Token token = peekToken();
    if (token == '.')
    {
        advanceToken(); // consume '.'

        Token secondIdentifier = expectToken(Token::IDENT);

        Range range(identifier.getRange().getStart(), secondIdentifier.getRange().getEnd());
        // Create a new token that combines the two identifiers, replace the . with a / since it will be
        // used as a path separator in the import statement.
        Token token(Token::STRING, range, identifier.getValue() + "/" + secondIdentifier.getValue());
        auto moduleName = make_shared<StringNode>(token);

        accept(moduleName);
    }

    auto moduleName = make_shared<StringNode>(identifier);
    moduleName->setRangeStart(identifier.getRange().getStart());
    moduleName->setRangeEnd(identifier.getRange().getEnd());

    accept(moduleName);
}

// letStmt -> LET IDENT = expr ;
Result<VarDeclNode> Parser::parseLetStmt()
{
    Token letToken = expectToken(Token::LET);

    Token identifier = expectToken(Token::IDENT);

    Token equalsToken = expectToken('=');

    auto exprResult = parseExpr();
    if (!exprResult.status)
    {
        reject();
    }

    Token semicolonToken = expectToken(';');

    // Create a variable declaration node with the identifier and the expression
    auto varDecl = make_shared<VarDeclNode>(identifier, exprResult.value);
    varDecl->setRangeStart(letToken.getRange().getStart());

    accept(varDecl);
}

// constStmt -> CONST IDENT = expr ;
Result<VarDeclNode> Parser::parseConstStmt()
{
    Token constToken = expectToken(Token::CONST);

    Token identifier = expectToken(Token::IDENT);

    Token equalsToken = expectToken('=');

    auto exprResult = parseExpr();
    if (!exprResult.status)
    {
        reject();
    }

    Token semicolonToken = expectToken(';');

    // Create a variable declaration node with the identifier and the expression
    auto varDecl = make_shared<VarDeclNode>(identifier, exprResult.value, true);
    varDecl->setRangeStart(constToken.getRange().getStart());

    accept(varDecl);
}

//******************************************************************************************

//***************************************************
// expr -> assign expr'
Result<ExpressionNode> Parser::parseExpr()
{
    auto result = parseAssign();
    if (!result.status)
    {
        reject();
    }

    result = parseExprP(result.value);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}
// expr' -> , assign expr'
//        | nothing
Result<ExpressionNode> Parser::parseExprP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();
    if (token != ',')
    {
        accept(lhs);
    }
    advanceToken(); // consume ','

    auto rhs = parseAssign();
    if (!rhs.status)
    {
        reject();
    }

    auto exprP = parseExprP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.value));
    if (!exprP.status)
    {
        reject();
    }

    accept(exprP.value);
}

//***************************************************
// assign -> or assign'
Result<ExpressionNode> Parser::parseAssign()
{
    auto result = parseOr();
    if (!result.status)
    {
        reject();
    }

    result = parseAssignP(result.value);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}
// assign' -> = or assign'
//          | += or assign'
//          | -= or assign'
//          | *= or assign'
//          | /= or assign'
//          | %= or assign'
//          | nothing
Result<ExpressionNode> Parser::parseAssignP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();
    if (token != '=' &&
        token != Token::PLUS_EQUAL &&
        token != Token::MINUS_EQUAL &&
        token != Token::MUL_EQUAL &&
        token != Token::DIV_EQUAL &&
        token != Token::MOD_EQUAL)
    {
        accept(lhs);
    }
    advanceToken(); // consume 'or'

    auto rhs = parseOr();
    if (!rhs.status)
    {
        reject();
    }

    rhs = parseAssignP(make_shared<AssignNode>(lhs, token, rhs.value));
    if (!rhs.status)
    {
        reject();
    }

    accept(rhs.value);
}

//***************************************************
// or -> and or'
Result<ExpressionNode> Parser::parseOr()
{
    auto result = parseAnd();
    if (!result.status)
    {
        reject();
    }

    result = parseOrP(result.value);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}
// or' -> OR and or'
//      | nothing
Result<ExpressionNode> Parser::parseOrP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();
    if (token != Token::OR)
    {
        accept(lhs);
    }
    advanceToken(); // consume 'and'

    auto rhs = parseAnd();
    if (!rhs.status)
    {
        reject();
    }

    rhs = parseOrP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.value));
    if (!rhs.status)
    {
        reject();
    }

    accept(rhs.value);
}

//***************************************************
// and -> equality and'
Result<ExpressionNode> Parser::parseAnd()
{
    auto result = parseEquality();
    if (!result.status)
    {
        reject();
    }

    result = parseAndP(result.value);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}
// and' -> AND equality and'
//       | nothing
Result<ExpressionNode> Parser::parseAndP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();
    if (token != Token::AND)
    {
        accept(lhs);
    }
    advanceToken(); // consume 'eq'

    auto rhs = parseEquality();
    if (!rhs.status)
    {
        reject();
    }

    rhs = parseAndP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.value));
    if (!rhs.status)
    {
        reject();
    }

    accept(rhs.value);
}

//***************************************************
// equality -> relation equality'
Result<ExpressionNode> Parser::parseEquality()
{
    auto result = parseRelation();
    if (!result.status)
    {
        reject();
    }

    result = parseEqualityP(result.value);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}
// equality' -> EQ relation equality'
//            | NE relation equality'
//            | nothing
Result<ExpressionNode> Parser::parseEqualityP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();
    if (token != Token::EQ && token != Token::NE)
    {
        accept(lhs);
    }
    advanceToken(); // consume operator

    auto rhs = parseRelation();
    if (!rhs.status)
    {
        reject();
    }

    rhs = parseEqualityP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.value));
    if (!rhs.status)
    {
        reject();
    }

    accept(rhs.value);
}

//***************************************************
// relation -> addit relation'
Result<ExpressionNode> Parser::parseRelation()
{
    auto result = parseAddit();
    if (!result.status)
    {
        reject();
    }

    result = parseRelationP(result.value);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}
// relation' -> > addit relation'
//            | < addit relation'
//            | GE addit relation'
//            | LE addit relation'
//            | nothing
Result<ExpressionNode> Parser::parseRelationP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();
    if (token != '>' && token != '<' && token != Token::GE && token != Token::LE)
    {
        accept(lhs);
    }
    advanceToken(); // consume 'relation' operator

    auto rhs = parseAddit();
    if (!rhs.status)
    {
        reject();
    }

    rhs = parseRelationP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.value));
    if (!rhs.status)
    {
        reject();
    }

    accept(rhs.value);
}

//***************************************************
// addit -> mult addit'
Result<ExpressionNode> Parser::parseAddit()
{
    auto result = parseMult();
    if (!result.status)
    {
        reject();
    }

    result = parseAdditP(result.value);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}
// addit' -> + mult addit'
//         | - mult addit'
//         | nothing
Result<ExpressionNode> Parser::parseAdditP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();
    if (token != '+' && token != '-')
    {
        accept(lhs);
    }
    advanceToken(); // consume operator

    auto rhs = parseMult();
    if (!rhs.status)
    {
        reject();
    }

    rhs = parseAdditP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.value));
    if (!rhs.status)
    {
        reject();
    }

    accept(rhs.value);
}

//***************************************************
// mult -> unary mult'
Result<ExpressionNode> Parser::parseMult()
{
    auto result = parseUnary();
    if (!result.status)
    {
        reject();
    }

    result = parseMultP(result.value);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}
// mult' -> * uanry mult'
//        | / uanry mult'
//        | % unary mult'
//        | nothing
Result<ExpressionNode> Parser::parseMultP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();
    if (token != '*' && token != '/' && token != '%')
    {
        accept(lhs);
    }
    advanceToken(); // consume operator

    auto rhs = parseUnary();
    if (!rhs.status)
    {
        reject();
    }

    rhs = parseMultP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.value));
    if (!rhs.status)
    {
        reject();
    }

    accept(rhs.value);
}

//***************************************************
// unary -> INC unary
//        | DEC unary
//        | + mult
//        | - mult
//        | ! mult
//        | post
Result<ExpressionNode> Parser::parseUnary()
{
    Token token = peekToken();

    if (token != Token::INC && token != Token::DEC && token != '+' && token != '-' && token != '!')
    {
        acceptNode(parsePost());
    }

    advanceToken(); // consume unary operator

    auto result = parseUnary();
    if (!result.status)
    {
        reject();
    }

    accept(make_shared<UnaryExprNode>(result.value, make_shared<OpNode>(token), true));
}

//***************************************************
// argList -> assign argListP
Result<ArgListNode> Parser::parseArgList()
{
    auto assignResult = parseAssign();
    if (!assignResult.status)
    {
        reject();
    }

    auto listResult = parseArgListP(assignResult.value);
    if (!listResult.status)
    {
        reject();
    }

    auto argList = make_shared<ArgListNode>();
    argList->addAllArgs(listResult.value);

    accept(argList);
}

// argList' -> , assign argListP
//           | nothing
Result<ArgListNode> Parser::parseArgListP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();
    if (token != ',')
    {
        accept(make_shared<ArgListNode>(lhs));
    }
    advanceToken(); // consume ','

    auto assignResult = parseAssign();
    if (!assignResult.status)
    {
        reject();
    }

    auto listResult = parseArgListP(assignResult.value);
    if (!listResult.status)
    {
        reject();
    }

    auto argList = make_shared<ArgListNode>(lhs);
    argList->addAllArgs(listResult.value);

    accept(argList);
}


//***************************************************
// post -> primary post''
Result<ExpressionNode> Parser::parsePost()
{
    auto result = parsePrimary();
    if (!result.status)
    {
        reject();
    }

    result = parsePostPP(result.value);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}
// post' -> [ expr ]
//        | ( argList )
//        | . IDENT
//        | INC
//        | DEC
Result<ExpressionNode> Parser::parsePostP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();

    if (token == '[')
    {
        Token openBracketToken = token;
        advanceToken();

        auto exprResult = parseExpr();
        if (!exprResult.status)
        {
            reject();
        }

        Token closeBracketToken = expectToken(']');

        shared_ptr<ArrayAccessNode> arrayAccessNode = make_shared<ArrayAccessNode>(lhs, exprResult.value);
        arrayAccessNode->setRangeEnd(closeBracketToken.getRange().getEnd());

        accept(arrayAccessNode);
    }
    else if (token == '(')
    {
        Token openToken = token;
        advanceToken(); // consume '('

        token = peekToken();

        if (token == ')')
        {
            advanceToken(); // consume ')'
            auto callNode = make_shared<CallNode>(lhs);
            callNode->setRangeEnd(token.getRange().getEnd());
            accept(callNode);
        }

        auto argListResult = parseArgList();
        if (!argListResult.status)
        {
            reject();
        }

        Token closeParenToken = expectToken(')');

        auto callNode = make_shared<CallNode>(lhs, argListResult.value);
        callNode->setRangeEnd(closeParenToken.getRange().getEnd());

        accept(callNode);
    }
    else if (token == '.')
    {
        advanceToken(); // consume '.'

        Token identifier = expectToken(Token::IDENT);

        accept(make_shared<MemberAccessNode>(lhs, identifier));
    }
    else if (token == Token::INC || token == Token::DEC)
    {
        advanceToken(); // consume 'inc' or 'dec'
        accept(make_shared<UnaryExprNode>(lhs, make_shared<OpNode>(token), false));
    }

    reject();
}
// post'' -> post' post''
//         | ϵ
Result<ExpressionNode> Parser::parsePostPP(shared_ptr<ExpressionNode> lhs)
{
    Token token = peekToken();
    if (!isInFirstSet(token, postPFirsts))
    {
        accept(lhs);
    }

    auto result = parsePostP(lhs);
    if (!result.status)
    {
        reject();
    }

    result = parsePostPP(result.value);
    if (!result.status)
    {
        reject();
    }

    accept(result.value);
}

//***************************************************
// primary -> ( expr )
//          | [ argList ]
//          | IDENT
//          | NUMBER
//          | STRING
Result<ExpressionNode> Parser::parsePrimary()
{
    Token token = peekToken();

    if (token == '(')
    {
        advanceToken(); // consume '('

        auto exprResult = parseExpr();
        if (!exprResult.status)
        {
            reject();
        }

        Token closeParenToken = expectToken(')');

        accept(exprResult.value);
    }
    else if (token == '[')
    {
        Token openBracketToken = expectToken('[');

        Token token = peekToken();
        if (token == ']')
        {
            advanceToken(); // consume ']'
            auto arrayNode = make_shared<ArrayNode>();
            arrayNode->setRangeStart(openBracketToken.getRange().getStart());
            arrayNode->setRangeEnd(token.getRange().getEnd());
            accept(arrayNode);
        }

        auto exprList = parseArgList();
        if (!exprList.status)
        {
            reject();
        }

        Token closeBracketToken = expectToken(']');

        shared_ptr<ArrayNode> arrayNode = make_shared<ArrayNode>(exprList.value->getArgs());
        arrayNode->setRangeStart(openBracketToken.getRange().getStart());
        arrayNode->setRangeEnd(closeBracketToken.getRange().getEnd());

        accept(arrayNode);
    }
    else if (token.getType() == Token::IDENT)
    {
        advanceToken(); // consume identifier
        accept(make_shared<VarExprNode>(token));
    }
    else if (token.getType() == Token::NUMBER)
    {
        advanceToken(); // consume number
        accept(make_shared<NumberNode>(token));
    }
    else if (token.getType() == Token::STRING)
    {
        advanceToken(); // consume string
        accept(make_shared<StringNode>(token));
    }

    expected("primary expression");
}
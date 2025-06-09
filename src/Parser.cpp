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

#define red "\033[31m"
#define reset "\033[0m"

#define accept(x) return { true, x }
#define reject() return { false, nullptr }
#define acceptNode(x) \
    { \
        auto result = x; \
        if (!result.success) \
        { \
            reject(); \
        } \
        accept(result.node); \
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
{
    initFirstSets();
}

void Parser::initFirstSets()
{
    exprStmtFirsts = { Token::NUMBER, Token::IDENT, Token::STRING, Token::PRINT, Token::LET, Token::CONST, Token::INC, Token::DEC, '(', '[', ';', '-', '+', '!' };
    forStmtFirsts = { Token::FOR };
    whileStmtFirsts = { Token::WHILE };
    ifStmtFirsts = { Token::IF };
    blockFirsts = { '{' };
    funcDeclFirsts = { Token::FN };
    returnStmtFirsts = { Token::RETURN };
    breakStmtFirsts = { Token::BREAK };
    letStmtFirsts = { Token::LET };
    constStmtFirsts = { Token::CONST };
    printStmtFirsts = { Token::PRINT };
    exprFirsts = { Token::NUMBER, Token::IDENT, Token::STRING, Token::PRINT, Token::LET, Token::CONST, Token::INC, Token::DEC, '(', '[', '-', '+', '!' };
    assignFirsts = { Token::NUMBER, Token::IDENT, Token::STRING, Token::PRINT, Token::LET, Token::CONST, Token::INC, Token::DEC, '(', '[', ';', '+', '!' };
    orFirsts = { Token::OR };
    andFirsts = { Token::AND };
    equalityFirsts = { Token::EQ, Token::NE };
    relationFirsts = { '>', '<', Token::LE, Token::GE };
    additFirsts = { '+', '-' };
    multFirsts = { '*', '/', '%' };
    unaryFirsts = { '+', '-', '!', '~' };
    argListFirsts = exprFirsts;
    postPFirsts = { '(', '[', '.', Token::INC, Token::DEC };
}

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
    if (!result.success || hadError)
    {
        reject();
    }
    accept(result.node);
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
        if (!result.success)
        {
            break;
        }

        statements->addStatement(result.node);
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
//       | returnStmt   - firsts: RETURN
Result<StatementNode> Parser::parseStmt()
{
    Token token = peekToken();

    if (token == Token::END)
    {
        accept(nullptr); // No more statements to parse
    }
    else if (isInFirstSet(token, exprStmtFirsts))
    {
        acceptNode(parseExprStmt());
    }
    else if (isInFirstSet(token, forStmtFirsts))
    {
        acceptNode(parseForStmt());
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
    else if (token == '}' && depth != 0)
    {
        reject();
    }

    error("unexpected token");
}

// exprStmt -> expr ;                       - firsts: NUMBER, IDENT, STRING, (, [
//           | LET IDENT = expr ;      - firsts: LET
//           | CONST IDENT = expr ;    - firsts: CONST
//           | PRINT ( exprList ) ;         - firsts: PRINT
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
    else if (isInFirstSet(token, printStmtFirsts))
    {
        acceptNode(parsePrintStmt());
    }
    else if (isInFirstSet(token, exprFirsts))
    {
        auto exprResult = parseExpr();
        if (!exprResult.success)
        {
            reject();
        }

        expectToken(';');

        accept(exprResult.node);
    }

    reject();
}

// forStmt -> FOR ( exprStmt exprStmt expr ) stmt
Result<ForStatementNode> Parser::parseForStmt()
{
    Token forToken = expectToken(Token::FOR);

    Token openParenToken = expectToken('(');

    auto initResult = parseExprStmt();
    if (!initResult.success)
    {
        reject();
    }

    auto conditionResult = parseExprStmt();
    if (!conditionResult.success)
    {
        reject();
    }

    auto incrementResult = parseExpr();
    if (!incrementResult.success)
    {
        reject();
    }

    Token closeParenToken = expectToken(')');

    auto bodyResult = parseStmt();
    if (!bodyResult.success)
    {
        reject();
    }

    auto forNode = make_shared<ForStatementNode>(initResult.node, conditionResult.node, incrementResult.node, bodyResult.node);
    forNode->setRangeStart(forToken.getRange().getStart());

    accept(forNode);
}

// whileStmt -> WHILE ( expr ) stmt
Result<WhileNode> Parser::parseWhileStmt()
{
    Token whileToken = expectToken(Token::WHILE);

    Token openParenToken = expectToken('(');

    auto exprResult = parseExpr();
    if (!exprResult.success)
    {
        reject();
    }

    Token closeParenToken = expectToken(')');

    auto bodyResult = parseStmt();
    if (!bodyResult.success)
    {
        reject();
    }

    auto whileNode = make_shared<WhileNode>(exprResult.node, bodyResult.node);
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
    if (!exprResult.success)
    {
        reject();
    }

    Token closeParenToken = expectToken(')');

    auto thenStmtResult = parseStmt();
    if (!thenStmtResult.success)
    {
        reject();
    }

    shared_ptr<StatementNode> elseBranch = nullptr;
    Token token = peekToken();
    if (token == Token::ELSE)
    {
        advanceToken(); // consume 'else'
        auto elseStmtResult = parseStmt();
        if (!elseStmtResult.success)
        {
            reject();
        }
        elseBranch = elseStmtResult.node;
    }

    auto ifStmt = make_shared<IfStatementNode>(exprResult.node, thenStmtResult.node, elseBranch);
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
    if (!stmtsResult.success)
    {
        reject();
    }

    Token closeCurlyToken = expectToken('}');

    depth--;

    auto blockNode = make_shared<BlockNode>(stmtsResult.node);
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
        if (!result.success)
        {
            reject();
        }
        params = result.node;
    }

    Token closeParenToken = expectToken(')');

    auto bodyResult = parseStmt();
    if (!bodyResult.success)
    {
        reject();
    }

    auto funcDecl = make_shared<FuncDeclNode>(identifier, params, bodyResult.node);
    funcDecl->setRangeStart(fnToken.getRange().getStart());
    //funcDecl->setRangeEnd(closeParenToken.getRange().getEnd());

    accept(funcDecl);
}

// paramList -> IDENT paramList'
//            | ε
Result<ParamListNode> Parser::parseParamList()
{
    Token identifier = expectToken(Token::IDENT);

    auto param = make_shared<VarDeclNode>(identifier);
    param->setRangeStart(identifier.getRange().getStart());

    auto result = parseParamListP(param);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
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
    if (!result.success)
    {
        reject();
    }

    auto params = make_shared<ParamListNode>(lhs);
    params->addAllParams(result.node);
    params->setRangeEnd(param->getRange().getEnd());

    accept(params);
}

// returnStmt -> RETURN expr ;
//             | RETURN ;
Result<ReturnStatementNode> Parser::parseReturnStmt()
{
    Token returnToken = expectToken(Token::RETURN);

    auto exprResult = parseExpr();
    if (!exprResult.success)
    {
        reject();
    }

    Token semicolonToken = expectToken(';');

    auto returnStmt = make_shared<ReturnStatementNode>(exprResult.node);
    returnStmt->setRangeStart(returnToken.getRange().getStart());
    returnStmt->setRangeEnd(semicolonToken.getRange().getEnd());

    accept(returnStmt);
}

Result<BreakNode> Parser::parseBreakStmt()
{
    Token breakToken = expectToken(Token::BREAK);

    Token semicolonToken = expectToken(';');

    auto breakNode = make_shared<BreakNode>(breakToken);
    breakNode->setRangeStart(breakToken.getRange().getStart());
    breakNode->setRangeEnd(semicolonToken.getRange().getEnd());

    accept(breakNode);
}

// letStmt -> LET IDENT = expr ;
Result<VarDeclNode> Parser::parseLetStmt()
{
    Token letToken = expectToken(Token::LET);

    Token identifier = expectToken(Token::IDENT);

    Token equalsToken = expectToken('=');

    auto exprResult = parseExpr();
    if (!exprResult.success)
    {
        reject();
    }

    Token semicolonToken = expectToken(';');

    // Create a variable declaration node with the identifier and the expression
    auto varDecl = make_shared<VarDeclNode>(identifier, exprResult.node);
    varDecl->setRangeStart(letToken.getRange().getStart());
    varDecl->setRangeEnd(semicolonToken.getRange().getEnd());

    accept(varDecl);
}

// constStmt -> CONST IDENT = expr ;
Result<VarDeclNode> Parser::parseConstStmt()
{
    Token constToken = expectToken(Token::CONST);

    Token identifier = expectToken(Token::IDENT);

    Token equalsToken = expectToken('=');

    auto exprResult = parseExpr();
    if (!exprResult.success)
    {
        reject();
    }

    Token semicolonToken = expectToken(';');

    // Create a variable declaration node with the identifier and the expression
    auto varDecl = make_shared<VarDeclNode>(identifier, exprResult.node, true);
    varDecl->setRangeStart(constToken.getRange().getStart());
    varDecl->setRangeEnd(semicolonToken.getRange().getEnd());

    accept(varDecl);
}

// printStmt -> PRINT ( expr ) ;
Result<PrintStatementNode> Parser::parsePrintStmt()
{
    Token printToken = expectToken(Token::PRINT);

    Token openParanToken = expectToken('(');

    shared_ptr<ExpressionNode> exprNode = nullptr;
    Token token = peekToken();
    if (token != ')')
    {
        auto exprResult = parseExpr();
        if (!exprResult.success)
        {
            reject();
        }
        exprNode = exprResult.node;
    }

    Token closeParanToken = expectToken(')');

    Token semicolonToken = expectToken(';');

    auto printStmt = make_shared<PrintStatementNode>(exprNode);
    printStmt->setRangeStart(printToken.getRange().getStart());
    printStmt->setRangeEnd(semicolonToken.getRange().getEnd());
    accept(printStmt);
}

//******************************************************************************************

//***************************************************
// expr -> assign expr'
Result<ExpressionNode> Parser::parseExpr()
{
    auto result = parseAssign();
    if (!result.success)
    {
        reject();
    }

    result = parseExprP(result.node);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
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
    if (!rhs.success)
    {
        reject();
    }

    auto exprP = parseExprP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.node));
    if (!exprP.success)
    {
        reject();
    }

    accept(exprP.node);
}

//***************************************************
// assign -> or assign'
Result<ExpressionNode> Parser::parseAssign()
{
    auto result = parseOr();
    if (!result.success)
    {
        reject();
    }

    result = parseAssignP(result.node);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
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
    if (!rhs.success)
    {
        reject();
    }

    rhs = parseAssignP(make_shared<AssignNode>(lhs, token, rhs.node));
    if (!rhs.success)
    {
        reject();
    }

    accept(rhs.node);
}

//***************************************************
// or -> and or'
Result<ExpressionNode> Parser::parseOr()
{
    auto result = parseAnd();
    if (!result.success)
    {
        reject();
    }

    result = parseOrP(result.node);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
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
    if (!rhs.success)
    {
        reject();
    }

    rhs = parseOrP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.node));
    if (!rhs.success)
    {
        reject();
    }

    accept(rhs.node);
}

//***************************************************
// and -> equality and'
Result<ExpressionNode> Parser::parseAnd()
{
    auto result = parseEquality();
    if (!result.success)
    {
        reject();
    }

    result = parseAndP(result.node);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
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
    if (!rhs.success)
    {
        reject();
    }

    rhs = parseAndP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.node));
    if (!rhs.success)
    {
        reject();
    }

    accept(rhs.node);
}

//***************************************************
// equality -> relation equality'
Result<ExpressionNode> Parser::parseEquality()
{
    auto result = parseRelation();
    if (!result.success)
    {
        reject();
    }

    result = parseEqualityP(result.node);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
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
    if (!rhs.success)
    {
        reject();
    }

    rhs = parseEqualityP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.node));
    if (!rhs.success)
    {
        reject();
    }

    accept(rhs.node);
}

//***************************************************
// relation -> addit relation'
Result<ExpressionNode> Parser::parseRelation()
{
    auto result = parseAddit();
    if (!result.success)
    {
        reject();
    }

    result = parseRelationP(result.node);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
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
    if (!rhs.success)
    {
        reject();
    }

    rhs = parseRelationP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.node));
    if (!rhs.success)
    {
        reject();
    }

    accept(rhs.node);
}

//***************************************************
// addit -> mult addit'
Result<ExpressionNode> Parser::parseAddit()
{
    auto result = parseMult();
    if (!result.success)
    {
        reject();
    }

    result = parseAdditP(result.node);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
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
    if (!rhs.success)
    {
        reject();
    }

    rhs = parseAdditP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.node));
    if (!rhs.success)
    {
        reject();
    }

    accept(rhs.node);
}

//***************************************************
// mult -> unary mult'
Result<ExpressionNode> Parser::parseMult()
{
    auto result = parseUnary();
    if (!result.success)
    {
        reject();
    }

    result = parseMultP(result.node);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
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
    if (!rhs.success)
    {
        reject();
    }

    rhs = parseMultP(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), rhs.node));
    if (!rhs.success)
    {
        reject();
    }

    accept(rhs.node);
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
    if (!result.success)
    {
        reject();
    }

    accept(make_shared<BinaryExprNode>(nullptr, make_shared<OpNode>(token), result.node));
}

//***************************************************
// argList -> assign argListP
Result<ArgListNode> Parser::parseArgList()
{
    auto assignResult = parseAssign();
    if (!assignResult.success)
    {
        reject();
    }

    auto listResult = parseArgListP(assignResult.node);
    if (!listResult.success)
    {
        reject();
    }

    auto argList = make_shared<ArgListNode>();
    argList->addAllArgs(listResult.node);

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
    if (!assignResult.success)
    {
        reject();
    }

    auto listResult = parseArgListP(assignResult.node);
    if (!listResult.success)
    {
        reject();
    }

    auto argList = make_shared<ArgListNode>(lhs);
    argList->addAllArgs(listResult.node);

    accept(argList);
}


//***************************************************
// post -> primary post''
Result<ExpressionNode> Parser::parsePost()
{
    auto result = parsePrimary();
    if (!result.success)
    {
        reject();
    }

    result = parsePostPP(result.node);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
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
        Token openToken = token;
        advanceToken(); // consume '['

        auto exprResult = parseExpr();
        if (!exprResult.success)
        {
            reject();
        }

        Token closeBracketToken = expectToken(']');

        accept(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), exprResult.node));
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
        if (!argListResult.success)
        {
            reject();
        }

        Token closeParenToken = expectToken(')');

        auto callNode = make_shared<CallNode>(lhs, argListResult.node);
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
        accept(make_shared<BinaryExprNode>(lhs, make_shared<OpNode>(token), nullptr));
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
    if (!result.success)
    {
        reject();
    }

    result = parsePostPP(result.node);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
}

//***************************************************
// primary -> ( expr )
//          | [ expr ]
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
        if (!exprResult.success)
        {
            reject();
        }

        Token closeParenToken = expectToken(')');

        accept(exprResult.node);
    }
    else if (token == '[')
    {
        advanceToken(); // consume '['

        auto exprResult = parseExpr();
        if (!exprResult.success)
        {
            reject();
        }

        Token closeBracketToken = expectToken(']');

        accept(exprResult.node);
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
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

#define error(msg) \
    tokenError(msg, currentToken, __FILE__, __LINE__); \
    hadError = true; \
    reject()

#define expected(expected) \
    error("expected '" + string(expected) + "' but found '" + currentToken.toString() + "'")

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

#define expectToken(x) \
    if (peekToken() != x) { \
        expected(x); \
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
        if (result.node)
        {
            delete result.node; // Clean up the node if it was created
        }
        reject();
    }
    accept(result.node);
}

// stmts -> stmts stmt
//        | stmt
Result<StatementsNode> Parser::parseStmts()
{
    StatementsNode *statements = new StatementsNode();
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
        //acceptNode(parseForStmt());
    }
    else if (isInFirstSet(token, whileStmtFirsts))
    {
        //acceptNode(parseWhileStmt());
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

        token = peekToken();
        if (token != ';')
        {
            expected(";");
        }
        advanceToken(); // consume ';'

        accept(exprResult.node);
    }

    reject();
}

// ifStmt -> IF ( expr ) stmt
//         | IF ( expr ) stmt ELSE stmt
Result<IfStatementNode> Parser::parseIfStmt()
{
    Token token = peekToken();
    if (token != Token::IF)
    {
        expected("if");
    }
    Token ifToken = token;
    advanceToken(); // consume 'if'

    token = peekToken();
    if (token != '(')
    {
        expected("(");
    }
    advanceToken(); // consume '('

    auto exprResult = parseExpr();
    if (!exprResult.success)
    {
        reject();
    }

    token = peekToken();
    if (token != ')')
    {
        expected(")");
    }
    advanceToken(); // consume ')'

    auto thenStmtResult = parseStmt();
    if (!thenStmtResult.success)
    {
        reject();
    }

    StatementNode *elseBranch = nullptr;
    token = peekToken();
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

    IfStatementNode *ifStmt = new IfStatementNode(exprResult.node, thenStmtResult.node, elseBranch);
    ifStmt->setRangeStart(ifToken.getRange().getStart());

    accept(ifStmt);
}

Result<BlockNode> Parser::parseBlock()
{
    Token token = peekToken();
    if (token != '{')
    {
        expected("{");
    }
    depth++;
    Token blockStart = token;
    advanceToken(); // consume '{'

    token = peekToken();
    if (token == '}')
    {
        advanceToken(); // consume '}'
        depth--;
        BlockNode *emptyBlock = new BlockNode();
        emptyBlock->setRangeStart(blockStart.getRange().getStart());
        emptyBlock->setRangeEnd(token.getRange().getEnd());
        accept(emptyBlock); // Empty block
    }

    auto stmtsResult = parseStmts();
    if (!stmtsResult.success)
    {
        reject();
    }

    token = peekToken();
    if (token != '}')
    {
        expected("}");
    }
    advanceToken(); // consume '}'
    depth--;

    BlockNode *blockNode = new BlockNode(stmtsResult.node);
    blockNode->setRangeStart(blockStart.getRange().getStart());
    blockNode->setRangeEnd(token.getRange().getEnd());

    accept(blockNode);
}

// funcDecl -> FN IDENT ( argList ) stmt ;
//           | FN IDENT ( ) stmt;
Result<FuncDeclNode> Parser::parseFuncDecl()
{
    Token token = peekToken();
    if (token != Token::FN)
    {
        expected("fn");
    }
    Token fnToken = token;
    advanceToken(); // consume 'fn'

    token = peekToken();
    if (token.getType() != Token::IDENT)
    {
        expected("identifier");
    }
    Token identifier = token;
    advanceToken(); // consume identifier

    token = peekToken();
    if (token != '(')
    {
        expected("(");
    }
    advanceToken(); // consume '('

    token = peekToken();

    ParamListNode *params = new ParamListNode();
    if (token != ')')
    {
        auto result = parseParamList();
        if (!result.success)
        {
            reject();
        }
        params = result.node;
    }

    token = peekToken();
    if (token != ')')
    {
        expected(")");
    }
    advanceToken(); // consume ')'

    auto bodyResult = parseStmt();
    if (!bodyResult.success)
    {
        reject();
    }

    FuncDeclNode *funcDecl = new FuncDeclNode(identifier, params, bodyResult.node);
    funcDecl->setRangeStart(fnToken.getRange().getStart());

    accept(funcDecl);
}

// paramList -> IDENT paramList'
//            | ε
Result<ParamListNode> Parser::parseParamList()
{
    Token token = peekToken();
    if (token.getType() != Token::IDENT)
    {
        expected("identifier");
    }
    Token identifier = token;
    advanceToken(); // consume identifier

    VarDeclNode *param = new VarDeclNode(identifier);
    param->setRangeStart(identifier.getRange().getStart());

    auto result = parseParamListP(param);
    if (!result.success)
    {
        reject();
    }

    accept(result.node);
}

Result<ParamListNode> Parser::parseParamListP(VarDeclNode *lhs)
{
    Token token = peekToken();
    if (token != ',')
    {
        ParamListNode *params = new ParamListNode(lhs);
        params->setRangeEnd(lhs->getRange().getEnd());
        accept(params);
    }
    advanceToken(); // consume ','

    token = peekToken();
    if (token.getType() != Token::IDENT)
    {
        expected("identifier");
    }
    Token identifier = token;
    advanceToken(); // consume identifier

    VarDeclNode *param = new VarDeclNode(identifier);
    param->setRangeStart(identifier.getRange().getStart());

    auto result = parseParamListP(param);
    if (!result.success)
    {
        reject();
    }

    ParamListNode *params = new ParamListNode(lhs);
    params->addParam(param);
    params->setRangeEnd(param->getRange().getEnd());

    accept(params);
}

Result<ReturnStatementNode> Parser::parseReturnStmt()
{
    Token token = peekToken();
    if (token != Token::RETURN)
    {
        expected("return");
    }
    Token returnToken = token;
    advanceToken(); // consume 'return'

    auto exprResult = parseExpr();
    if (!exprResult.success)
    {
        reject();
    }

    token = peekToken();
    if (token != ';')
    {
        expected(";");
    }
    advanceToken(); // consume ';'

    ReturnStatementNode *returnStmt = new ReturnStatementNode(exprResult.node);
    returnStmt->setRangeStart(returnToken.getRange().getStart());
    returnStmt->setRangeEnd(token.getRange().getEnd());

    accept(returnStmt);
}

// letStmt -> LET IDENT = expr ;
Result<VarDeclNode> Parser::parseLetStmt()
{
    Token token = peekToken();
    if (token != Token::LET)
    {
        expected("let");
    }
    Token let = token;
    advanceToken(); // consume 'let'

    token = peekToken();
    if (token.getType() != Token::IDENT)
    {
        expected("identifier");
    }
    Token identifier = token;
    advanceToken(); // consume identifier

    token = peekToken();
    if (token != '=')
    {
        expected("=");
    }
    advanceToken(); // consume '='

    auto exprResult = parseExpr();
    if (!exprResult.success)
    {
        reject();
    }

    token = peekToken();
    if (token != ';')
    {
        expected(";");
    }
    advanceToken(); // consume ';'

    // Create a variable declaration node with the identifier and the expression
    VarDeclNode *varDecl = new VarDeclNode(identifier, exprResult.node);
    varDecl->setRangeStart(let.getRange().getStart());

    accept(varDecl);
}

Result<VarDeclNode> Parser::parseConstStmt()
{
    Token token = peekToken();
    if (token != Token::CONST)
    {
        reject();
    }
    Token constToken = token;
    advanceToken(); // consume 'const'

    token = peekToken();
    if (token.getType() != Token::IDENT)
    {
        expected("identifier");
    }
    Token identifier = token;
    advanceToken(); // consume identifier

    token = peekToken();
    if (token != '=')
    {
        expected("=");
    }
    advanceToken(); // consume '='

    auto exprResult = parseExpr();
    if (!exprResult.success)
    {
        reject();
    }

    token = peekToken();
    if (token != ';')
    {
        expected(";");
    }
    advanceToken(); // consume ';'

    // Create a variable declaration node with the identifier and the expression
    VarDeclNode *varDecl = new VarDeclNode(identifier, exprResult.node, true);
    varDecl->setRangeStart(constToken.getRange().getStart());

    accept(varDecl);
}

Result<PrintStatementNode> Parser::parsePrintStmt()
{
    Token token = peekToken();
    if (token != Token::PRINT)
    {
        expected("print");
    }
    Token printToken = token;
    advanceToken(); // consume

    token = peekToken();
    if (token != '(')
    {
        expected("(");
    }
    advanceToken(); // consume '('

    auto exprResult = parseExpr();
    if (!exprResult.success)
    {
        reject();
    }

    token = peekToken();
    if (token != ')')
    {
        expected(")");
    }
    advanceToken(); // consume ')'

    token = peekToken();
    if (token != ';')
    {
        expected(";");
    }
    Token semicolonToken = token;
    advanceToken(); // consume ';'

    PrintStatementNode *printStmt = new PrintStatementNode(exprResult.node);
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
Result<ExpressionNode> Parser::parseExprP(ExpressionNode *lhs)
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

    auto exprP = parseExprP(new BinaryExprNode(lhs, new OpNode(token), rhs.node));
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
//          | nothing
Result<ExpressionNode> Parser::parseAssignP(ExpressionNode *lhs)
{
    Token token = peekToken();
    if (token != '=')
    {
        accept(lhs);
    }
    advanceToken(); // consume 'or'

    auto rhs = parseOr();
    if (!rhs.success)
    {
        reject();
    }

    rhs = parseAssignP(new AssignNode(lhs, rhs.node));
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
Result<ExpressionNode> Parser::parseOrP(ExpressionNode *lhs)
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

    rhs = parseOrP(new BinaryExprNode(lhs, new OpNode(token), rhs.node));
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
Result<ExpressionNode> Parser::parseAndP(ExpressionNode *lhs)
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

    rhs = parseAndP(new BinaryExprNode(lhs, new OpNode(token), rhs.node));
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
Result<ExpressionNode> Parser::parseEqualityP(ExpressionNode *lhs)
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

    rhs = parseEqualityP(new BinaryExprNode(lhs, new OpNode(token), rhs.node));
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
Result<ExpressionNode> Parser::parseRelationP(ExpressionNode *lhs)
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

    rhs = parseRelationP(new BinaryExprNode(lhs, new OpNode(token), rhs.node));
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
Result<ExpressionNode> Parser::parseAdditP(ExpressionNode *lhs)
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

    rhs = parseAdditP(new BinaryExprNode(lhs, new OpNode(token), rhs.node));
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
Result<ExpressionNode> Parser::parseMultP(ExpressionNode *lhs)
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

    rhs = parseMultP(new BinaryExprNode(lhs, new OpNode(token), rhs.node));
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

    accept(new BinaryExprNode(nullptr, new OpNode(token), result.node));
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

    ArgListNode *argList = new ArgListNode();
    argList->addAllArgs(listResult.node);

    accept(argList);
}

// argList' -> , assign argListP
//           | nothing
Result<ArgListNode> Parser::parseArgListP(ExpressionNode *lhs)
{
    Token token = peekToken();
    if (token != ',')
    {
        accept(new ArgListNode(lhs));
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

    ArgListNode *argList = new ArgListNode(lhs);
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
Result<ExpressionNode> Parser::parsePostP(ExpressionNode *lhs)
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

        token = peekToken();
        if (token != ']')
        {
            expected("]");
        }
        advanceToken(); // consume ']'

        accept(new BinaryExprNode(lhs, new OpNode(token), exprResult.node));
    }
    else if (token == '(')
    {
        Token openToken = token;
        advanceToken(); // consume '('

        token = peekToken();

        if (token == ')')
        {
            advanceToken(); // consume ')'
            accept(new CallNode(lhs));
        }

        auto argListResult = parseArgList();
        if (!argListResult.success)
        {
            reject();
        }

        token = peekToken();
        if (token != ')')
        {
            expected(")");
        }
        advanceToken(); // consume ')'

        accept(new CallNode(lhs, argListResult.node));
    }
    else if (token == '.')
    {
        advanceToken(); // consume '.'

        token = peekToken();
        if (token.getType() != Token::IDENT)
        {
            expected("identifier");
        }
        advanceToken(); // consume identifier

        accept(new MemberAccessNode(lhs, token));
    }
    else if (token == Token::INC || token == Token::DEC)
    {
        advanceToken(); // consume 'inc' or 'dec'
        accept(new BinaryExprNode(lhs, new OpNode(token), nullptr));
    }

    reject();
}
// post'' -> post' post''
//         | ϵ
Result<ExpressionNode> Parser::parsePostPP(ExpressionNode *lhs)
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

        token = peekToken();
        if (token != ')')
        {
            expected(")");
        }
        advanceToken(); // consume ')'

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

        token = peekToken();
        if (token != ']')
        {
            expected("]");
        }
        advanceToken(); // consume ']'

        accept(exprResult.node);
    }
    else if (token.getType() == Token::IDENT)
    {
        advanceToken(); // consume identifier
        accept(new VarExprNode(token));
    }
    else if (token.getType() == Token::NUMBER)
    {
        advanceToken(); // consume number
        accept(new NumberNode(token));
    }
    else if (token.getType() == Token::STRING)
    {
        advanceToken(); // consume string
        accept(new StringNode(token));
    }

    expected("primary expression");
}
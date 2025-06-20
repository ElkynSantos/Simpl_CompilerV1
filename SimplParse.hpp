#ifndef _PARSE_H_
#define _PARSE_H_
#include <iostream>
#include "tokens.hpp"
#include <vector>
#include "SimplLexer.hpp"
#include "SimplAst.hpp"


class SimplParser {
    public:
        SimplParser(SimplLexer& lexer): lexer(lexer), currentToken(Token::Eof){}
        programNode* parse();

    private:
        SimplLexer& lexer;
        Token currentToken;
        programNode* program();
        GlobalVarDeclNode* globalVarDeclare();
        TypeNameNode* type();
        std::vector<AstNode*> arrayinitializer();
        AstNode* expression();
        AstNode* booleanExpression();
        AstNode* booleanTerm();
        AstNode* booleanFactor();
        AstNode* relationalExpression();
        AstNode* arithmeticExpression();
        AstNode* term();
        AstNode* factor();
        AstNode* primary();

        sizeExpressionNode* sizeExpression();
        std::vector<AstNode*> argumentList();

        GlobalFnDeclareNode* globalFnDeclare();
        ParameterList*  paramList();
        Parameter* param();
        TypeNameFunctionNode* functionTypes();
        std::vector<statementsNode*> statements();
        AstNode* statement();
        ConditionalStatement* conditionalStatement();
        ReturnStatement* returnStatement();
        AstNode* loop();
        AstNode* assignmentValues(); // here call function
        PrintStatement* print();
        void throwError(const std::vector<Token>& expectedTokens);

};

#endif
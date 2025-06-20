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
        bool parse();

    private:
        SimplLexer& lexer;
        Token currentToken;
        AstNode* program();
        AstNode* globalVarDeclare();
        AstNode* type();
        AstNode* sizeExpression();
        AstNode* arrayinitializer();
        AstNode* expression();
        AstNode* booleanExpression();
        AstNode* booleanTerm();
        AstNode* booleanFactor();
        AstNode* relationalExpression();
        AstNode* arithmeticExpression();
        AstNode* term();
        AstNode* factor();
        AstNode* primary();
        AstNode* argumentList();

        // void globalFnDeclare();
        // void paramList();
        // void param();
        // void functionTypes();
        // void statements();
        // void statement();
        // void conditionalStatement();
        // void argumentList();
        // void returnStatement();
        // void loop();
        // void assignmentValues();
        // void print();
        void throwError(const std::vector<Token>& expectedTokens);

};

#endif
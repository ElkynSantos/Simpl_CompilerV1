#ifndef _PARSE_H_
#define _PARSE_H_
#include <iostream>
#include "tokens.hpp"
#include <vector>
#include "SimplLexer.hpp"


class SimplParser {
    public:
        SimplParser(SimplLexer& lexer): lexer(lexer), currentToken(Token::Eof){}
        bool parse();

    private:
        SimplLexer& lexer;
        Token currentToken;
        void program();
        void globalVarDeclare();
        void globalFnDeclare();
        void type();
        void sizeExpression();
        void arrayinitializer();
        void expression();
        void booleanExpression();
        void booleanTerm();
        void booleanFactor();
        void relationalExpression();
        void arithmeticExpression();
        void term();
        void factor();
        void primary();

        void globalFunctionDeclare();


        void throwError(const std::vector<Token>& expectedTokens);

};

#endif
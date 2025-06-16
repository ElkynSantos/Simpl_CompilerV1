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
        void express();
        void program();
        void globalVarDeclare();
        void type();
        void sizeExpression();
        void arrayinitializer();
        void expression();
        void primaryExpression();
        void booleanExpression();
        void comparisonExpression();
        void arrayAccess();
        void argumentList();
        void functionCall();
        void arithmeticExpression();
        void globalFunctionDeclare();
        void term();
        void factor();

        void throwError(const std::vector<Token>& expectedTokens);

};

#endif
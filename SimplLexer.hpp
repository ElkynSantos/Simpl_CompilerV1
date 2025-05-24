#ifndef _LEXER_H_
#define _LEXER_H_
#include <string>
#include <optional>
#include "tokens.hpp"

class SimplLexer {
    public: 
        int pos;
        std::string input;
        std::string text;
        SimplLexer(std::string input) : pos(0), input(input) {
            text = "";
        }
        ~SimplLexer() = default;
        Token getNextToken();
        std::string tokenToString(Token tk);
    private:
        int getNextChar();
        void ungetChar(int ch);
        void reportError(int ch);
       
        

};
#endif
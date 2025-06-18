#ifndef _LEXER_H_
#define _LEXER_H_
#include <string>
#include <optional>
#include <unordered_map>
#include "tokens.hpp"
#include <cctype>

class SimplLexer {
    public: 
        int pos;
        std::string input;
        std::string text;
        SimplLexer(std::string input) : pos(0), input(input) {
            text = "";
            initKeywords();
        }
        ~SimplLexer() = default;
        Token getNextToken();
        std::string tokenToString(Token tk);
    private:
        int getNextChar();
        std::optional<Token> keywordToken(const std::string& text);
        std::unordered_map<std::string, Token> keywords;
        void ungetChar(int ch);
        void reportError(int ch);
        void initKeywords();    

};
#endif
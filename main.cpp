#include <iostream>
#include <string.h>
#include "SimplLexer.hpp"
#include "tokens.hpp"

int main(){
    std::string input = R"( 
    987 assd
    identi fic ador 123123
    )";
    
    SimplLexer lexer(input);

    Token tk = lexer.getNextToken();

    while(tk != Token::Eof) {
        std::cout << lexer.tokenToString(tk) <<" : "<<lexer.text<< std::endl;
        tk = lexer.getNextToken();
    }
    return 0;
}
#include <iostream>
#include <string.h>
#include "SimplLexer.hpp"
#include "tokens.hpp"

int main(){
    std::string input = R"( 
    987 assd
    /* iden
    ti fica 92231313dsadsa 
      */ 
    ador 123123
    // this is a comment
    /
    /9
    /a
    /* THIS IS A LINES  
    13$#$#$#@$%$% COMMENT */
    asda dada
    ++*-
    -
    *
    %
    ==
    !=
    =
    >=>
    <=
    <
    x = x
    if
    else
    while
    true
    false
    int
    )";
    
    SimplLexer lexer(input);

    Token tk = lexer.getNextToken();


    while(tk != Token::Eof) {
        std::cout << lexer.tokenToString(tk) <<" : "<<lexer.text<< std::endl;
        tk = lexer.getNextToken();
    }
    return 0;
}

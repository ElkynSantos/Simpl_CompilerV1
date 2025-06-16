#include <iostream>
#include "SimplParse.hpp"
#include "SimplLexer.hpp"
#include <stdexcept>
#include "tokens.hpp"

bool SimplParser::parse() {
    currentToken = lexer.getNextToken();

    while (currentToken != Token::Eof) {
        program();
    }
    return true;
}


void SimplParser::program(){

    globalVarDeclare();
    //globalFnDeclare();

}

void SimplParser :: globalVarDeclare(){
    
    if( currentToken == Token::KwLet){
       
        currentToken = lexer.getNextToken();

        if( currentToken != Token::Ident){
            throwError({Token::Ident});
        }

        currentToken = lexer.getNextToken();

        if(currentToken != Token::Colon){
            throwError({Token::Colon});    
        }

        currentToken = lexer.getNextToken();
        type();
        if(currentToken == Token::Semicolon){ 
            currentToken = lexer.getNextToken();
            return;
        }
        
        if(currentToken == Token::Assignment){
            currentToken = lexer.getNextToken();
            expression();
            if(currentToken != Token::Semicolon) {
                throwError({Token::Semicolon});
            
            }
            currentToken = lexer.getNextToken();
            return;
        }
        else if (currentToken == Token::BracketLeft){
            currentToken = lexer.getNextToken();
            sizeExpression();

            if(currentToken != Token::BracketRight) throwError({Token::BracketRight});
            
            currentToken = lexer.getNextToken();

            if(currentToken == Token::Semicolon){ 
        
                currentToken = lexer.getNextToken();
                return;
            }   

            if(currentToken != Token::Assignment) throwError({Token::Assignment});

            currentToken = lexer.getNextToken();

            if(currentToken != Token :: BracketLeft) throwError({Token::BracketLeft});
            
            currentToken = lexer.getNextToken();
            arrayinitializer();

            if(currentToken != Token :: BracketRight) throwError({Token::BracketRight});
            currentToken = lexer.getNextToken();
            
            if(currentToken != Token :: Semicolon) throwError({Token::Semicolon});
            currentToken = lexer.getNextToken();

            return;
        }else{
            throwError({Token::Assignment, Token::BracketLeft});
        }
    }
    

}


void SimplParser::globalFnDeclare() {
    if (currentToken == Token::KwFn) {
        currentToken = lexer.getNextToken();

        if (currentToken != Token::Ident) {
            throwError({Token::Ident});
        }

        currentToken = lexer.getNextToken();

        if (currentToken != Token::ParenthesisLeft) {
            throwError({Token::ParenthesisLeft});
        }

        currentToken = lexer.getNextToken();

        
    } else {
        throwError({Token::KwFn});
    }
}

void SimplParser::arrayinitializer() {
    expression();
    while (currentToken == Token::Comma) {
        currentToken = lexer.getNextToken();
        expression();
    }
}
void SimplParser::type(){
    if(currentToken == Token::KwInt || currentToken == Token::KwBool){
        currentToken = lexer.getNextToken();
    }else{
         throwError({Token::KwInt, Token::KwBool});
    }
}

void SimplParser::sizeExpression(){
    expression();
}

void SimplParser::expression() {
   booleanExpression();
}

void SimplParser :: booleanExpression(){
    booleanTerm();
    while (currentToken == Token::KwOr) {
        currentToken = lexer.getNextToken();
        booleanTerm();
    }
}

void SimplParser::booleanTerm() {
    booleanFactor();
    while (currentToken == Token::kwAnd) {
        currentToken = lexer.getNextToken();
        booleanFactor();
    }
    
}

void SimplParser::booleanFactor() {
    if(currentToken == Token::KwNot){
        currentToken = lexer.getNextToken();
        booleanFactor();
    } 

    relationalExpression();
}

void SimplParser::relationalExpression() {
   arithmeticExpression();
   if(currentToken == Token::Equal || currentToken == Token::NotEqual ||
      currentToken == Token::LessThan || currentToken == Token::LessThanOrEqualTo ||
      currentToken == Token::GreaterThan || currentToken == Token::GreaterThanOrEqualTo) {
        currentToken = lexer.getNextToken();
        arithmeticExpression();
    }
}

void SimplParser::arithmeticExpression() {
   term();
    while (currentToken == Token::Addition || currentToken == Token::Subtraction) {
        currentToken = lexer.getNextToken();
        term();
    }
}

void SimplParser::throwError(const std::vector<Token>& expectedTokens) {
    std::string errorMessage = "Syntax Error, expected one of -> ";
    for (size_t i = 0; i < expectedTokens.size(); ++i) {
        errorMessage += "'" + lexer.tokenToString(expectedTokens[i]) + "'";
        if (i < expectedTokens.size() - 1) {
            errorMessage += ", ";
        }
    }
    throw std::runtime_error(errorMessage);
}

void SimplParser::term(){

    factor();

    while(currentToken == Token::Multiplication || currentToken == Token::Module || currentToken == Token::Division){
        currentToken = lexer.getNextToken();
        factor();
    }
    
}

void SimplParser::factor(){

    if(currentToken == Token::Addition || currentToken == Token::Subtraction){
        currentToken = lexer.getNextToken();
        primary();
          
    }

    primary();

}

void SimplParser::primary(){
    if(currentToken == Token::Number || currentToken == Token::KwTrue || currentToken == Token::KwFalse){
        currentToken = lexer.getNextToken();
    }
    else if(currentToken == Token::Ident){
        currentToken = lexer.getNextToken();

        if(currentToken == Token::BracketLeft){
            currentToken = lexer.getNextToken();
            expression();
            if(currentToken != Token::BracketRight) {
                throwError({Token::BracketRight});
            }
            currentToken = lexer.getNextToken();

        }

        if(currentToken == Token::ParenthesisLeft){
            currentToken = lexer.getNextToken();
            expression();

            while(currentToken == Token::Comma) {
                currentToken = lexer.getNextToken();
                expression();
            }
            if(currentToken != Token::ParenthesisRight) {
                throwError({Token::ParenthesisRight});
            }  
            currentToken = lexer.getNextToken();
        }
    }else if(currentToken == Token::ParenthesisLeft){
        currentToken = lexer.getNextToken();
        expression();
        if(currentToken != Token::ParenthesisRight) {
            throwError({Token::ParenthesisRight});
        }
        currentToken = lexer.getNextToken();
    }
}



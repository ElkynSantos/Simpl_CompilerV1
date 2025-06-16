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
        
        std::cout << "Type: " << lexer.tokenToString(currentToken) << std::endl;

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
            std::cout << "Type: " << lexer.tokenToString(currentToken) << std::endl;

            currentToken = lexer.getNextToken();
            sizeExpression();
            std::cout << "Type: " << lexer.tokenToString(currentToken) << std::endl;

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


void SimplParser::arrayinitializer() {
    std::cout << "Type: " << lexer.tokenToString(currentToken) << std::endl;

    if (currentToken == Token::Number || currentToken == Token::KwTrue || currentToken == Token::KwFalse) {
        currentToken = lexer.getNextToken();
    } else {
        expression();
    }

    while (currentToken == Token::Comma) {
        currentToken = lexer.getNextToken();

        if (currentToken == Token::Number || currentToken == Token::KwTrue || currentToken == Token::KwFalse) {
            currentToken = lexer.getNextToken();

        } else {
            expression();
        }
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
    primaryExpression();

    while (currentToken == Token::Addition || currentToken == Token::Subtraction ||
           currentToken == Token::Multiplication || currentToken == Token::Division ||
           currentToken == Token::Module || currentToken == Token::kwAnd ||
           currentToken == Token::KwOr || currentToken == Token::KwNot|| currentToken == Token::Equal ||
           currentToken == Token::NotEqual || currentToken == Token::LessThan ||
           currentToken == Token::LessThanOrEqualTo || currentToken == Token::GreaterThan ||
           currentToken == Token::GreaterThanOrEqualTo) {
        currentToken = lexer.getNextToken();
        primaryExpression();
    }
}

void SimplParser::primaryExpression() {
    if (currentToken == Token::ParenthesisLeft) {
        currentToken = lexer.getNextToken();

        expression();

        if (currentToken != Token::ParenthesisRight) {
            throw std::runtime_error("Syntax Error: Expected ')'");
        }
        currentToken = lexer.getNextToken();
    } else if (currentToken == Token::Number ||
               currentToken == Token::KwTrue || currentToken == Token::KwFalse) {
        currentToken = lexer.getNextToken();
    } else if (currentToken == Token::Ident) {
        currentToken = lexer.getNextToken();
        if (currentToken == Token::BracketLeft) {
            arrayAccess();
        } else if (currentToken == Token::ParenthesisLeft) {
            functionCall(); 
        }
    } else {
        throw std::runtime_error("Syntax Error: Invalid primary expression");
    }
}



void SimplParser::comparisonExpression() {
    expression();
    if (currentToken == Token::Equal || currentToken == Token::NotEqual ||
        currentToken == Token::LessThan || currentToken == Token::LessThanOrEqualTo ||
        currentToken == Token::GreaterThan || currentToken == Token::GreaterThanOrEqualTo) {
        currentToken = lexer.getNextToken();
        expression();
    } else {
        throw std::runtime_error("Syntax Error: Expected comparison operator");
    }
}

void SimplParser::arrayAccess() {

    if (currentToken == Token::BracketLeft) {
        currentToken = lexer.getNextToken();
        expression();
        if (currentToken != Token::BracketRight) {
            throw std::runtime_error("Syntax Error: Expected ']'");
        }
        currentToken = lexer.getNextToken(); 
    } else {
        throw std::runtime_error("Syntax Error: Expected '[' after identifier");
    }
}


void SimplParser::functionCall() {

    if (currentToken == Token::ParenthesisLeft) {
        currentToken = lexer.getNextToken();
        if (currentToken != Token::ParenthesisRight) {
            argumentList();
        }
        if (currentToken != Token::ParenthesisRight) {
            throw std::runtime_error("Syntax Error: Expected ')'");
        }
        currentToken = lexer.getNextToken();
    } else {
        throw std::runtime_error("Syntax Error: Expected '(' for function call");
    }
    
}

void SimplParser::argumentList() {
    expression();
    while (currentToken == Token::Comma) {
        currentToken = lexer.getNextToken();
        expression();
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
void SimplParser::express(){

    term();

    while(currentToken == Token::Addition){
        currentToken = lexer.getNextToken();
        term();
    }
    
}

void SimplParser::term(){

    factor();

    while(currentToken == Token::Multiplication){
        currentToken = lexer.getNextToken();
        factor();
    }
    
}

void SimplParser::factor(){

    if(currentToken == Token::ParenthesisLeft){

        currentToken = lexer.getNextToken();
        express();
        if(currentToken != Token::ParenthesisRight){
            throw std::runtime_error(std::string("Syntax Error expected -> ')'"));
        }

    }else if(currentToken != Token::Number){
            throw std::runtime_error(std::string("Syntax Error expected -> 'Number'"));
    }

    currentToken = lexer.getNextToken();
    
}



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

    if(currentToken == Token::KwLet) {
        globalVarDeclare();

    }else if(currentToken == Token::KwFn) {
        globalFnDeclare();
    }

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
        }
        else if(currentToken == Token::Assignment){
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
    }else{
        throwError({Token::KwLet});
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
        paramList();
        if(currentToken != Token::ParenthesisRight) {
            throwError({Token::ParenthesisRight});
        }
        currentToken = lexer.getNextToken();

        if (currentToken != Token::AssignmentFunction) {
            throwError({Token::AssignmentFunction});
        }

        currentToken = lexer.getNextToken();

        functionTypes();

        if(currentToken != Token::KeyLeft) {
            throwError({Token::KeyLeft});
        }

        currentToken = lexer.getNextToken();
        
        statements();

        if( currentToken != Token::KeyRight) {
            throwError({Token::KeyRight});
        }

        currentToken = lexer.getNextToken();

    } else {
        throwError({Token::KwFn});
    }
}

void SimplParser::paramList() {

    if (currentToken == Token::Ident) {
        param();   
        while (currentToken == Token::Comma) {
            currentToken = lexer.getNextToken();
            param();
        }
    }

}

void SimplParser :: param(){

    if(currentToken == Token::Ident) {
        currentToken = lexer.getNextToken();
        if(currentToken != Token::Colon) {
            throwError({Token::Colon});
        }

        currentToken = lexer.getNextToken();

        if(currentToken == Token::KwRef) {
            currentToken = lexer.getNextToken();
        }
       
        type();

        if(currentToken == Token::BracketLeft) {
            currentToken = lexer.getNextToken();
            sizeExpression();
            if(currentToken != Token::BracketRight) {
                throwError({Token::BracketRight});
            }
            currentToken = lexer.getNextToken();
        }
           
    }else {
        throwError({Token::Ident});
    }

}

void SimplParser::functionTypes(){
    if(currentToken == Token::KwInt || currentToken == Token::KwBool || currentToken == Token::KwVoid){
        currentToken = lexer.getNextToken();
    }else{
        throwError({Token::KwInt, Token::KwBool, Token::KwVoid});
    }
}
void SimplParser::statements(){
    
    while(currentToken == Token::KwLet ||  currentToken == Token::Ident || currentToken == Token::KwIf || currentToken == Token::KwWhile || 
           currentToken == Token::KwFor || currentToken == Token::KwReturn || currentToken == Token::KwPrint){
        statement();
    }

}

void SimplParser::statement(){

    if(currentToken == Token::KwLet) {
        globalVarDeclare();
    } 
    else if (currentToken == Token::Ident) {
        // logic call function here
        assignmentValues();
    } 
    else if (currentToken == Token::KwIf) {
        conditionalStatement();
    } 
    else if (currentToken == Token::KwWhile || currentToken == Token::KwFor) {
        loop();
    }
    else if(currentToken == Token::KwPrint){
        print();
    } 
    else if (currentToken == Token::KwReturn) {
       returnStatement();
    }
    
}

void SimplParser :: print(){
    if(currentToken == Token::KwPrint){
        currentToken = lexer.getNextToken();
        if(currentToken != Token::ParenthesisLeft){
            throwError({Token :: ParenthesisLeft});
        }
        currentToken = lexer.getNextToken();
        if(currentToken != Token::StringLiteral || currentToken != Token::Ident){
            throwError({Token :: StringLiteral, Token::Ident});
        }
        currentToken = lexer.getNextToken();

        if(currentToken != Token::ParenthesisRight){
            throwError({Token :: ParenthesisRight});
        
        }
        currentToken = lexer.getNextToken();
        if(currentToken != Token::Semicolon){
            throwError({Token :: ParenthesisRight});

        }
        currentToken = lexer.getNextToken();


    }else{
        throwError({Token :: KwPrint});

    }
}

void SimplParser::returnStatement() {
    if (currentToken == Token::KwReturn) {
        currentToken = lexer.getNextToken();
        if (currentToken != Token::Semicolon) {
            expression();
            if (currentToken != Token::Semicolon) {
                throwError({Token::Semicolon});
            }
        }
        currentToken = lexer.getNextToken();
    } else {
        throwError({Token::KwReturn});
    }
}

void SimplParser::assignmentValues(){
    if(currentToken == Token::Ident) {
        currentToken = lexer.getNextToken();
        //functionCall
        if(currentToken == Token::ParenthesisLeft) {
            currentToken = lexer.getNextToken();

            argumentList();
            if(currentToken != Token::ParenthesisRight) {
                throwError({Token::ParenthesisRight});
            }
            currentToken = lexer.getNextToken();

            if(currentToken != Token::Semicolon) {
                throwError({Token::Semicolon});
            }
            currentToken = lexer.getNextToken();

            return;
        }
        else if(currentToken == Token::BracketLeft) {
            currentToken = lexer.getNextToken();
            sizeExpression();
            if(currentToken != Token::BracketRight) {
                throwError({Token::BracketRight});
            }
            currentToken = lexer.getNextToken();
        }

        if(currentToken == Token::Assignment) {
            currentToken = lexer.getNextToken();
            expression();
            if(currentToken != Token::Semicolon) {
                throwError({Token::Semicolon});
            }
            currentToken = lexer.getNextToken();
        }
    }else{
        throwError({Token::Ident});
    }
}

void SimplParser::argumentList() {
    expression();
    while (currentToken == Token::Comma) {
        currentToken = lexer.getNextToken();
        expression();
    }
}
void SimplParser::conditionalStatement() {
    if(currentToken == Token::KwIf) {
        currentToken = lexer.getNextToken();
        if(currentToken != Token::ParenthesisLeft) {
            throwError({Token::ParenthesisLeft});
        }
        currentToken = lexer.getNextToken();
        expression();
        if(currentToken != Token::ParenthesisRight) {
            throwError({Token::ParenthesisRight});
        }
        currentToken = lexer.getNextToken();
        
        if(currentToken != Token::KeyLeft) {
            throwError({Token::KeyLeft});
        }
        currentToken = lexer.getNextToken();

        statements();
        if(currentToken != Token::KeyRight) {
            throwError({Token::KeyRight});
        }

        currentToken = lexer.getNextToken();
        
       while (currentToken == Token::KwElse) {
            currentToken = lexer.getNextToken();

            if (currentToken == Token::KwIf) {
                currentToken = lexer.getNextToken();

                if (currentToken != Token::ParenthesisLeft) {
                    throwError({Token::ParenthesisLeft});
                }
                currentToken = lexer.getNextToken();

                expression(); 
                if (currentToken != Token::ParenthesisRight) {
                    throwError({Token::ParenthesisRight});
                }
                currentToken = lexer.getNextToken();

                if (currentToken != Token::KeyLeft) {
                    throwError({Token::KeyLeft});
                }
                currentToken = lexer.getNextToken();

                statements();

                if (currentToken != Token::KeyRight) {
                    throwError({Token::KeyRight});
                }
                currentToken = lexer.getNextToken();
            } else {
                if (currentToken != Token::KeyLeft) {
                    throwError({Token::KeyLeft});
                }
                currentToken = lexer.getNextToken();

                statements(); 

                if (currentToken != Token::KeyRight) {
                    throwError({Token::KeyRight});
                }
                currentToken = lexer.getNextToken();

                break;
            }
        }     
       
    }else {
        throwError({Token::KwIf});
    }
}


void SimplParser::loop() {
    if (currentToken == Token::KwWhile) {
        currentToken = lexer.getNextToken();
        if (currentToken != Token::ParenthesisLeft) {
            throwError({Token::ParenthesisLeft});
        }
        currentToken = lexer.getNextToken();
        expression();
        if (currentToken != Token::ParenthesisRight) {
            throwError({Token::ParenthesisRight});
        }
        currentToken = lexer.getNextToken();

        if (currentToken != Token::KeyLeft) {
            throwError({Token::KeyLeft});
        }
        currentToken = lexer.getNextToken();

        statements();
        if (currentToken != Token::KeyRight) {
            throwError({Token::KeyRight});
        }
        currentToken = lexer.getNextToken();
    } else if(currentToken == Token::KwFor){
        currentToken = lexer.getNextToken();

        if(currentToken != Token::Ident) {
            throwError({Token::Ident});
        }
        currentToken = lexer.getNextToken();

        if(currentToken != Token::Assignment) {
            throwError({Token::Assignment});
        }
        currentToken = lexer.getNextToken();

        expression();

        if(currentToken != Token::KwTo) {
            throwError({Token::KwTo});
        }

        currentToken = lexer.getNextToken();

        expression();

        if(currentToken == Token::KwStep) {
            currentToken = lexer.getNextToken();
            expression();
        }

        if(currentToken != Token::KeyLeft) {
            throwError({Token::KeyLeft});
        }
        
        currentToken = lexer.getNextToken();
        statements();

        if(currentToken != Token::KeyRight) {
            throwError({Token::KeyRight});
        }

        currentToken = lexer.getNextToken(); 

    }else {
        throwError({Token::KwWhile, Token::KwFor});
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
    else if(currentToken == Token::KwReadInt || currentToken == Token::KwReadBool){
        currentToken=lexer.getNextToken();
        if(currentToken != Token::ParenthesisLeft){
            throwError({Token::ParenthesisLeft});
        }

        currentToken = lexer.getNextToken();

        if(currentToken != Token::ParenthesisRight){
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

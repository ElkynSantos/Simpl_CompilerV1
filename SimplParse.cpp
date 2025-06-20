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

AstNode* SimplParser::program(){

    if(currentToken == Token::KwLet) {
        globalVarDeclare();

    }
    //else if(currentToken == Token::KwFn) {
    //     globalFnDeclare();
    // }

}

AstNode* SimplParser :: globalVarDeclare(){
    
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

// AstNode* SimplParser::globalFnDeclare() {

//     if (currentToken == Token::KwFn) {
//         currentToken = lexer.getNextToken();

//         if (currentToken != Token::Ident) {
//             throwError({Token::Ident});
//         }

//         currentToken = lexer.getNextToken();

//         if (currentToken != Token::ParenthesisLeft) {
//             throwError({Token::ParenthesisLeft});
//         }
//         currentToken = lexer.getNextToken();
//         paramList();
//         if(currentToken != Token::ParenthesisRight) {
//             throwError({Token::ParenthesisRight});
//         }
//         currentToken = lexer.getNextToken();

//         if (currentToken != Token::AssignmentFunction) {
//             throwError({Token::AssignmentFunction});
//         }

//         currentToken = lexer.getNextToken();

//         functionTypes();

//         if(currentToken != Token::KeyLeft) {
//             throwError({Token::KeyLeft});
//         }

//         currentToken = lexer.getNextToken();
        
//         statements();
//         if( currentToken != Token::KeyRight) {
//             throwError({Token::KeyRight});
//         }

//         currentToken = lexer.getNextToken();

//     } else {
//         throwError({Token::KwFn});
//     }
// }

// AstNode* SimplParser::paramList() {

//     if (currentToken == Token::Ident) {
//         param();   
//         while (currentToken == Token::Comma) {
//             currentToken = lexer.getNextToken();
//             param();
//         }
//     }

// }

// AstNode* SimplParser :: param(){

//     if(currentToken == Token::Ident) {
//         currentToken = lexer.getNextToken();
//         if(currentToken != Token::Colon) {
//             throwError({Token::Colon});
//         }

//         currentToken = lexer.getNextToken();

//         if(currentToken == Token::KwRef) {
//             currentToken = lexer.getNextToken();
//         }
       
//         type();

//         if(currentToken == Token::BracketLeft) {
//             currentToken = lexer.getNextToken();
//             sizeExpression();
//             if(currentToken != Token::BracketRight) {
//                 throwError({Token::BracketRight});
//             }
//             currentToken = lexer.getNextToken();
//         }
           
//     }else {
//         throwError({Token::Ident});
//     }

// }

// AstNode* SimplParser::functionTypes(){
//     if(currentToken == Token::KwInt || currentToken == Token::KwBool || currentToken == Token::KwVoid){
//         currentToken = lexer.getNextToken();
//     }else{
//         throwError({Token::KwInt, Token::KwBool, Token::KwVoid});
//     }
// }

// AstNode* SimplParser::statements(){
    
//     while(currentToken == Token::KwLet ||  currentToken == Token::Ident || currentToken == Token::KwIf || currentToken == Token::KwWhile || 
//            currentToken == Token::KwFor || currentToken == Token::KwReturn || currentToken == Token::KwPrint){
//         statement();
//     }

// }

// AstNode* SimplParser::statement(){

//     if(currentToken == Token::KwLet) {
//         globalVarDeclare();
//     } 
//     else if (currentToken == Token::Ident) {
//         // logic call function here
//         assignmentValues();
//     } 
//     else if (currentToken == Token::KwIf) {
//         conditionalStatement();
//     } 
//     else if (currentToken == Token::KwWhile || currentToken == Token::KwFor) {
//         loop();
//     }
//     else if(currentToken == Token::KwPrint){
//         print();
//     } 
//     else if (currentToken == Token::KwReturn) {
//        returnStatement();
//     }
    
// }

// AstNode* SimplParser :: print(){
//     if(currentToken == Token::KwPrint){
//         currentToken = lexer.getNextToken();
//         if(currentToken != Token::ParenthesisLeft){
//             throwError({Token :: ParenthesisLeft});
//         }
//         currentToken = lexer.getNextToken();
//         if(currentToken == Token::StringLiteral || currentToken == Token::Ident){
//             currentToken = lexer.getNextToken();

//             if(currentToken == Token::BracketLeft){
//                 currentToken = lexer.getNextToken();
//                 sizeExpression();
//                 if(currentToken != Token::BracketRight){
//                     throwError({Token :: BracketRight});
//                 }
//                 currentToken = lexer.getNextToken();

//             }
//         }else{
//             throwError({Token :: StringLiteral, Token::Ident});
//         }

//         if(currentToken != Token::ParenthesisRight){
//             throwError({Token :: ParenthesisRight});
        
//         }
//         currentToken = lexer.getNextToken();
//         if(currentToken != Token::Semicolon){
//             throwError({Token :: Semicolon});

//         }
//         currentToken = lexer.getNextToken();
//     }else{
//         throwError({Token :: KwPrint});

//     }
// }

// AstNode* SimplParser::returnStatement() {
//     if (currentToken == Token::KwReturn) {
//         currentToken = lexer.getNextToken();
//         if (currentToken != Token::Semicolon) {
//             expression();
//             if (currentToken != Token::Semicolon) {
//                 throwError({Token::Semicolon});
//             }
//             currentToken = lexer.getNextToken();
            
//         }else{
//              currentToken = lexer.getNextToken();
//         }
       
//     } else {
//         throwError({Token::KwReturn});
//     }
// }

// AstNode* SimplParser::assignmentValues(){
//     if(currentToken == Token::Ident) {
//         currentToken = lexer.getNextToken();
//         //functionCall
//         if(currentToken == Token::ParenthesisLeft) {
//             currentToken = lexer.getNextToken();

//             argumentList();
//             if(currentToken != Token::ParenthesisRight) {
//                 throwError({Token::ParenthesisRight});
//             }
//             currentToken = lexer.getNextToken();

//             if(currentToken != Token::Semicolon) {
//                 throwError({Token::Semicolon});
//             }
//             currentToken = lexer.getNextToken();

//             return;
//         }
//         else if(currentToken == Token::BracketLeft) {
//             currentToken = lexer.getNextToken();
//             sizeExpression();
//             if(currentToken != Token::BracketRight) {
//                 throwError({Token::BracketRight});
//             }
//             currentToken = lexer.getNextToken();
//         }

//         if(currentToken == Token::Assignment) {
//             currentToken = lexer.getNextToken();
//             expression();
//             if(currentToken != Token::Semicolon) {
//                 throwError({Token::Semicolon});
//             }
//             currentToken = lexer.getNextToken();
//         }
//     }else{
//         throwError({Token::Ident});
//     }
// }

AstNode* SimplParser::argumentList() {
    expression();
    while (currentToken == Token::Comma) {
        currentToken = lexer.getNextToken();
        expression();
    }
}
// AstNode* SimplParser::conditionalStatement() {
//     if(currentToken == Token::KwIf) {
//         currentToken = lexer.getNextToken();
//         if(currentToken != Token::ParenthesisLeft) {
//             throwError({Token::ParenthesisLeft});
//         }
//         currentToken = lexer.getNextToken();
//         expression();
//         if(currentToken != Token::ParenthesisRight) {
//             throwError({Token::ParenthesisRight});
//         }
//         currentToken = lexer.getNextToken();
        
//         if(currentToken != Token::KeyLeft) {
//             throwError({Token::KeyLeft});
//         }
//         currentToken = lexer.getNextToken();

//         statements();

//         std::cout << "Current token after statements: " << lexer.tokenToString(currentToken) << std::endl;
//         if(currentToken != Token::KeyRight) {
//             throwError({Token::KeyRight});
//         }

//         currentToken = lexer.getNextToken();
        
//        while (currentToken == Token::KwElse) {
//             currentToken = lexer.getNextToken();

//             if (currentToken == Token::KwIf) {
//                 currentToken = lexer.getNextToken();

//                 if (currentToken != Token::ParenthesisLeft) {
//                     throwError({Token::ParenthesisLeft});
//                 }
//                 currentToken = lexer.getNextToken();

//                 expression(); 
//                 if (currentToken != Token::ParenthesisRight) {
//                     throwError({Token::ParenthesisRight});
//                 }
//                 currentToken = lexer.getNextToken();

//                 if (currentToken != Token::KeyLeft) {
//                     throwError({Token::KeyLeft});
//                 }
//                 currentToken = lexer.getNextToken();

//                 statements();

//                 if (currentToken != Token::KeyRight) {
//                     throwError({Token::KeyRight});
//                 }
//                 currentToken = lexer.getNextToken();
//             } else {
//                 if (currentToken != Token::KeyLeft) {
//                     throwError({Token::KeyLeft});
//                 }
//                 currentToken = lexer.getNextToken();

//                 statements(); 

//                 if (currentToken != Token::KeyRight) {
//                     throwError({Token::KeyRight});
//                 }
//                 currentToken = lexer.getNextToken();

//                 break;
//             }
//         }     
       
//     }else {
//         throwError({Token::KwIf});
//     }
// }


// AstNode* SimplParser::loop() {
//     if (currentToken == Token::KwWhile) {
//         currentToken = lexer.getNextToken();
//         if (currentToken != Token::ParenthesisLeft) {
//             throwError({Token::ParenthesisLeft});
//         }
//         currentToken = lexer.getNextToken();
//         expression();
//         if (currentToken != Token::ParenthesisRight) {
//             throwError({Token::ParenthesisRight});
//         }
//         currentToken = lexer.getNextToken();

//         if (currentToken != Token::KeyLeft) {
//             throwError({Token::KeyLeft});
//         }
//         currentToken = lexer.getNextToken();

//         statements();
//         if (currentToken != Token::KeyRight) {
//             throwError({Token::KeyRight});
//         }
//         currentToken = lexer.getNextToken();
//     } else if(currentToken == Token::KwFor){
//         currentToken = lexer.getNextToken();

//         if(currentToken != Token::Ident) {
//             throwError({Token::Ident});
//         }
//         currentToken = lexer.getNextToken();

//         if(currentToken != Token::Assignment) {
//             throwError({Token::Assignment});
//         }
//         currentToken = lexer.getNextToken();

//         expression();

//         if(currentToken != Token::KwTo) {
//             throwError({Token::KwTo});
//         }

//         currentToken = lexer.getNextToken();

//         expression();

//         if(currentToken == Token::KwStep) {
//             currentToken = lexer.getNextToken();
//             expression();
//         }

//         if(currentToken != Token::KeyLeft) {
//             throwError({Token::KeyLeft});
//         }
        
//         currentToken = lexer.getNextToken();
//         statements();

//         if(currentToken != Token::KeyRight) {
//             throwError({Token::KeyRight});
//         }

//         currentToken = lexer.getNextToken(); 

//     }else {
//         throwError({Token::KwWhile, Token::KwFor});
//     }
// }

AstNode* SimplParser::arrayinitializer() {
    expression();
    while (currentToken == Token::Comma) {
        currentToken = lexer.getNextToken();
        expression();
    }
}
AstNode* SimplParser::type(){
    if(currentToken == Token::KwInt || currentToken == Token::KwBool){
        currentToken = lexer.getNextToken();
    }else{
         throwError({Token::KwInt, Token::KwBool});
    }
}

AstNode* SimplParser::sizeExpression(){
    expression();
}

AstNode* SimplParser::expression() {
   booleanExpression();
}

AstNode* SimplParser :: booleanExpression(){
    booleanTerm();
    while (currentToken == Token::KwOr) {
        currentToken = lexer.getNextToken();
        booleanTerm();
    }
}

AstNode* SimplParser::booleanTerm() {
    booleanFactor();
    while (currentToken == Token::kwAnd) {
        currentToken = lexer.getNextToken();
        booleanFactor();
    }
    
}

AstNode* SimplParser::booleanFactor() {
    if(currentToken == Token::KwNot){
        currentToken = lexer.getNextToken();
        booleanFactor();
    } 

    relationalExpression();
}

AstNode* SimplParser::relationalExpression() {
   arithmeticExpression();
   if(currentToken == Token::Equal || currentToken == Token::NotEqual ||
      currentToken == Token::LessThan || currentToken == Token::LessThanOrEqualTo ||
      currentToken == Token::GreaterThan || currentToken == Token::GreaterThanOrEqualTo) {
        currentToken = lexer.getNextToken();
        arithmeticExpression();
    }
}

AstNode* SimplParser::arithmeticExpression() {

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

AstNode* SimplParser::term(){

    factor();

    while(currentToken == Token::Multiplication || currentToken == Token::Module || currentToken == Token::Division){
        currentToken = lexer.getNextToken();
        factor();
    }
    
}

AstNode* SimplParser::factor(){

    if(currentToken == Token::Addition || currentToken == Token::Subtraction){
        currentToken = lexer.getNextToken();
        primary();
          
    }

    primary();

}

AstNode* SimplParser::primary(){

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
            
            argumentList();
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

#include <iostream>
#include "SimplParse.hpp"
#include "SimplLexer.hpp"
#include <stdexcept>
#include "tokens.hpp"

programNode* SimplParser::parse() {

    currentToken = lexer.getNextToken();


    programNode *simplProgram = program();

    if (currentToken != Token::Eof)
    {

        throwError({Token::Eof});
    }
    std::cout<< simplProgram->toString()<<std::endl ;
    return simplProgram;
}

programNode* SimplParser::program() {
    std::vector<GlobalDeclaration*> globalDeclarations;

    while(currentToken == Token::KwLet || currentToken == Token::KwFn) {
        if(currentToken == Token::KwLet) {
            GlobalVarDeclNode* varDecl = globalVarDeclare();
            if(varDecl) {
                globalDeclarations.push_back(varDecl);
            }
        } else if(currentToken == Token::KwFn) {
            GlobalFnDeclareNode* funcDecl = globalFnDeclare();
            if(funcDecl) {
                globalDeclarations.push_back(funcDecl);
            }
        }
    }

    if(globalDeclarations.empty()) {
        throwError({Token::KwLet, Token::KwFn});
        return nullptr;
    }

    return new programNode(globalDeclarations);
}

GlobalVarDeclNode* SimplParser :: globalVarDeclare(){
    std::string identifier;
    TypeNameNode* typeNode = nullptr;
    sizeExpressionNode* sizeExpressionArray = nullptr;
    AstNode* initializer = nullptr;
    bool isArray = false;
    if( currentToken == Token::KwLet){
       
        currentToken = lexer.getNextToken();

        if( currentToken != Token::Ident){
            throwError({Token::Ident});
        }
        identifier = lexer.text;
        currentToken = lexer.getNextToken();

        if(currentToken != Token::Colon){
            throwError({Token::Colon});    
        }

        currentToken = lexer.getNextToken();
        typeNode = type();
        if(currentToken == Token::Semicolon){ 
            currentToken = lexer.getNextToken();
            return new GlobalVarDeclNode(identifier, typeNode, sizeExpressionArray, initializer, false);
        }
        else if(currentToken == Token::Assignment){
            currentToken = lexer.getNextToken();
            initializer = expression();
            if(currentToken != Token::Semicolon) {
                throwError({Token::Semicolon});
            
            }
            currentToken = lexer.getNextToken();
            return new GlobalVarDeclNode(identifier, typeNode, sizeExpressionArray, initializer, false);
        }
        else if (currentToken == Token::BracketLeft){
            currentToken = lexer.getNextToken();
            sizeExpressionArray = sizeExpression();

            if(currentToken != Token::BracketRight) throwError({Token::BracketRight});
            
            currentToken = lexer.getNextToken();

            if(currentToken == Token::Semicolon){ 
        
                currentToken = lexer.getNextToken();
                return new GlobalVarDeclNode(identifier, typeNode, sizeExpressionArray, initializer, true);
            }   

            if(currentToken != Token::Assignment) throwError({Token::Assignment});

            currentToken = lexer.getNextToken();

            if(currentToken != Token :: BracketLeft) throwError({Token::BracketLeft});
            
            currentToken = lexer.getNextToken();
            initializer = new Initializer(arrayinitializer());

            if(currentToken != Token :: BracketRight) throwError({Token::BracketRight});
            currentToken = lexer.getNextToken();
            
            if(currentToken != Token :: Semicolon) throwError({Token::Semicolon});
            currentToken = lexer.getNextToken();

            return new GlobalVarDeclNode(identifier, typeNode, sizeExpressionArray, initializer, true);
        }else{
            throwError({Token::Assignment, Token::BracketLeft});
        }
        return nullptr;
    }else{
        throwError({Token::KwLet});
    }
    return nullptr;
}

GlobalFnDeclareNode* SimplParser::globalFnDeclare() {
    if (currentToken == Token::KwFn) {
        currentToken = lexer.getNextToken();

        // Capturar nombre de función
        if (currentToken != Token::Ident) {
            throwError({Token::Ident});
        }
        std::string functionName = lexer.text;
        currentToken = lexer.getNextToken();

        // Capturar parámetros
        if (currentToken != Token::ParenthesisLeft) {
            throwError({Token::ParenthesisLeft});
        }
        currentToken = lexer.getNextToken();
        ParameterList* params = paramList();
        if(currentToken != Token::ParenthesisRight) {
            throwError({Token::ParenthesisRight});
        }
        currentToken = lexer.getNextToken();

        // Capturar tipo de retorno
        if (currentToken != Token::AssignmentFunction) {
            throwError({Token::AssignmentFunction});
        }
        currentToken = lexer.getNextToken();
        TypeNameFunctionNode* returnType = functionTypes();

        // Capturar cuerpo de función
        if(currentToken != Token::KeyLeft) {
            throwError({Token::KeyLeft});
        }
        currentToken = lexer.getNextToken();
        
        std::vector<statementsNode*> functionStatementsRaw = statements();
        std::vector<AstNode*> functionStatements;
        for (auto stmt : functionStatementsRaw) {
            functionStatements.push_back(static_cast<AstNode*>(stmt));
        }
        statementsNode* body = new statementsNode(functionStatements);

        if(currentToken != Token::KeyRight) {
            throwError({Token::KeyRight});
        }
        currentToken = lexer.getNextToken();

        return new GlobalFnDeclareNode(functionName, params, returnType, body);
    } else {
        throwError({Token::KwFn});
    }
    return nullptr;
}

ParameterList* SimplParser::paramList() {
    std::vector<Parameter*> parameters;
    if (currentToken == Token::Ident) {
        parameters.push_back(param());   
        while (currentToken == Token::Comma) {
            currentToken = lexer.getNextToken();
            parameters.push_back(param());           
        }
    }
    return new ParameterList(parameters);
}

Parameter* SimplParser::param() {
    if(currentToken == Token::Ident) {
        std::string paramName = lexer.text;
        currentToken = lexer.getNextToken();

        if(currentToken != Token::Colon) {
            throwError({Token::Colon});
        }
        currentToken = lexer.getNextToken();

        // Check if parameter is by reference
        std::cout << "Current token: " << lexer.tokenToString(currentToken) << std::endl;
        bool isByReference = false;
        if(currentToken == Token::KwRef) {
            isByReference = true;
            currentToken = lexer.getNextToken();
        }
       
        // Get parameter type
        TypeNameNode* paramType = type();

        // Handle array type
        sizeExpressionNode* arraySize = nullptr;
        if(currentToken == Token::BracketLeft) {
            currentToken = lexer.getNextToken();
            arraySize = sizeExpression();
            if(currentToken != Token::BracketRight) {
                throwError({Token::BracketRight});
            }
            currentToken = lexer.getNextToken();
        }
           
        return new Parameter(paramName, paramType, arraySize, isByReference);
    } else {
        throwError({Token::Ident});
    }
    return nullptr;
}

TypeNameFunctionNode* SimplParser::functionTypes() {
    EnumFunctionType type;
    
    if(currentToken == Token::KwInt) {
        type = EnumFunctionType::Int;
        currentToken = lexer.getNextToken();
    }
    else if(currentToken == Token::KwBool) {
        type = EnumFunctionType::Bool;
        currentToken = lexer.getNextToken();
    }
    else if(currentToken == Token::KwVoid) {
        type = EnumFunctionType::Void;
        currentToken = lexer.getNextToken();
    }
    else {
        throwError({Token::KwInt, Token::KwBool, Token::KwVoid});
        return nullptr;
    }
    
    return new TypeNameFunctionNode(type);
}  


std::vector<statementsNode*> SimplParser::statements(){
    std::vector<statementsNode*> stmts;
    
    while(currentToken == Token::KwLet ||  currentToken == Token::Ident || currentToken == Token::KwIf || currentToken == Token::KwWhile || 
           currentToken == Token::KwFor || currentToken == Token::KwReturn || currentToken == Token::KwPrint){
        AstNode* stmt = statement();
        if (stmt != nullptr) {
            stmts.push_back(static_cast<statementsNode*>(stmt));
        }
    }
    return stmts;

}

AstNode* SimplParser::statement(){

    if(currentToken == Token::KwLet) {
       return globalVarDeclare();
    } 
    else if (currentToken == Token::Ident) {
       return  assignmentValues();
    } 
    else if (currentToken == Token::KwIf) {
       return conditionalStatement();
    } 
    else if (currentToken == Token::KwWhile || currentToken == Token::KwFor) {
       return loop();
    }
    else if(currentToken == Token::KwPrint){
       return print();
    } 
    else if (currentToken == Token::KwReturn) {
       return returnStatement();
    }
    
    return nullptr;
}

PrintStatement* SimplParser :: print(){
    std::string identifier;
    sizeExpressionNode* express = nullptr;
    if(currentToken == Token::KwPrint){
        currentToken = lexer.getNextToken();
        if(currentToken != Token::ParenthesisLeft){
            throwError({Token :: ParenthesisLeft});
        }
        currentToken = lexer.getNextToken();
        if(currentToken == Token::StringLiteral || currentToken == Token::Ident || currentToken == Token::Number) {
            identifier = lexer.text;
            currentToken = lexer.getNextToken();

            if(currentToken == Token::BracketLeft){
                currentToken = lexer.getNextToken();
                express = sizeExpression();
                if(currentToken != Token::BracketRight){
                    throwError({Token :: BracketRight});
                }
                currentToken = lexer.getNextToken();

            }
        }else{
            throwError({Token :: StringLiteral, Token::Ident});
        }

        if(currentToken != Token::ParenthesisRight){
            throwError({Token :: ParenthesisRight});
        
        }
        currentToken = lexer.getNextToken();
        if(currentToken != Token::Semicolon){
            throwError({Token :: Semicolon});

        }
        currentToken = lexer.getNextToken();
        
        return new PrintStatement(identifier, express);
    }else{
        throwError({Token :: KwPrint});

    }

    return nullptr;
}

ReturnStatement* SimplParser::returnStatement() {
    AstNode* expr = nullptr;
    if (currentToken == Token::KwReturn) {
        currentToken = lexer.getNextToken();
        if (currentToken != Token::Semicolon) {
            expr = expression();
            if (currentToken != Token::Semicolon) {
                throwError({Token::Semicolon});
            }
            currentToken = lexer.getNextToken();
            return new ReturnStatement(expr);
        }else{
            currentToken = lexer.getNextToken();
        }
       
    } else {
        throwError({Token::KwReturn});
    }
    return nullptr;
}

AstNode* SimplParser::assignmentValues(){
    std::string identifier;
    sizeExpressionNode* index = nullptr;
    bool isArray = false;
    if(currentToken == Token::Ident) {
        identifier = lexer.text;
        currentToken = lexer.getNextToken();
        //functionCall
        if(currentToken == Token::ParenthesisLeft) {
            currentToken = lexer.getNextToken();

            std::vector<ArgumentNode*> args = argumentList();
            if(currentToken != Token::ParenthesisRight) {
                throwError({Token::ParenthesisRight});
            }
            currentToken = lexer.getNextToken();

            if(currentToken != Token::Semicolon) {
                throwError({Token::Semicolon});
            }
            currentToken = lexer.getNextToken();

            return new FunctionCall(identifier, args);
        }
        else if(currentToken == Token::BracketLeft) {
            isArray = true;
            currentToken = lexer.getNextToken();
            index = sizeExpression();
            if(currentToken != Token::BracketRight) {
                throwError({Token::BracketRight});
            }
            currentToken = lexer.getNextToken();
        }

        if(currentToken == Token::Assignment) {
            currentToken = lexer.getNextToken();
            AstNode* express = expression();
            if(currentToken != Token::Semicolon) {
                throwError({Token::Semicolon});
            }
            currentToken = lexer.getNextToken();
            return new AssignStament(identifier, index, new Initializer(express), isArray);
 
        }

    return nullptr;
    }else{
        throwError({Token::Ident});
    }
    return nullptr;
}

std::vector<ArgumentNode*> SimplParser::argumentList() {
    std::vector<ArgumentNode*> arguments;
    bool isRef = false;

    if(currentToken == Token::KwRef){
        isRef = true;
       currentToken = lexer.getNextToken();
    }

    AstNode* expr = expression();
    arguments.push_back(new ArgumentNode(expr, isRef));

    while (currentToken == Token::Comma) {
        currentToken = lexer.getNextToken();
        isRef = false;
        if(currentToken == Token::KwRef){
            isRef = true;
            currentToken = lexer.getNextToken();
        }
        expr = expression();
        arguments.push_back(new ArgumentNode(expr, isRef));
    }

    return arguments;
}


ConditionalStatement* SimplParser::conditionalStatement() {
    if(currentToken == Token::KwIf) {
        currentToken = lexer.getNextToken();
        if(currentToken != Token::ParenthesisLeft) {
            throwError({Token::ParenthesisLeft});
        }
        currentToken = lexer.getNextToken();
        AstNode* ifCondition = expression();
        if(currentToken != Token::ParenthesisRight) {
            throwError({Token::ParenthesisRight});
        }
        currentToken = lexer.getNextToken();
        
        if(currentToken != Token::KeyLeft) {
            throwError({Token::KeyLeft});
        }
        currentToken = lexer.getNextToken();

        std::vector<statementsNode*> ifStatementsRaw = statements();
        std::vector<AstNode*> ifStatements;
        for (auto stmt : ifStatementsRaw) {
            ifStatements.push_back(static_cast<AstNode*>(stmt));
        }
        AstNode* ifBody = new statementsNode(ifStatements);
        IfStatement* mainIf = new IfStatement(ifCondition, ifBody);

        if(currentToken != Token::KeyRight) {
            throwError({Token::KeyRight});
        }

        currentToken = lexer.getNextToken();
        std::vector<ElseIfStatement*> elseIfs;
        ElseStatement* elseStmt = nullptr;


        while (currentToken == Token::KwElse) {

            currentToken = lexer.getNextToken();

            if (currentToken == Token::KwIf) {
                currentToken = lexer.getNextToken();

                if (currentToken != Token::ParenthesisLeft) {
                    throwError({Token::ParenthesisLeft});
                }
                currentToken = lexer.getNextToken();

                AstNode* elseIfCondition = expression();
                if (currentToken != Token::ParenthesisRight) {
                    throwError({Token::ParenthesisRight});
                }
                currentToken = lexer.getNextToken();

                if (currentToken != Token::KeyLeft) {
                    throwError({Token::KeyLeft});
                }
                currentToken = lexer.getNextToken();

                std::vector<statementsNode*> elseIfStatementsRaw = statements();
                std::vector<AstNode*> elseIfStatements;
                for (auto stmt : elseIfStatementsRaw) {
                    elseIfStatements.push_back(static_cast<AstNode*>(stmt));
                }
                AstNode* elseIfBody = new statementsNode(elseIfStatements);

                if (currentToken != Token::KeyRight) {
                    throwError({Token::KeyRight});
                }
                currentToken = lexer.getNextToken();
                elseIfs.push_back(new ElseIfStatement(elseIfCondition, elseIfBody));

            } else {
                if (currentToken != Token::KeyLeft) {
                    throwError({Token::KeyLeft});
                }
                currentToken = lexer.getNextToken();

                std::vector<statementsNode*> elseStatementsRaw = statements();
                std::vector<AstNode*> elseStatements;
                for (auto stmt : elseStatementsRaw) {
                    elseStatements.push_back(static_cast<AstNode*>(stmt));
                }
                AstNode* elseBody = new statementsNode(elseStatements);


                if (currentToken != Token::KeyRight) {
                    throwError({Token::KeyRight});
                }
                currentToken = lexer.getNextToken();
                elseStmt = new ElseStatement(elseBody);
                return new ConditionalStatement(mainIf, elseIfs, elseStmt);
                break;
            }


        }
        std::cout<< "token actual " << lexer.tokenToString(currentToken) << std::endl;
        return new ConditionalStatement(mainIf, elseIfs, elseStmt);
    }else {
        throwError({Token::KwIf});
    }
    return nullptr;
}


AstNode* SimplParser::loop() {
    if (currentToken == Token::KwWhile) {
        currentToken = lexer.getNextToken();
        if (currentToken != Token::ParenthesisLeft) {
            throwError({Token::ParenthesisLeft});
        }
        currentToken = lexer.getNextToken();
        AstNode* express = expression();
        if (currentToken != Token::ParenthesisRight) {
            throwError({Token::ParenthesisRight});
        }
        currentToken = lexer.getNextToken();

        if (currentToken != Token::KeyLeft) {
            throwError({Token::KeyLeft});
        }
        currentToken = lexer.getNextToken();

        std::vector<statementsNode*> statementsBlockRaw = statements();
        std::vector<AstNode*> statementsBlock;

        for (statementsNode* statement : statementsBlockRaw) {
            statementsBlock.push_back(static_cast<AstNode*>(statement));
        }
        statementsNode* body = new statementsNode(statementsBlock);

        if (currentToken != Token::KeyRight) {
            throwError({Token::KeyRight});
        }

        currentToken = lexer.getNextToken();
        return new WhileStatement(express, body);
    } else if(currentToken == Token::KwFor){
        currentToken = lexer.getNextToken();
        std::string identifier = lexer.text;
        if(currentToken != Token::Ident) {
            throwError({Token::Ident});
        }
        currentToken = lexer.getNextToken();

        if(currentToken != Token::Assignment) {
            throwError({Token::Assignment});
        }
        currentToken = lexer.getNextToken();

        AstNode* startExpress = expression();

        if(currentToken != Token::KwTo) {
            throwError({Token::KwTo});
        }

        currentToken = lexer.getNextToken();

        AstNode* EndExpress = expression();
        AstNode* stepExpress = nullptr;
        if(currentToken == Token::KwStep) {
            currentToken = lexer.getNextToken();
            stepExpress = expression();
        }

        if(currentToken != Token::KeyLeft) {
            throwError({Token::KeyLeft});
        }
        
        currentToken = lexer.getNextToken();
        std::vector<statementsNode*> statementsBlockRaw = statements();
        std::vector<AstNode*> statementsBlock;

        for (statementsNode* statement : statementsBlockRaw) {
            statementsBlock.push_back(static_cast<AstNode*>(statement));
        }
        statementsNode* body = new statementsNode(statementsBlock);
        if(currentToken != Token::KeyRight) {
            throwError({Token::KeyRight});
        }

        currentToken = lexer.getNextToken(); 
 
        return new ForStament(identifier, startExpress, EndExpress, stepExpress, body);
    }else {
        throwError({Token::KwWhile, Token::KwFor});
    }
    return nullptr;
}

std::vector<AstNode*> SimplParser::arrayinitializer() {
    std::vector<AstNode*> valuesInArray;

    valuesInArray.push_back(expression());
    while (currentToken == Token::Comma) {
        currentToken = lexer.getNextToken();
        valuesInArray.push_back(expression());
    }
    return valuesInArray;

}
TypeNameNode* SimplParser::type(){
    if(currentToken == Token::KwInt || currentToken == Token::KwBool){
        
        currentToken = lexer.getNextToken();
        return new TypeNameNode(currentToken  == Token::KwInt ? EnumVarType::Int : EnumVarType::Bool);

    }else{
        throwError({Token::KwInt, Token::KwBool});
    }
    return nullptr;

}

sizeExpressionNode* SimplParser::sizeExpression(){
    AstNode* expr = expression();
    return new sizeExpressionNode(expr);

}

AstNode* SimplParser::expression() {
  return booleanExpression();
}

AstNode* SimplParser :: booleanExpression(){
     
    AstNode* left = booleanTerm();
    while (currentToken == Token::KwOr) {
        currentToken = lexer.getNextToken();
        AstNode *right = booleanTerm();
        left = new OrBoolean(left, right);
    }
    return left;
}

AstNode* SimplParser::booleanTerm() {
    AstNode* left = booleanFactor();
    while (currentToken == Token::kwAnd) {
        currentToken = lexer.getNextToken();
        AstNode* right = booleanFactor();
        left = new AndBoolean(left, right);
    }
    return left;
   
}

AstNode* SimplParser::booleanFactor() {
    if(currentToken == Token::KwNot){
        currentToken = lexer.getNextToken();
       return new UnaryNotBoolean(booleanFactor());
    } 

    return relationalExpression();
}

AstNode* SimplParser::relationalExpression() {
   AstNode *left = arithmeticExpression();
   if(currentToken == Token::Equal || currentToken == Token::NotEqual ||
      currentToken == Token::LessThan || currentToken == Token::LessThanOrEqualTo ||
      currentToken == Token::GreaterThan || currentToken == Token::GreaterThanOrEqualTo) {
        Token currentOp = currentToken;
        currentToken = lexer.getNextToken();
        AstNode *right = arithmeticExpression();

        switch (currentOp) {
            case Token::Equal:
                return new EqualBoolean(left, right);
            case Token::NotEqual:
                return new NEqualBoolean(left, right);
            case Token::LessThan:
                return new Less_ThanBoolean(left, right);
            case Token::LessThanOrEqualTo:
                return new LessEqualBoolean(left, right);
            case Token::GreaterThan:
                return new Greater_ThanBoolean(left, right);
            case Token::GreaterThanOrEqualTo:
                return new GreaterEqualBoolean(left, right);
            default:
                throwError({Token::Equal, Token::NotEqual, Token::LessThan, Token::LessThanOrEqualTo, Token::GreaterThan, Token::GreaterThanOrEqualTo});
        }
    }
    return left;
}

AstNode* SimplParser::arithmeticExpression() {

    AstNode* left = term();
    while (currentToken == Token::Addition || currentToken == Token::Subtraction) {
        Token currentOp = currentToken;
        currentToken = lexer.getNextToken();
        AstNode *right = term();
        
         if (currentOp == Token::Addition)
        {
            left = new SumExpr(left, right);
        }
        else if (currentOp == Token::Subtraction)
        {
            left = new SubExpr(left, right);
        }

    }
    return left;    
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
    AstNode *left = factor();
    while(currentToken == Token::Multiplication || currentToken == Token::Module || currentToken == Token::Division){
        Token currentOp = currentToken;
        currentToken = lexer.getNextToken();
        AstNode *right = factor();

        switch (currentOp) {
            case Token::Multiplication:
                left = new MulExpr(left, right);
                break;
            case Token::Division:
                left = new DivExpr(left, right);
                break;
            case Token::Module:
                left = new ModExpr(left, right);
                break;
        }
        return left;

    }
    return left;   
}

AstNode* SimplParser::factor(){

    if(currentToken == Token::Addition || currentToken == Token::Subtraction){

        Token currentOp = currentToken;
        currentToken = lexer.getNextToken();
        AstNode *expr = primary();
          
    }

    return primary();

}

AstNode* SimplParser::primary(){

    if(currentToken == Token::Number || currentToken == Token::KwTrue || currentToken == Token::KwFalse){
        Token currentConstant = currentToken;

        switch (currentConstant)
        {
        case Token::Number:
            {
                int value = std::stoi(lexer.text);
                currentToken = lexer.getNextToken();

                return new IntConst(value);
            }
            break;
        case Token::KwTrue:
        case Token::KwFalse:
            {
                bool value = (currentConstant == Token::KwTrue);
                currentToken = lexer.getNextToken();
                return new BooleanConst(value);
            }
        }
    }
    else if(currentToken == Token::Ident){
        std::string identifier = lexer.text;
        currentToken = lexer.getNextToken();

        if(currentToken == Token::BracketLeft){
            currentToken = lexer.getNextToken();
            AstNode *indexExpr = expression();
            if(currentToken != Token::BracketRight) {
                throwError({Token::BracketRight});
            }
            currentToken = lexer.getNextToken();
            return new ArrayVariable(identifier, indexExpr);
        }

        if(currentToken == Token::ParenthesisLeft){
            currentToken = lexer.getNextToken();
            
            std::vector<ArgumentNode *> args = argumentList();
            if(currentToken != Token::ParenthesisRight) {
                throwError({Token::ParenthesisRight});
            }  
            currentToken = lexer.getNextToken();
            return new FunctionCall(identifier, args);
        }

        return new Variable(identifier);

    }
    else if(currentToken == Token::KwReadInt || currentToken == Token::KwReadBool){
        std::string functionName = lexer.text;
        currentToken=lexer.getNextToken();
        if(currentToken != Token::ParenthesisLeft){
            throwError({Token::ParenthesisLeft});
        }

        currentToken = lexer.getNextToken();

        if(currentToken != Token::ParenthesisRight){
            throwError({Token::ParenthesisRight});
        }
        currentToken = lexer.getNextToken();
        std::cout << "Read function called: " << lexer.tokenToString(currentToken) << std::endl;
        return new ReadFunctions(functionName);
    }
    else if(currentToken == Token::ParenthesisLeft){
        currentToken = lexer.getNextToken();
        AstNode *expr = expression();
        if(currentToken != Token::ParenthesisRight) {
            throwError({Token::ParenthesisRight});
        }
        currentToken = lexer.getNextToken();
        return expr;
    }
 return nullptr;
}

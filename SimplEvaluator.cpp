#include<stdexcept>
#include<iostream>
#include "SimplEvaluator.hpp"
#include "SimplAst.hpp"


int SimplEvaluator::evaluate(AstNode* node) {
    if (!node) {
        throw std::runtime_error("Null node encountered during evaluation.");
    }

    switch (node->kind()) {
        case NodeKind::Program:{
            const auto *ProgramNode = static_cast<programNode*>(node);
            for(auto &decl : ProgramNode->globalDeclarations) {
                if (decl->kind() == NodeKind::GlobalFnDeclare) {
                    const auto *fnDecl = static_cast<const GlobalFnDeclareNode*>(decl);
                    evaluate(decl);
                    methodTable[fnDecl->functionName] = fnDecl;
                }else if(decl->kind() == NodeKind::GlobalVarDeclare) {
                    evaluate(decl);
                }
            }
            auto it = methodTable.find("main");
            if (it == methodTable.end())
            {
                throw std::runtime_error("Please Define a principal (main) function");
            }

            evaluate(it->second->functionBody);

            return 1;
        }
        case NodeKind::GlobalFnDeclare:{
            const auto *fnDecl = static_cast<const GlobalFnDeclareNode*>(node);
            evaluate(fnDecl->parameters);

             if (fnDecl->returnType->type == EnumFunctionType::Int || fnDecl->returnType->type  == EnumFunctionType::Bool) {
                bool hasReturn = false;
                const auto* stmts = static_cast<const statementsNode*>(fnDecl->functionBody);
                
                for (const auto* stmt : stmts->statements) {
                    if (stmt->kind() == NodeKind::ReturnStatement) {
                        hasReturn = true;
                        break;
                    }
                }
                
                if (!hasReturn) {
                    throw std::runtime_error("Function '" + fnDecl->functionName + 
                                        "' must return a value of type int or bool");
                }
            }
            return 0;
        }
        case NodeKind::ParameterList:{
            const auto paramList = static_cast<const ParameterList*>(node);
            for (const auto &param : paramList->parameterList) {
                if (param->sizeExpression) {
                    int size = evaluate(param->sizeExpression->expression);
                    if (size <= 0) {
                        throw std::runtime_error("Invalid array size for parameter '" + param->identifier + "'. Size must be greater than 0.");
                    }
                }
                if (param->isRef) {
                
                }
            }
            return 0;
        }
        case NodeKind::GlobalVarDeclare:{
            const auto *varDecl = static_cast<const GlobalVarDeclNode*>(node);
            if(varDecl->isArray){
                std::string identifier = varDecl->identifier;
                int Sizevalue = evaluate(varDecl->sizeExpression);
                if(Sizevalue <= 0){
                    throw std::runtime_error("Invalid array size for variable '" + identifier + "'. Size must be greater than 0.");
                }
                if (isVariableDefined(identifier)) {
                    throw std::runtime_error("Variable '" + identifier + "' is already defined.");
                }
                arrays[identifier] = std::vector<std::optional<Value>>(Sizevalue, std::nullopt);
                if(varDecl -> initializer){
                    if(varDecl->type->type == EnumVarType::Int) {
                        if(varDecl->initializer->isArray) {
                            const auto &arrayValues = varDecl->initializer->arrayExpressions;
                            if (arrayValues.size() > Sizevalue) {
                                throw std::runtime_error("Initializer array size exceeds declared size for variable '" + identifier + "'.");
                            }
                            for (size_t i = 0; i < arrayValues.size(); ++i) {
                                int value = evaluate(arrayValues[i]);
                                arrays[identifier][i] = Value(value);
                            }
                        } else {
                             const auto &arrayValues = varDecl->initializer->arrayExpressions;
                            if (arrayValues.size() > Sizevalue) {
                                throw std::runtime_error("Initializer array size exceeds declared size for variable '" + identifier + "'.");
                            }
                            for (size_t i = 0; i < arrayValues.size(); ++i) {

                                int value = evaluate(arrayValues[i]);
                                if (value < 0) {
                                    throw std::runtime_error("Invalid value for array element '" + identifier + "[" + std::to_string(i) + "]'. Value must be non-negative.");
                                }
                                arrays[identifier][i] = Value(value);
                            }   
                        }
                       
                    } else if (varDecl->type->type == EnumVarType::Bool) {
                        if(varDecl->initializer->isArray) {
                            const auto &arrayValues = varDecl->initializer->arrayExpressions;
                            if (arrayValues.size() > Sizevalue) {
                                throw std::runtime_error("Initializer array size exceeds declared size for variable '" + identifier + "'.");
                            }
                            for (size_t i = 0; i < arrayValues.size(); ++i) {
                                int value = evaluate(arrayValues[i]);
                                if(value > 1 && value < 0){
                                    throw std::runtime_error("Invalid boolean value for variable '" + identifier + "[" + std::to_string(i) + "]'. Expected true or false.");
                                }
                                arrays[identifier][i] = Value(value == 1 ? true : false);;
                            }
                        } else {
                            int value = evaluate(varDecl->initializer->expression);
                            if(value > 1 && value < 0){
                                throw std::runtime_error("Invalid boolean value for variable '" + identifier + "'. Expected true or false.");
                            }
                            arrays[identifier][0] = Value(value == 1 ? true : false);
                        }

                    }

                }
            }else{
                std::string identifier = varDecl->identifier;
          
                if (isVariableDefined(identifier)) {
                    throw std::runtime_error("Variable '" + identifier + "' is already defined.");
                }
                
                if (varDecl->type->type == EnumVarType::Int) {
                    variables[identifier] = Value(0);
                } else if (varDecl->type->type == EnumVarType::Bool) {
                    variables[identifier] = Value(false);
                }
                if(varDecl->initializer){
                    
                    if(varDecl->type->type == EnumVarType::Int) {
                        int value = evaluate(varDecl->initializer->expression);
                        variables[identifier] = Value(value);
                    } else if (varDecl->type->type == EnumVarType::Bool) {
                       int value = evaluate(varDecl->initializer->expression);
                        if(value > 1 && value < 0){
                            throw std::runtime_error("Invalid boolean value for variable '" + identifier + "'. Expected true or false.");
                        }
                        variables[identifier] = Value(value == 1 ? true : false);
                    } 
                }
                
            }
            return 0;
        }
         case NodeKind::Initializer:{
            const auto *initializer = static_cast<const Initializer*>(node);
            if(initializer->isArray) {
                return 0;
            } else {
                return evaluate(initializer->expression);
            }
            
        }
        case NodeKind::EqualBoolean:
        {
            const auto *equalBooleanNode = static_cast<const EqualBoolean *>(node);
            return evaluate(equalBooleanNode->left) == evaluate(equalBooleanNode->right);
        }
        case NodeKind::NEqualBoolean:
        {
            const auto *nEqualBooleanNode = static_cast<const NEqualBoolean *>(node);
            return evaluate(nEqualBooleanNode->left) != evaluate(nEqualBooleanNode->right);
        }
        case NodeKind::Less_ThanBoolean:
        {
            const auto *lessThanBooleanNode = static_cast<const Less_ThanBoolean *>(node);
            return evaluate(lessThanBooleanNode->left) < evaluate(lessThanBooleanNode->right);
        }
        case NodeKind::Greater_ThanBoolean:
        {
            const auto *greaterThanBooleanNode = static_cast<const Greater_ThanBoolean *>(node);
            return evaluate(greaterThanBooleanNode->left) > evaluate(greaterThanBooleanNode->right);
        }
        case NodeKind::LessEqualBoolean:
        {
            const auto *lessEqualBooleanNode = static_cast<const LessEqualBoolean *>(node);
            return evaluate(lessEqualBooleanNode->left) <= evaluate(lessEqualBooleanNode->right);
        }
        case NodeKind::GreaterEqualBoolean:
        {
            const auto *greaterEqualBooleanNode = static_cast<const GreaterEqualBoolean *>(node);
            return evaluate(greaterEqualBooleanNode->left) >= evaluate(greaterEqualBooleanNode->right);
        }
        case NodeKind::OrBoolean:
        {
            const auto *orBooleanNode = static_cast<const OrBoolean *>(node);
            return evaluate(orBooleanNode->left) || evaluate(orBooleanNode->right);
        }
        case NodeKind::AndBoolean:
        {
            const auto *andBooleanNode = static_cast<const AndBoolean *>(node);
            return evaluate(andBooleanNode->left) && evaluate(andBooleanNode->right);
        }
        case NodeKind::UnaryNotBoolean:
        {
            const auto *unaryNotBooleanNode = static_cast<const UnaryNotBoolean *>(node);
            return !evaluate(unaryNotBooleanNode->expr);
        }
        case NodeKind::SumExpr:
        {
            const auto *sumNode = static_cast<const SumExpr *>(node);
            return evaluate(sumNode->left) + evaluate(sumNode->right);
        }
        case NodeKind::SubExpr:
        {
            const auto *subNode = static_cast<const SubExpr *>(node);
            return evaluate(subNode->left) - evaluate(subNode->right);
        }
        case NodeKind::MulExpr:
        {
            const auto *mulNode = static_cast<const MulExpr *>(node);
            int leftValue = evaluate(mulNode->left);
            int rightValue = evaluate(mulNode->right);
            return evaluate(mulNode->left) * evaluate(mulNode->right);
        }
        case NodeKind::DivExpr:
        {
            const auto *divNode = static_cast<const DivExpr *>(node);
            return evaluate(divNode->left) / evaluate(divNode->right);
        }
        case NodeKind::ModExpr:
        {
            const auto *modNode = static_cast<const ModExpr *>(node);
            return evaluate(modNode->left) % evaluate(modNode->right);
        }
        case NodeKind::UnaryAddExpr:
        {
            const auto *unaryAddNode = static_cast<const UnaryAddExpr *>(node);
            return evaluate(unaryAddNode->expr);
        }
        case NodeKind::UnarySubExpr:
        {
            const auto *unarySubNode = static_cast<const UnarySubExpr *>(node);
            return -evaluate(unarySubNode->expr);
        }
        case NodeKind::IntConst:
        case NodeKind::BooleanConst:
        {
            const auto *intNode = static_cast<const IntConst *>(node);
            return intNode->value;
        }
        case NodeKind::Parameter:{
            const auto *paramNode = static_cast<const Parameter *>(node);
            if (paramNode->sizeExpression) {
                int size = evaluate(paramNode->sizeExpression->expression);
                if (size <= 0) {
                    throw std::runtime_error("Invalid array size for parameter '" + paramNode->identifier + "'. Size must be greater than 0.");
                }
            }
            if (paramNode->isRef) {
                if (!isVariableDefined(paramNode->identifier)) {
                    throw std::runtime_error("Parameter '" + paramNode->identifier + "' is not defined.");
                }
            } 
            return 0;
        }
        case NodeKind::Variable:
        {
            const auto *varNode = static_cast<const Variable *>(node);
            const std::string &identifier = varNode->name;
            if (!isVariableDefined(identifier)) {
                throw std::runtime_error("Variable '" + identifier + "' is not defined.");
            }
            auto it = variables.find(identifier);
            if (it != variables.end() && it->second.has_value()) {
                return it->second->intVal;
            } else {
                throw std::runtime_error("Variable '" + identifier + "' is not initialized.");
            }
        }
        case NodeKind::FunctionCall: {
            const auto* callNode = static_cast<const FunctionCall*>(node);
            
            auto it = methodTable.find(callNode->name);
            if (it == methodTable.end()) {
                throw std::runtime_error("Function '" + callNode->name + "' not defined.");
            }

            const auto* fnDecl = it->second;
            
            if (callNode->args.size() != fnDecl->parameters->parameterList.size()) {
                throw std::runtime_error("Incorrect number of arguments for function '" + callNode->name + "'");
            }
        
            auto previousVariables = variables;
            auto previousArrays = arrays;
             for (size_t i = 0; i < callNode->args.size(); ++i)
            {
               const auto* param = fnDecl->parameters->parameterList[i];
                const auto* arg = callNode->args[i];
                
                if (arg->expression->kind() == NodeKind::Variable) {
                    const auto* varNode = static_cast<const Variable*>(arg->expression);
                    
                    if (variables.find(varNode->name) != variables.end()) {
                        if (param->isRef) {
                            variables[param->identifier] = variables[varNode->name];
                            variablesRef[param->identifier] = variables[param->identifier];
                        } else {
                            variables[param->identifier] = variables[varNode->name];
                        }
                    }
                    else if (arrays.find(varNode->name) != arrays.end()) {
                        if (param->isRef) {
                            arrays[param->identifier] = arrays[varNode->name];
                            arraysRef[param->identifier] = arrays[param->identifier];
                        } else {
                           arrays[param->identifier] = arrays[varNode->name];
                        }
                    } else {
                        throw std::runtime_error("Variable '" + varNode->name + "' not defined");
                    }
                } else if (arg->expression->kind() == NodeKind::ArrayVariable) {
                    const auto* arrayVarNode = static_cast<const ArrayVariable*>(arg->expression);
                    if (param->isRef) {
                        throw std::runtime_error("Cannot pass individual array elements by reference");
                    }
                    
                    auto arrayIt = arrays.find(arrayVarNode->name);
                    if (arrayIt == arrays.end()) {
                        throw std::runtime_error("Array '" + arrayVarNode->name + "' not found");
                    }

                    int index = evaluate(arrayVarNode->indexExpr);
                    if (index < 0 || index >= arrayIt->second.size()) {
                        throw std::runtime_error("Array index out of bounds for '" + arrayVarNode->name + "'");
                    }

                    if (!arrayIt->second[index].has_value()) {
                        throw std::runtime_error("Array element at index " + std::to_string(index) + " is not initialized");
                    }

                    Value arrayElement = arrayIt->second[index].value();
                    variables[param->identifier] = arrayElement;
                }
                else {
                    Value argValue = evaluate(arg->expression);
                    variables[param->identifier] = argValue;
                }
            }
            variables = variables;
            arrays = arrays;
            Value result = evaluate(fnDecl->functionBody);
            

            for (size_t i = 0; i < callNode->args.size(); ++i) {
                const auto* paramDecl = static_cast<const Parameter*>(fnDecl->parameters->parameterList[i]);
                const auto* argNode = callNode->args[i];

                if (paramDecl->isRef && argNode->expression->kind() == NodeKind::Variable) {
                    const auto* varNode = static_cast<const Variable*>(argNode->expression);
                    
                    if (variables.find(varNode->name) != variables.end()) {
                        variablesRef[paramDecl->identifier] = variables[paramDecl->identifier];
                    }
                    else if (arrays.find(varNode->name) != arrays.end()) {
                        arraysRef[paramDecl->identifier] = arrays[paramDecl->identifier];
                    }
                }
            }

            variables = previousVariables;
            arrays = previousArrays;

            for (size_t i = 0; i < callNode->args.size(); ++i) {
                const auto* paramDecl = static_cast<const Parameter*>(fnDecl->parameters->parameterList[i]);
                const auto* argNode = callNode->args[i];

                if (paramDecl->isRef && argNode->expression->kind() == NodeKind::Variable) {
                    const auto* varNode = static_cast<const Variable*>(argNode->expression);
                    
                    if (variables.find(varNode->name) != variables.end()) {
                        variables[varNode->name] = variablesRef[paramDecl->identifier];
                    }
                    else if (arrays.find(varNode->name) != arrays.end()) {
                        arrays[varNode->name] = arraysRef[paramDecl->identifier];
                    }
                }
            }

            variablesRef.clear();
            arraysRef.clear();
            
            return result.intVal;
        }
        case NodeKind::ArrayVariable:
        {
            const auto *arrayVarNode = static_cast<const ArrayVariable *>(node);
            const std::string &identifier = arrayVarNode->name;
            AstNode *indexExpr = arrayVarNode->indexExpr;

            if (!isVariableDefined(identifier)) {
                throw std::runtime_error("Array variable '" + identifier + "' is not defined.");
            }

            auto it = arrays.find(identifier);
            if (it != arrays.end()) {
                int index = evaluate(indexExpr);

                if (index < 0 || index >= it->second.size()) {
                    throw std::runtime_error("Index out of bounds for array variable '" + identifier + "'.");
                }
                if(!it->second[index].has_value()) {
                    throw std::runtime_error("Array variable '" + identifier + "' does not contain values.");
                }
                Value arrayValue = it->second[index].value();

                return arrayValue.intVal;
            } else {
                throw std::runtime_error("Array variable '" + identifier + "' is not initialized.");
            }
        }
        case NodeKind::PrintStatement:{
            const auto *printStmtNode = static_cast<const PrintStatement *>(node);
            if(printStmtNode->isStringLiteral) {
                std::cout << printStmtNode->identifier << std::endl;
            } else if (printStmtNode->expression) {
                int value = evaluate(printStmtNode->expression);
                if (printStmtNode->expression->kind() == NodeKind::Variable) {
                    const auto *varNode = static_cast<const Variable *>(printStmtNode->expression);
                    const std::string &identifier = varNode->name;

                    auto it = variables.find(identifier);
                    if (it != variables.end() && it->second.has_value()) {
                        Value varValue = it->second.value();
                        if (varValue.type == Value::Int) {
                            std::cout << varValue.intVal << std::endl;
                        } else if (varValue.type == Value::Bool) {
                            std::cout << (varValue.boolVal ? "true" : "false") << std::endl;
                        }
                    } else {
                        throw std::runtime_error("Variable '" + identifier + "' is not initialized.");
                    }

                }else if (printStmtNode->expression->kind() == NodeKind::ArrayVariable) {
                    const auto *arrayVarNode = static_cast<const ArrayVariable *>(printStmtNode->expression);
                    const std::string &identifier = arrayVarNode->name;
                    AstNode *indexExpr = arrayVarNode->indexExpr;
                    auto it = arrays.find(identifier);
                    
                    if (it != arrays.end()) {
                        int index = evaluate(indexExpr);
                        Value arrayValue = it->second[index].value();
                        if (arrayValue.type == Value::Int) {
                            std::cout << arrayValue.intVal << std::endl;
                        } else if (arrayValue.type == Value::Bool) {
                            std::cout << (arrayValue.boolVal ? "true" : "false") << std::endl;
                        }
                    } else {
                        throw std::runtime_error("Array variable '" + identifier + "' is not initialized.");
                    }
                }else{
                    std::cout << value << std::endl;
                }
                
            } else {
                throw std::runtime_error("Print statement has no expression or identifier.");
            }
            return 0;
        }
       case NodeKind::AssignmentValues: {
            const auto *assignNode = static_cast<const AssignStament *>(node);
            Value assignValue = evaluate(assignNode->expr);

            if(assignNode->isArray) {
                if (assignNode->index) {
                    if (!isVariableDefined(assignNode->identifier)) {
                        throw std::runtime_error("array '" + assignNode->identifier + "' is not defined.");
                    }
                    
                    int index = evaluate(assignNode->index);
                    auto& array = arrays[assignNode->identifier];
                    
                    if (index < 0 || index >= array.size()) {
                        throw std::runtime_error("Index out of bounds for array variable '" + assignNode->identifier + "'.");
                    }

                    // Check type compatibility with first element's type
                    if (array[0].has_value()) {
                        Value firstElement = array[0].value();
                       
                        int value = assignValue.intVal;
                        // Additional check for boolean arrays
                        if (firstElement.type == Value::Bool) {
                          
                            if (value != 0 && value != 1) {
                                throw std::runtime_error("Invalid boolean value for array '" + 
                                    assignNode->identifier + "'. Expected 0 or 1.");
                            }
                            // Convert to proper boolean
                            assignValue = Value(value == 1);
                        }else{
                           assignValue = Value(value);
                        }
                    }
                    arrays[assignNode->identifier][index] = assignValue;
                    

                } else {
                    throw std::runtime_error("Array assignment requires an index.");
                }
            } else {
                if (!isVariableDefined(assignNode->identifier)) {
                    throw std::runtime_error("Variable '" + assignNode->identifier + "' is not defined.");
                }

                auto it = variables.find(assignNode->identifier);
                if (it != variables.end() && it->second.has_value()) {
                    Value currentValue = it->second.value();

                }
                
                if(it->second->type == Value::Bool) {
                    int value = assignValue.intVal;

                    if (value != 0 && value != 1) {
                        throw std::runtime_error("Invalid boolean value for array '" + 
                            assignNode->identifier + "'. Expected 0 or 1.");
                    }
                    std::cout << "Assigning boolean value to variable: " << assignNode->identifier << std::endl;
                    assignValue = Value(value == 1);

                } else if(it->second->type == Value::Int) {
                    assignValue = Value(assignValue.intVal);
                }
                variables[assignNode->identifier] = assignValue;
            }
            
            return assignValue.type == Value::Int ? assignValue.intVal : assignValue.boolVal;
        }
        case NodeKind::Statements:{
            const auto *statements= static_cast<const statementsNode *>(node);
            int result = 0;
            for (const auto &stmt : statements->statements) {
                result = evaluate(stmt);
            }
            return result;
        }
        case NodeKind::ReturnStatement:{
            const auto *returnNode = static_cast<const ReturnStatement *>(node);
            if (returnNode->expr) {
                return evaluate(returnNode->expr);
            } else {
                return 0;
            }
        }
        case NodeKind::whileStatement: {
            const auto *whileNode = static_cast<const WhileStatement *>(node);
            int result = 0;

            while (evaluate(whileNode->condition)) {
                result = evaluate(whileNode->body);
            }
            return result;
        }
        case NodeKind::forStatement: {
            const auto *forNode = static_cast<const ForStament *>(node);
            int result = 0;
            int step = 1;
            
            if (isVariableDefined(forNode->identifier)) {
                throw std::runtime_error("Loop variable '" + forNode->identifier + "' is already defined.");
            }
            variables[forNode->identifier] = Value(evaluate(forNode->startExpr));
            if(forNode->stepExpr) {
                step = evaluate(forNode->stepExpr);
            }
            int valueInitial = variables[forNode->identifier]->intVal;
            int valueEnd = evaluate(forNode->endExpr);
            while (variables[forNode->identifier]->intVal <= evaluate(forNode->endExpr)) {
                result = evaluate(forNode->body);
                variables[forNode->identifier]->intVal += step;
            }
            return result;
        }
        case NodeKind::ConditionalStatement: {
            const auto *condNode = static_cast<const ConditionalStatement *>(node);
            int result = 0;
            result = evaluate(condNode->ifStatement);
            if (result != 0) {

                return evaluate(condNode->ifStatement->statements); 
            }

            for (const auto &elseIf : condNode->elseIfStatements) {
                result = evaluate(elseIf);
                if (result != 0) {
                    return evaluate(elseIf->statements);
                }
            }

            if (condNode->elseStatement) {
                result = evaluate(condNode->elseStatement);
            }

            return result;
        }
        case NodeKind::ifStatement: {
            const auto *ifNode = static_cast<const IfStatement *>(node);
            if (evaluate(ifNode->condition)) {
                return 1;
            }
            return 0;
        }
        case NodeKind::elseIfStatement: {
            const auto *elseIfNode = static_cast<const ElseIfStatement *>(node);
            if (evaluate(elseIfNode->condition)) {
                return 1;
            }
            return 0;
        }
        case NodeKind::elseStatement: {
            const auto *elseNode = static_cast<const ElseStatement *>(node);
            int value = evaluate(elseNode->statements);
            return value;
        }
        case NodeKind::ReadFunctions: {
            int value;
            std::cin >> value;
            return value;
        }
        case NodeKind::TypeNameNode: {
            const auto *typeNode = static_cast<const TypeNameNode *>(node);
            switch (typeNode->type) {
                case EnumVarType::Int:
                    return 0;
                case EnumVarType::Bool:
                    return false;
                default:
                    throw std::runtime_error("Unsupported type encountered during evaluation.");
            }
        }
        case NodeKind::TypeNameFunctionNode: {
            const auto *typeNode = static_cast<const TypeNameFunctionNode *>(node);
            switch (typeNode->type) {
                case EnumFunctionType::Int:
                    return 0;
                case EnumFunctionType::Bool:
                    return false;
                case EnumFunctionType::Void:
                    return 0;
                default:
                    throw std::runtime_error("Unsupported function type encountered during evaluation.");
            }
        }
        case NodeKind::SizeExpression: {
            const auto *sizeExprNode = static_cast<const sizeExpressionNode *>(node);
            return evaluate(sizeExprNode->expression);
        }
        default:
           int nodeType = static_cast<int>(node->kind());
        throw std::runtime_error("Unsupported node type (" + std::to_string(nodeType) + ") encountered during evaluation.");
    }
}




bool SimplEvaluator::isVariableDefined(const std::string& identifier) const {
    if (variables.find(identifier) !=variables.end() ||
        arrays.find(identifier) != arrays.end()) {
        return true;
    }


    return false;
}

#include "SimplAst.hpp"

std::string programNode::toString() const {
    std::string result = "Program:\n";
    for (const auto& decl : globalDeclarations) {
        result += "  " + decl->toString();
        if (result.back() != '\n') {
            result += '\n';
        }
        result += '\n';
    } 
    return result;
}

std::string GlobalFnDeclareNode::toString() const {
    std::string result = "Function " + functionName + "(";
    if (parameters) {
        result += parameters->toString();
    }
    result += ") -> " + returnType->toString() + " {\n";
    if (functionBody) {
        result += functionBody->toString();
    }
    result += "\n}";
    return result;
}

std::string statementsNode::toString() const {
    std::string result = "Statements:\n";
    for (const auto& stmt : statements) {
        result += "  " + stmt->toString() + "\n";
    }
    return result;
}

std::string AssignStament::toString() const {
    std::string result = identifier;
    if (isArray && index) {
        result += "[" + index->toString() + "]";
    }
    result += " = ";
    if (expr) {
        result += expr->toString();
    }
    return result;
}

std::string ReturnStatement::toString() const {
    return "return " + (expr ? expr->toString() : "");
}

std::string ConditionalStatement::toString() const {
    std::string result = ifStatement->toString();
    for (const auto& elseIf : elseIfStatements) {
        result += "\n" + elseIf->toString();
    }
    if (elseStatement) {
        result += "\n" + elseStatement->toString();
    }
    return result;
}

std::string IfStatement::toString() const {
    return "if (" + condition->toString() + ") {\n" 
           + statements->toString() + "\n}";
}

std::string ElseIfStatement::toString() const {
    return "else if (" + condition->toString() + ") {\n" 
           + statements->toString() + "\n}";
}

std::string ElseStatement::toString() const {
    return "else {\n" + statements->toString() + "\n}";
}

std::string WhileStatement::toString() const {
    return "while (" + condition->toString() + ") {\n" 
           + body->toString() + "\n}";
}

std::string ForStament::toString() const {
    std::string result = "for " + identifier + " = " + startExpr->toString();
    result += " to " + endExpr->toString();
    if (stepExpr) {
        result += " step " + stepExpr->toString();
    }
    result += " {\n" + body->toString() + "\n}";
    return result;
}

std::string PrintStatement::toString() const {
    std::string result = "print(" + identifier;
    if (sizeExpression) {
        result += "[" + sizeExpression->toString() + "]";
    }
    result += ")";
    return result;
}

std::string ParameterList::toString() const {
    std::string result;
    for (size_t i = 0; i < parameterList.size(); ++i) {
        result += parameterList[i]->toString();
        if (i < parameterList.size() - 1) {
            result += ", ";
        }
    }
    return result;
}

std::string Parameter::toString() const {
    std::string result = identifier + ": ";
    if (isRef) {
        result += "ref ";
    }
    result += type->toString();
    if (sizeExpression) {
        result += "[" + sizeExpression->toString() + "]";
    }
    return result;
}
std::string TypeNameFunctionNode::toString() const
{
    switch (type)
    {
        case EnumFunctionType::Int:
            return "Int";
        case EnumFunctionType::Bool:
            return "Bool";
        case EnumFunctionType::Void:
            return "Void";
        default:
            return "Unknown";
    }
}


std::string GlobalVarDeclNode::toString() const
{
    std::string result = type->toString();
    result += " " + identifier;

    if (isArray)
    {
        result += " [Array]";
        if (sizeExpression)
        {
            result += " Size: " + sizeExpression->toString();
        }
    }

    if (initializer)
    {
        result += " = " + initializer->toString();
    }

    return result;
}

std::string Initializer::toString() const {
    if (isArray) {
        std::string result = "Initializer(Array): [";
        for (const auto& expr : arrayExpressions) {
            result += expr->toString() + ", ";
        }
        if (!arrayExpressions.empty()) {
            result.pop_back();
            result.pop_back();
        }
        result += "]";
        return result;
    } else {
        return "Initializer(Expression): " + (expression ? expression->toString() : "null");
    }
}

std::string ReadFunctions::toString() const
{
    return "ReadFunction: " + name;
}

std::string TypeNameNode::toString() const
{
    return type;
}

std::string sizeExpressionNode::toString() const
{
    return "SizeExpression: " + (expression ? expression->toString() : "null");
}

std::string EqualBoolean::toString() const
{
    return "(" + left->toString() + " == " + right->toString() + ")";
}

std::string NEqualBoolean::toString() const
{
    return "(" + left->toString() + " != " + right->toString() + ")";
}

std::string Less_ThanBoolean::toString() const
{
    return "(" + left->toString() + " < " + right->toString() + ")";
}

std::string Greater_ThanBoolean::toString() const
{
    return "(" + left->toString() + " > " + right->toString() + ")";
}

std::string LessEqualBoolean::toString() const
{
    return "(" + left->toString() + " <= " + right->toString() + ")";
}

std::string GreaterEqualBoolean::toString() const
{
    return "(" + left->toString() + " >= " + right->toString() + ")";
}

std::string UnaryNotBoolean::toString() const
{
    return "not " + expr->toString();
}

std::string AndBoolean::toString() const
{
    return left->toString() + " and " + right->toString();
}

std::string OrBoolean::toString() const
{
    return left->toString() + " or " + right->toString();
}

std::string UnaryAddExpr::toString() const
{
    return "+" + expr->toString();
}

std::string UnarySubExpr::toString() const
{
    return "-" + expr->toString();
}

std::string SumExpr::toString() const
{
    return "(" + left->toString() + " + " + right->toString() + ")";
}

std::string SubExpr::toString() const
{
    return "(" + left->toString() + " - " + right->toString() + ")";
}

std::string MulExpr::toString() const
{
    return "(" + left->toString() + " * " + right->toString() + ")";
}

std::string DivExpr::toString() const
{
    return "(" + left->toString() + " / " + right->toString() + ")";
}

std::string ModExpr::toString() const
{
    return "(" + left->toString() + " % " + right->toString() + ")";
}

std::string IntConst::toString() const
{
    return std::to_string(value);
}


std::string BooleanConst::toString() const
{
    return std::to_string(value);
}
std::string Variable::toString() const
{
    return name;
}

std::string ArrayVariable::toString() const
{
    return name + "[" + (indexExpr ? indexExpr->toString() : "null") + "]";
}

std::string FunctionCall::toString() const
{
    std::string result = name + "- FC (";
    for (size_t i = 0; i < args.size(); ++i)
    {
        result += args[i]->toString();
        if (i < args.size() - 1)
            result += ", ";
    }
    result += ")";
    return result;
}

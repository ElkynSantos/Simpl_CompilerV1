#include "SimplAst.hpp"


std::string InputNode::toString() const
{
    return "InputNode: " + (program ? program->toString() : "null");
}

std::string programNode::toString() const
{
    std::string result = "Program: ";
    
    for (const auto &decl : GlobalVarDeclNode)
    {
        result += decl->toString() + "\n";
    }
    
    return result;
}

std::string GlobalVarDeclNode::toString() const
{
    std::string result = type->toString();
    result += " " + identifier;
    return result;
}

std::string TypeName::toString() const
{
    return type + (arraySize >= 0 ? std::to_string(arraySize) + "]" : "");
}

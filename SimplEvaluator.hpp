#ifndef _EVALUATOR_HPP_
#define _EVALUATOR_HPP_
#include <string>
#include <vector>
#include "SimplAst.hpp"
#include <optional>

struct Value {
    enum Type { Int, Bool } type;
    union {
        int intVal;
        bool boolVal;
    };
    
    Value(int v) : type(Int), intVal(v) {}
    Value(bool v) : type(Bool), boolVal(v) {}
};


class SimplEvaluator {
    public:
    SimplEvaluator() {}
    
    ~SimplEvaluator() = default;
    int evaluate(AstNode* node);

    bool isVariableDefined(const std::string& identifier) const;

    private:
    std::unordered_map<std::string, const GlobalFnDeclareNode *> methodTable;
    std::unordered_map<std::string, std::optional<Value>> variables;
    std::unordered_map<std::string, std::vector<std::optional<Value>>> arrays;
    std::unordered_map<std::string, std::optional<Value>> variablesRef;
    std::unordered_map<std::string, std::vector<std::optional<Value>>> arraysRef;
    void restoreReferences(const FunctionCall* callNode, 
                          const GlobalFnDeclareNode* fnDecl);
    

};

#endif

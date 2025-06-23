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

struct Scope {
    std::unordered_map<std::string, std::optional<Value>> variables;
    std::unordered_map<std::string, std::vector<std::optional<Value>>> arrays;
};

class SimplEvaluator {
    public:
    SimplEvaluator() {
        currentScope = new Scope();
        varsByFunctions["global"] = currentScope;
    }
    
    ~SimplEvaluator() {
        for (auto& pair : varsByFunctions) {
            delete pair.second;
        }
    }
    Scope* currentScope;
    std::string currentFunction = "global";
    std::string lastFunction = "";
    int evaluate(AstNode* node);
    bool isVariableDefined(const std::string& identifier) const;
    void enterFunctionScope(const std::string& functionName);
    void exitFunctionScope();
    void printScope(const std::string& scopeName = "") const;

    private:
    std::unordered_map<std::string, Scope*> varsByFunctions;
    std::unordered_map<std::string, const GlobalFnDeclareNode *> methodTable;
    

};

#endif

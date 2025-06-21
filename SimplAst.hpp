#ifndef _AST_H
#define _AST_H
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

class programNode;
class GlobalFnDeclareNode;
class GlobalVarDeclNode;
class TypeNameFunctionNode;
class TypeNameNode;
class sizeExpressionNode;
class Initializer;
class UnaryExpr;
class BinaryExpr;
class EqualBoolean;
class NEqualBoolean;
class Less_ThanBoolean;
class Greater_ThanBoolean;
class LessEqualBoolean;
class GreaterEqualBoolean;
class UnaryNotBoolean;
class AndBoolean;
class OrBoolean;
class UnaryAddExpr;
class UnarySubExpr;
class SumExpr;
class SubExpr;
class MulExpr;
class DivExpr;
class ModExpr;
class IntConst;
class BooleanConst;
class ReadFunctions;
class Variable;
class ArrayVariable;
class FunctionCall;
class ParameterList;
class Parameter;
class statementsNode;
class IfStatement;
class ElseIfStatement;
class ElseStatement;
class whileStatement;
class forStament;
class PrintStatement;
class AssignStament;
class ConditionalStatement;
class ReturnStatement;
class GlobalDeclaration;
class ArgumentNode;



enum class EnumFunctionType{
  Int,
  Bool,
  Void
};
enum class EnumVarType{
  Int,
  Bool,
};


enum class NodeKind{
    Program,
    GlobalVarDeclare,
    TypeNameNode,
    TypeNameFunctionNode,
    SizeExpression,
    Initializer,
    ArrayInitializer,
    Expression,
    Term,
    Factor,
    Primary,
    GlobalFnDeclare,
    ParameterList,
    Parameter,
    FunctionTypes,
    Statements,
    Statement,
    ConditionalStatement,
    ifStatement,
    elseIfStatement,
    elseStatement,

    ArgumentList,
    Argument,
    ReturnStatement,
    Loop,
    whileStatement,
    forStatement,
    AssignmentValues,
     // Booleans Binary
    EqualBoolean,
    NEqualBoolean,
    Less_ThanBoolean,
    Greater_ThanBoolean,
    LessEqualBoolean,
    GreaterEqualBoolean,
    OrBoolean,
    AndBoolean,
    EqualExpr,
    UnaryNotBoolean,

    SumExpr,
    SubExpr,
    MulExpr,
    DivExpr,
    ModExpr,
    UnaryAddExpr,
    UnarySubExpr,
    IntConst,
    Variable,
    ArrayVariable,
    FunctionCall,
    ReadFunctions,
    StringLiteral,
    PrintStatement,
        
};

class AstNode {

  public:
      AstNode()=default;
      virtual ~AstNode() = default;

      virtual NodeKind kind() const = 0;
      virtual std::string toString() const = 0;
};

class GlobalDeclaration : public AstNode {
public:
    virtual ~GlobalDeclaration() = default;
};

class programNode : public AstNode {
public:
    programNode(const std::vector<GlobalDeclaration*>& declarations)
        : globalDeclarations(declarations) {}

    NodeKind kind() const override {
        return NodeKind::Program;
    }

    std::string toString() const override;

    std::vector<GlobalDeclaration*> globalDeclarations;
};

class GlobalFnDeclareNode : public GlobalDeclaration {
public:
    GlobalFnDeclareNode(const std::string& name, 
                        ParameterList* parameters,
                        TypeNameFunctionNode* returnType,
                        statementsNode* body)
        : functionName(name), 
          parameters(parameters),
          returnType(returnType),
          functionBody(body) {}

    NodeKind kind() const override {
        return NodeKind::GlobalFnDeclare;
    }

    std::string toString() const override;

    std::string functionName;
    ParameterList* parameters;
    TypeNameFunctionNode* returnType;
    statementsNode* functionBody;
};

class statementsNode : public AstNode
{
  public:
    statementsNode(const std::vector<AstNode *> &statements)
        : statements(statements) {}

    NodeKind kind() const override
    {
      return NodeKind::Statements;
    }

    std::string toString() const override;

    std::vector<AstNode *> statements;
};

class AssignStament: public AstNode
{
public:
  AssignStament(const std::string &identifier, sizeExpressionNode* index, Initializer *expr, bool isArray)
      : identifier(identifier), index(index), expr(expr), isArray(isArray){}

  NodeKind kind() const override
  {
    return NodeKind::AssignmentValues;
  }

  std::string toString() const override;

  std::string identifier;
  sizeExpressionNode *index;
  Initializer *expr;
  bool isArray = false;
};

class ReturnStatement : public AstNode
{
  public:
    ReturnStatement(AstNode *expr) : expr(expr) {}

    NodeKind kind() const override
    {
      return NodeKind::ReturnStatement;
    }

    std::string toString() const override;

    AstNode *expr;
};

class ConditionalStatement : public AstNode
{
public:
    ConditionalStatement(IfStatement *ifStmt, std::vector<ElseIfStatement *> elseIfStmts, ElseStatement *elseStmt)
        : ifStatement(ifStmt), elseIfStatements(elseIfStmts), elseStatement(elseStmt) {}

    NodeKind kind() const override {
        return NodeKind::ConditionalStatement;
    }

    std::string toString() const override;

    IfStatement *ifStatement;
    std::vector<ElseIfStatement *> elseIfStatements;
    ElseStatement *elseStatement;
};

class IfStatement : public AstNode {
public:
    IfStatement(AstNode *condition, AstNode *statements) 
        : condition(condition), statements(statements) {}

    NodeKind kind() const override {
        return NodeKind::ifStatement;
    }

    std::string toString() const override;

    AstNode *condition;
    AstNode *statements;
};

class ElseIfStatement : public AstNode
{
  public:
    ElseIfStatement(AstNode *condition, AstNode *statements) : condition(condition), statements(statements) {}

    NodeKind kind() const override
    {
      return NodeKind::elseIfStatement;
    }

    std::string toString() const override;

    AstNode *condition;
    AstNode *statements;
};

class ElseStatement : public AstNode
{
  public:
    ElseStatement(AstNode *statements) : statements(statements) {}

    NodeKind kind() const override
    {
      return NodeKind::elseStatement;
    }

    std::string toString() const override;

    AstNode *statements;
};


class WhileStatement : public AstNode
{
public:
  WhileStatement(AstNode *condition, statementsNode *body) : condition(condition), body(body) {}

  NodeKind kind() const override
  {
    return NodeKind::whileStatement;
  }

  std::string toString() const override;

  AstNode *condition;
  statementsNode *body;
};

class ForStament : public AstNode
{
  public:
    ForStament(const std::string &identifier, AstNode *startExpr, AstNode *endExpr, AstNode *stepExpr, AstNode *body)
        : identifier(identifier), startExpr(startExpr), endExpr(endExpr), stepExpr(stepExpr), body(body) {}

    NodeKind kind() const override
    {
      return NodeKind::forStatement;
    }

    std::string toString() const override;

    std::string identifier;
    AstNode *startExpr;
    AstNode *endExpr;
    AstNode *stepExpr;
    AstNode *body;
};

class PrintStatement : public AstNode
{
  public:
    PrintStatement(std::string& identifier, sizeExpressionNode* sizeExpression) : identifier(identifier)
    ,sizeExpression(sizeExpression) {}

    NodeKind kind() const override
    {
      return NodeKind::PrintStatement;
    }

    std::string toString() const override;
  
    std::string identifier;
    sizeExpressionNode* sizeExpression;
    };


class TypeNameFunctionNode : public AstNode
{
  public:
    TypeNameFunctionNode(EnumFunctionType type) : type(type) {}

    NodeKind kind() const override
    {
      return NodeKind::TypeNameFunctionNode;
    }
    
    std::string toString() const override;

    EnumFunctionType type;

};


class ParameterList : public AstNode
{
  public:
    ParameterList(const std::vector<Parameter *> & parameterList)
        : parameterList(parameterList) {}

    NodeKind kind() const override
    {
      return NodeKind::ParameterList;
    }

    std::string toString() const override;
    std::vector<Parameter *> parameterList;
};
class Parameter : public AstNode
{
  public:
    Parameter(const std::string &identifier, TypeNameNode* type, 
    sizeExpressionNode* sizeExpression , bool isRef)
        : identifier(identifier), type(type), sizeExpression(sizeExpression), isRef(isRef) {}

    NodeKind kind() const override
    {
      return NodeKind::Parameter;
    }

    std::string toString() const override;

    std::string identifier;
    sizeExpressionNode* sizeExpression;
    TypeNameNode* type;
    bool isRef;

};

class GlobalVarDeclNode : public GlobalDeclaration
{
  public:
  GlobalVarDeclNode(const std::string& identifier, TypeNameNode* type, sizeExpressionNode* sizeExpression, AstNode* initializer, bool isArray)
        : identifier(identifier), 
        type(type), sizeExpression(sizeExpression), initializer(initializer), isArray(isArray) {}
  
  

  NodeKind kind() const override {
      return NodeKind::GlobalVarDeclare;
  }


  std::string toString() const override;
  std::string identifier;
  TypeNameNode* type;
  sizeExpressionNode* sizeExpression;
  AstNode* initializer;
  bool isArray;
};
class TypeNameNode : public AstNode
{
   public:
    TypeNameNode(EnumVarType type) : type(type) {}
    NodeKind kind() const override
    {
      return NodeKind::TypeNameFunctionNode;
    }
    
    std::string toString() const override;

    EnumVarType type;
};
class sizeExpressionNode : public AstNode
{
  public:
    sizeExpressionNode(AstNode* expression) : expression(expression) {}

    NodeKind kind() const override
    {
      return NodeKind::SizeExpression;
    }

    std::string toString() const override;

    AstNode* expression;

};

class Initializer : public AstNode
{
  public:
    Initializer(AstNode* expression) 
    : expression(expression), isArray(false) {}

    Initializer(const std::vector<AstNode*>& arrayExpressions) 
        : arrayExpressions(arrayExpressions), isArray(true) {}

    NodeKind kind() const override
    {
      return NodeKind::Initializer;
    }

    std::string toString() const override;
    AstNode* expression = nullptr;
    std::vector<AstNode*> arrayExpressions;
    bool isArray;
};


class UnaryExpr : public AstNode
{
public:
  UnaryExpr(AstNode *expr) : expr(expr) {}
  ~UnaryExpr() override = default;

  AstNode *expr;
};
class BinaryExpr : public AstNode
{
public:
  BinaryExpr(AstNode *left, AstNode *right) : left(left), right(right) {}

  AstNode *left;
  AstNode *right;
};

// -------------------------------------------------------------------------
class EqualBoolean : public BinaryExpr
{
public:
  EqualBoolean(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::EqualBoolean;
  }

  std::string toString() const override;
};

class NEqualBoolean : public BinaryExpr
{

public:
  NEqualBoolean(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::NEqualBoolean;
  }

  std::string toString() const override;
};

class Less_ThanBoolean : public BinaryExpr
{
public:
  Less_ThanBoolean(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::Less_ThanBoolean;
  }

  std::string toString() const override;
};

class Greater_ThanBoolean : public BinaryExpr
{
public:
  Greater_ThanBoolean(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::Greater_ThanBoolean;
  }

  std::string toString() const override;
};

class LessEqualBoolean : public BinaryExpr
{

public:
  LessEqualBoolean(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::LessEqualBoolean;
  }

  std::string toString() const override;
};

class GreaterEqualBoolean : public BinaryExpr
{

public:
  GreaterEqualBoolean(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::GreaterEqualBoolean;
  }

  std::string toString() const override;
};

class UnaryNotBoolean : public UnaryExpr
{

public:
  UnaryNotBoolean(AstNode *expr) : UnaryExpr(expr) {}

  NodeKind kind() const override
  {
    return NodeKind::UnaryNotBoolean;
  }

  std::string toString() const override;
};

class AndBoolean : public BinaryExpr
{
public:
  AndBoolean(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::AndBoolean;
  }

  std::string toString() const override;
};

class OrBoolean : public BinaryExpr
{
public:
  OrBoolean(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::OrBoolean;
  }

  std::string toString() const override;
};



class UnaryAddExpr : public UnaryExpr
{
public:
  UnaryAddExpr(AstNode *expr) : UnaryExpr(expr) {}

  NodeKind kind() const override
  {
    return NodeKind::UnaryAddExpr;
  }

  std::string toString() const override;
};

class UnarySubExpr : public UnaryExpr
{
public:
  UnarySubExpr(AstNode *expr) : UnaryExpr(expr) {}

  NodeKind kind() const override
  {
    return NodeKind::UnarySubExpr;
  }

  std::string toString() const override;
};

class SumExpr : public BinaryExpr
{
public:
  SumExpr(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::SumExpr;
  }

  std::string toString() const override;
};

class SubExpr : public BinaryExpr
{
public:
  SubExpr(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::SubExpr;
  }

  std::string toString() const override;
};

class MulExpr : public BinaryExpr
{
public:
  MulExpr(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::MulExpr;
  }

  std::string toString() const override;
};

class DivExpr : public BinaryExpr
{
public:
  DivExpr(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::DivExpr;
  }

  std::string toString() const override;
};

class ModExpr : public BinaryExpr
{
public:
  ModExpr(AstNode *left, AstNode *right) : BinaryExpr(left, right) {}

  NodeKind kind() const override
  {
    return NodeKind::ModExpr;
  }

  std::string toString() const override;
};

class IntConst : public AstNode
{
public:
  IntConst(int value) : value(value) {}

  NodeKind kind() const override
  {
    return NodeKind::IntConst;
  }

  std::string toString() const override;

  int value;
};

class BooleanConst : public AstNode
{
public:
  BooleanConst(bool value) : value(value) {}

  NodeKind kind() const override
  {
    return NodeKind::IntConst;
  }

  std::string toString() const override;

  bool value;
};

class ReadFunctions : public AstNode
{
public:
  ReadFunctions(const std::string &name) : name(name) {}

  NodeKind kind() const override
  {
    return NodeKind::ReadFunctions;
  }

  std::string toString() const override;

  std::string name;
};

class Variable : public AstNode
{
public:
  Variable(const std::string &name) : name(name) {}

  NodeKind kind() const override
  {
    return NodeKind::Variable;
  }

  std::string toString() const override;

  std::string name;
};

class ArrayVariable : public AstNode
{
public:
  ArrayVariable(const std::string &name, AstNode *indexExpr)
      : name(name), indexExpr(indexExpr) {}

  NodeKind kind() const override
  {
    return NodeKind::ArrayVariable;
  }

  std::string toString() const override;

  std::string name;
  AstNode *indexExpr;
};

class ArgumentNode : public AstNode {
public:
    ArgumentNode(AstNode* expr, bool isRef = false) 
        : expression(expr), isRef(isRef) {}

    NodeKind kind() const override {
        return NodeKind::Argument;
    }

    std::string toString() const override;
    AstNode* expression;
    bool isRef;
};
class FunctionCall : public AstNode
{
public:
  FunctionCall(const std::string &name, const std::vector<ArgumentNode *> &args)
      : name(name), args(args) {}

  NodeKind kind() const override
  {
    return NodeKind::FunctionCall;
  }

  std::string toString() const override;

  std::string name;
  std::vector<ArgumentNode *> args;
};




#endif
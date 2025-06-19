#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

enum class NodeKind{
    InputNode,
    Program,
    GlobalVarDeclare,
    TypeName,
    SizeExpression,
    ArrayInitializer,
    Expression,
    Term,
    Factor,
    Primary,
    GlobalFnDeclare,
    ParamList,
    Param,
    FunctionTypes,
    Statements,
    Statement,
    ConditionalStatement,
    ArgumentList,
    ReturnStatement,
    Loop,
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
    StringLiteral,
        
};

class AstNode {

    public:
        AstNode()=default;
        virtual ~AstNode() = default;

        virtual NodeKind kind() const = 0;
        virtual std::string toString() const = 0;
};

class InputNode : public AstNode
{
    public:
    InputNode(AstNode *program) : program(program) {}

    NodeKind kind() const override
    {
        return NodeKind::InputNode;
    }

    std::string toString() const override;

    AstNode *program;
};

class programNode : public AstNode
{
    public:
    programNode(const std::vector<AstNode *> &GlobalVarDeclNode)
    : GlobalVarDeclNode(GlobalVarDeclNode){}

    NodeKind kind() const override
    {
        return NodeKind::Program;
    }

    std::string toString() const override;

    std::vector<AstNode *> GlobalVarDeclNode;
    // std::vector<AstNode *> functionsDeclList;
};

class GlobalVarDeclNode : public AstNode
{
    public:
    GlobalVarDeclNode(const std::string& identifier, AstNode* type, AstNode* sizeExpression, AstNode* initializer, bool isArray)
        : identifier(identifier), type(type), initializer(initializer), isArray(isArray) {}

    NodeKind kind() const override {
        return NodeKind::GlobalVarDeclare;
    }


    std::string toString() const override;

    std::string identifier;
    AstNode* type;
    AstNode* sizeExpression;
    AstNode* initializer;
    bool isArray;
};

class TypeName : public AstNode
{
public:
  TypeName(const std::string &type, int arraySize) : type(type), arraySize(arraySize) {}

  NodeKind kind() const override
  {
    return NodeKind::TypeName;
  }

  std::string toString() const override;

  std::string type;
  int arraySize;
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

class FunctionCall : public AstNode
{
public:
  FunctionCall(const std::string &name, const std::vector<AstNode *> &args)
      : name(name), args(args) {}

  NodeKind kind() const override
  {
    return NodeKind::FunctionCall;
  }

  std::string toString() const override;

  std::string name;
  std::vector<AstNode *> args;
};
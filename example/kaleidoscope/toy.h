#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include <avian/util/string.h>
#include <avian/util/list.h>
#include <avian/util/fixed-allocator.h>
#include <avian/util/optional.h>
#include <avian/codegen/ir.h>
#include <avian/codegen/link.h>
#include <avian/codegen/builder.h>
#include <avian/codegen/compiler.h>
#include <avian/codegen/optimize.h>
#include <avian/codegen/architecture.h>
#include <avian/codegen/targets.h>
#include <avian/heap/heap.h>
#include <avian/system/system.h>
#include <avian/zone.h>

#undef assert

#include <assert.h>

class Token {
 public:
  enum Type {
    Unknown,
    Eof,

    // keywords
    Def,
    Extern,
    If,
    Then,
    Else,
    For,
    In,

    // primary
    Identifier,
    Number,
    LParen,
    RParen,
    Comma,
    Semicolon,
    Equals,
    FirstOperator,
    LessThan = FirstOperator,
    Plus,
    Minus,
    Times,
    LastOperator = Times,
  };

  Type type;

  avian::util::String text;

  union {
    double number;
  };

  Token() : type(Unknown), text(0, 0)
  {
  }

  Token(Type type) : type(type), text(0, 0)
  {
  }

  Token(double value) : type(Number), text(0, 0), number(value)
  {
  }

  Token(avian::util::String text) : type(Identifier), text(text)
  {
  }
};

class Lexer {
 private:
  const char* ptr;

 public:
  Lexer() : ptr(0)
  {
  }

  void setInput(const char* ptr)
  {
    this->ptr = ptr;
  }

  Token getToken();
};

class CompilerContext {
 public:
  vm::System* system;
  vm::Heap* heap;
  avian::codegen::Architecture* arch;
  avian::codegen::link::Module module;
  avian::codegen::link::Jit jit;

  static const size_t ExecutableSize = 1024;

  CompilerContext();
  ~CompilerContext();
};

class FunctionContext;

class Scope {
 public:
  virtual avian::codegen::ir::Value* lookup(avian::util::String name,
                                            FunctionContext& c) = 0;
};

class PrototypeAst;

class FunctionContext {
 public:
  CompilerContext& context;
  PrototypeAst* proto;
  avian::util::Arena arena;
  avian::codegen::ir::Compilation compilation;
  avian::codegen::ir::Function& f;
  avian::codegen::ir::Block* entryBlock;
  avian::codegen::ir::Builder b;
  Scope* scope;

  FunctionContext(CompilerContext& context, PrototypeAst* proto);
  ~FunctionContext();
};

class ExprAst {
 public:
  virtual ~ExprAst()
  {
  }
  virtual avian::codegen::ir::Value* codegen(FunctionContext& c)
      = 0;

  virtual void dump() = 0;
};

void dump(ExprAst* ast);

class NumberExprAst : public ExprAst {
 public:
  double value;

  NumberExprAst(double value) : value(value)
  {
  }

  virtual avian::codegen::ir::Value* codegen(FunctionContext& c);

  virtual void dump()
  {
    printf("%g", value);
  }
};

class VariableExprAst : public ExprAst {
 public:
  avian::util::String name;

  VariableExprAst(avian::util::String name) : name(name)
  {
  }

  virtual avian::codegen::ir::Value* codegen(FunctionContext& c);

  virtual void dump()
  {
    printf("var:%*s", (int)name.length, name.text);
  }
};

class BinaryExprAst : public ExprAst {
 public:
  Token::Type op;
  ExprAst* lhs;
  ExprAst* rhs;

  BinaryExprAst(Token::Type op, ExprAst* lhs, ExprAst* rhs)
      : op(op), lhs(lhs), rhs(rhs)
  {
  }

  virtual avian::codegen::ir::Value* codegen(FunctionContext& c);

  virtual void dump()
  {
    printf("binop:%d(", op);
    lhs->dump();
    printf(", ");
    rhs->dump();
    printf(")");
  }
};

class CallExprAst : public ExprAst {
 public:
  avian::util::String callee;
  avian::util::List<ExprAst*>* args;

  CallExprAst(avian::util::String callee, avian::util::List<ExprAst*>* args)
      : callee(callee), args(args)
  {
  }

  virtual avian::codegen::ir::Value* codegen(FunctionContext& c);

  virtual void dump()
  {
    printf("call:%*s(<todo>)", (int)callee.length, callee.text);
  }
};

class IfExprAst : public ExprAst {
 public:
  ExprAst* cond;
  ExprAst* then;
  ExprAst* else_;

  IfExprAst(ExprAst* cond, ExprAst* then, ExprAst* else_)
      : cond(cond), then(then), else_(else_)
  {
  }

  virtual avian::codegen::ir::Value* codegen(FunctionContext& c);

  virtual void dump()
  {
    printf("if(cond: ");
    cond->dump();
    printf(", then: ");
    then->dump();
    printf(", else: ");
    else_->dump();
    printf(")");
  }
};

class ForExprAst : public ExprAst, public Scope {
 public:
  avian::util::String varname;
  ExprAst* start;
  ExprAst* end;
  ExprAst* step;
  ExprAst* body;
  Scope* outerScope;
  avian::codegen::ir::Value* var;

  ForExprAst(avian::util::String varname,
             ExprAst* start,
             ExprAst* end,
             ExprAst* step,
             ExprAst* body)
      : varname(varname), start(start), end(end), step(step), body(body)
  {
  }

  virtual avian::codegen::ir::Value* codegen(FunctionContext& c);

  virtual avian::codegen::ir::Value* lookup(avian::util::String name,
                                            FunctionContext& c);

  virtual void dump()
  {
    printf("for(<todo>)");
  }
};

class PrototypeAst : public Scope {
 public:
  avian::util::String name;
  avian::util::List<avian::util::String>* args;

  PrototypeAst(avian::util::String name,
               avian::util::List<avian::util::String>* args)
      : name(name), args(args)
  {
  }

  virtual avian::codegen::ir::Value* lookup(avian::util::String name,
                                            FunctionContext& c);
};

class FunctionAst {
 public:
  PrototypeAst* proto;
  ExprAst* body;

  FunctionAst(PrototypeAst* proto, ExprAst* body) : proto(proto), body(body)
  {
  }

  uint8_t* codegen(CompilerContext& context);
};

template <unsigned Min, unsigned Max, class T>
class SmallIntMap {
 private:
  T items[Max - Min];

 public:
  T& operator[](unsigned index)
  {
    ASSERT(index >= Min);
    ASSERT(index < Max);
    return items[index - Min];
  }
};

class Parser {
 public:
  Lexer lexer;
  Token curTok;
  avian::util::Arena a;
  SmallIntMap
      <Token::FirstOperator, Token::LastOperator + 1, int> binopPrecedence;

  Parser();

  void nextToken();

  int getTokenPrecedence();

  ExprAst* error(const char* message);

  PrototypeAst* errorP(const char* message);

  ExprAst* parseIdentifierExpr();

  ExprAst* parseNumberExpr();

  ExprAst* parseParenExpr();

  ExprAst* parseIfExpr();

  ExprAst* parseForExpr();

  ExprAst* parsePrimary();

  ExprAst* parseBinOpRhs(int exprPrec, ExprAst* lhs);

  ExprAst* parseExpr();

  PrototypeAst* parsePrototype();

  FunctionAst* parseDefinition();

  FunctionAst* parseTopLevelExpr();

  PrototypeAst* parseExtern();
};

class Machine {
 private:
  Parser parser;
  CompilerContext context;

  void handleDefinition();

  void handleExtern();

  avian::util::Optional<double> handleTopLevelExpr();

 public:
  avian::util::Optional<double> line(const char* line);
  void mainLoop();
};

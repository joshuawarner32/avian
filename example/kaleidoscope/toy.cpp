#include "toy.h"

#include <math.h>

using namespace avian::codegen;
using namespace avian::util;

Token Lexer::getToken()
{
  while (isspace(*ptr)) {
    ptr++;
  }

  if (isalpha(*ptr)) {
    const char* begin = ptr;

    while (isalnum((*ptr))) {
      ptr++;
    }

    String id(begin, ptr - begin);

    if (id == "def") {
      return Token(Token::Def);
    } else if (id == "extern") {
      return Token(Token::Extern);
    } else if (id == "if") {
      return Token(Token::If);
    } else if (id == "then") {
      return Token(Token::Then);
    } else if (id == "else") {
      return Token(Token::Else);
    } else if (id == "for") {
      return Token(Token::For);
    } else if (id == "in") {
      return Token(Token::In);
    } else {
      return Token(id);
    }
  }

  if (isdigit(*ptr) || *ptr == '.') {
    const char* begin = ptr;
    do {
      ptr++;
    } while (isdigit(*ptr) || *ptr == '.');

    // This is nasty, but it's what strtod wants.
    // strtod really should have been written as:
    // double strtod(const char* begin, const char* const* end);
    char* end = const_cast<char*>(ptr);

    double value = strtod(begin, &end);
    assert(end == ptr);

    return Token(value);
  }

  if (*ptr == '#') {
    do {
      ptr++;
    } while (*ptr != '\0' && *ptr != '\n' && *ptr != '\r');

    if (*ptr != '\0') {
      return getToken();
    }
  }

  char ch = *ptr;

  if (ch == '\0') {
    return Token(Token::Eof);
  } else {
    ptr++;

    switch (ch) {
    case '<':
      return Token(Token::LessThan);
    case '+':
      return Token(Token::Plus);
    case '-':
      return Token(Token::Minus);
    case '*':
      return Token(Token::Times);
    case '(':
      return Token(Token::LParen);
    case ')':
      return Token(Token::RParen);
    case '=':
      return Token(Token::Equals);
    case ',':
      return Token(Token::Comma);
    default:
      return Token(Token::Unknown);
    }
  }
}

void dump(ExprAst* ast) {
  ast->dump();
  printf("\n");
  fflush(stdout);
}

ir::Value* NumberExprAst::codegen(FunctionContext& c)
{
  return c.b.constantFloatImm(c.b.types.f8, value);
}

ir::Value* VariableExprAst::codegen(FunctionContext& c)
{
  return c.scope->lookup(name, c);
}

ir::Value* BinaryExprAst::codegen(FunctionContext& c)
{
  ir::Value* l = lhs->codegen(c);
  ir::Value* r = rhs->codegen(c);
  switch (op) {
  case Token::Plus:
    return c.b.binaryOp(lir::FloatAdd, l, r);
  case Token::Minus:
    return c.b.binaryOp(lir::FloatSubtract, l, r);
  case Token::Times:
    return c.b.binaryOp(lir::FloatMultiply, l, r);
  case Token::LessThan: {
    // TODO: this is a horrible way to do this.  We should really use some sort
    // of compare instruction, followed by a select.
    ir::Block* ldOne = c.f.addBlock();
    ldOne->setName("ltOneRes");
    ir::Block* ldZero = c.f.addBlock();
    ldZero->setName("ltZeroRes");
    ir::Block* end = c.f.addBlock();
    end->setName("ltEnd");
    c.b.branch(lir::JumpIfFloatLess, l, r, ir::Edge(ldOne), ir::Edge(ldZero));
    c.b.setBlock(ldZero);
    ir::Value* zero = c.b.constantFloatImm(c.b.types.f8, 0);
    c.b.jump(ir::Edge(end));
    c.b.setBlock(ldOne);
    ir::Value* one = c.b.constantFloatImm(c.b.types.f8, 1);
    ir::Edge::ValuePair p1(one, zero);
    ir::JumpInstr* ji = c.b.jump(ir::Edge(
        end, Slice<ir::Value*>(0, 0), Slice<ir::Edge::ValuePair>(&p1, 1)));
    c.b.setBlock(end);

    return &ji->dest.latentValues[0].merge;
  } break;
  default:
    UNREACHABLE_;
  }
}

const size_t InitialZoneCapacityInBytes = 16 * 1024;

link::Symbol& compileThunk(CompilerContext& c, String name, size_t argCount, void* target)
{
  link::Symbol& sym = c.module.addSymbol(name);

  vm::Zone zone(c.system, c.heap, InitialZoneCapacityInBytes);
  Arena arena(StdAlloc::Instance);
  ir::Compilation compilation(c.module, arena);
  Assembler* assembler = c.arch->makeAssembler(c.heap, &zone);

  ir::Types types(sizeof(void*));
  size_t tc = argCount;
  ir::Type* pt = (ir::Type*)malloc(sizeof(ir::Type) * tc);
  for (size_t i = 0; i < tc; i++) {
    pt[i] = types.f8;
  }
  ir::Function& f = compilation.addFunction(sym, Slice<ir::Type>(pt, tc));

  ir::Value** pv = (ir::Value**)malloc(sizeof(ir::Value*) * tc);
  Slice<ir::Value*> params(pv, tc);
  for (size_t i = 0; i < tc; i++) {
    pv[i] = &f.params[i];
  }

  ir::Builder b(f.addBlock());
  b.return_(b.call(b.types.f8,
                   ir::LongJumpOrCall,
                   b.constantImm(b.types.address, reinterpret_cast<uint64_t>(target)),
                   params).get());

  ir::dump(f);
  compiler::BasicOptPass optPass;
  optPass.process(f);
  ir::dump(f);
  compiler::lower(c.arch, f);
  ir::dump(f);
  compiler::regalloc(c.arch, f);
  ir::dump(f);
  compiler::emit(c.arch, assembler, f);

  return sym;
}

link::Symbol& findSymbol(CompilerContext& c, String name, size_t arity)
{
  link::Module& m = c.module;
  for (List<link::Symbol>* l = m.symbols; l; l = l->next) {
    if (l->item.name == name) {
      return l->item;
    }
  }

  if(name == "sin") {
    return compileThunk(c, name, arity, reinterpret_cast<void*>(sin));
  }

  // TODO: report an error
  UNREACHABLE_;
}

ir::Value* CallExprAst::codegen(FunctionContext& c)
{

  size_t arity = args->count();

  link::Symbol& sym = findSymbol(c.context, callee, arity);

  // TODO:
  ir::Value* callee = c.b.symbol(sym);

  ir::Value* values[arity];
  size_t i = 0;
  for (List<ExprAst*>* a = args; a; a = a->next, i++) {
    values[i] = a->item->codegen(c);
  }

  return c.b.call(c.b.types.f8, 0, callee, Slice<ir::Value*>(values, arity))
      .get();
}

ir::Value* IfExprAst::codegen(FunctionContext& c)
{
  ir::Block* ifTrue = c.f.addBlock();
  ifTrue->setName("ifThen");
  ir::Block* ifFalse = c.f.addBlock();
  ifFalse->setName("ifElse");
  ir::Block* end = c.f.addBlock();
  end->setName("ifEnd");
  c.b.branch(lir::JumpIfFloatEqual,
             cond->codegen(c),
             c.b.constantFloatImm(c.b.types.f8, 1),
             ir::Edge(ifTrue),
             ir::Edge(ifFalse));
  c.b.setBlock(ifFalse);
  ir::Value* resFalse = else_->codegen(c);
  c.b.jump(ir::Edge(end));
  c.b.setBlock(ifTrue);
  ir::Value* resTrue = then->codegen(c);
  ir::Edge::ValuePair p1(resTrue, resFalse);
  ir::JumpInstr* ji = c.b.jump(ir::Edge(
      end, Slice<ir::Value*>(0, 0), Slice<ir::Edge::ValuePair>(&p1, 1)));
  c.b.setBlock(end);

  return &ji->dest.latentValues[0].merge;
}

ir::Value* ForExprAst::codegen(FunctionContext& c)
{
  ir::Value* stepValue = step ? step->codegen(c)
                              : c.b.constantFloatImm(c.b.types.f8, 1.0);

  var = c.b.alloca(c.b.types.f8);

  ir::Value* iter = start->codegen(c);
  c.b.store(var, iter);

  outerScope = c.scope;
  c.scope = this;

  ir::Block* bodyBlock = c.f.addBlock();
  bodyBlock->setName("forBody");
  ir::Block* condBlock = c.f.addBlock();
  condBlock->setName("forCond");
  ir::Block* endBlock = c.f.addBlock();
  endBlock->setName("forEnd");

  c.b.jump(condBlock);
  c.b.setBlock(condBlock);

  ir::Value* condValue = end->codegen(c);
  c.b.branch(lir::JumpIfFloatEqual,
             c.b.constantFloatImm(c.b.types.f8, 1.0),
             condValue,
             bodyBlock,
             endBlock);
  c.b.setBlock(bodyBlock);

  body->codegen(c);

  iter = c.b.load(c.b.types.f8, var);
  c.b.store(var,
            c.b.binaryOp(
                lir::FloatAdd, iter, stepValue));
  c.b.jump(condBlock);

  c.b.setBlock(endBlock);

  return c.b.constantFloatImm(c.b.types.f8, 0.0);
}

ir::Value* ForExprAst::lookup(String name, FunctionContext& c)
{
  if(name == varname) {
    return c.b.load(c.b.types.f8, var);
  } else {
    return outerScope->lookup(name, c);
  }
}

CompilerContext::CompilerContext()
    : system(vm::makeSystem()),
      heap(vm::makeHeap(system, 16 * 1024 * 1024)),
      arch(makeArchitectureNative(system, true)),
      module(StdAlloc::Instance, ExecutableSize),
      jit(module)
{
}

CompilerContext::~CompilerContext()
{
  arch->release();

  // TODO:  fix the memory leak, and reenable this code:
  // heap->dispose();

  system->dispose();
}

static ir::Function& beginFunction(CompilerContext& context,
                                   ir::Compilation& compilation,
                                   PrototypeAst* proto)
{
  link::Symbol& sym = context.module.addSymbol(proto->name);
  ir::Types types(sizeof(void*));
  size_t tc = proto->args->count();
  ir::Type* pt = (ir::Type*)malloc(sizeof(ir::Type) * tc);
  for (size_t i = 0; i < tc; i++) {
    pt[i] = types.f8;
  }
  ir::Function& f = compilation.addFunction(sym, Slice<ir::Type>(pt, tc));
  free(pt);
  return f;
}

FunctionContext::FunctionContext(CompilerContext& context, PrototypeAst* proto)
    : context(context),
      proto(proto),
      arena(StdAlloc::Instance),
      compilation(context.module, arena),
      f(beginFunction(context, compilation, proto)),
      entryBlock(f.addBlock()),
      b(entryBlock),
      scope(proto)
{
  entryBlock->setName("entry");
}

FunctionContext::~FunctionContext()
{
}

ir::Value* PrototypeAst::lookup(String name, FunctionContext& c)
{
  size_t i = 0;
  for (List<String>* l = args; l; l = l->next, i++) {
    if (l->item == name) {
      return c.b.param(i);
    }
  }
  // TODO: report an error
  UNREACHABLE_;
}

uint8_t* FunctionAst::codegen(CompilerContext& context)
{
  vm::Zone zone(context.system, context.heap, InitialZoneCapacityInBytes);
  Assembler* assembler = context.arch->makeAssembler(context.heap, &zone);

  FunctionContext fc(context, proto);

  ir::Value* ret = body->codegen(fc);
  fc.b.return_(ret);

  ir::dump(fc.f);
  compiler::BasicOptPass optPass;
  optPass.process(fc.f);
  ir::dump(fc.f);
  compiler::lower(context.arch, fc.f);
  ir::dump(fc.f);
  compiler::regalloc(context.arch, fc.f);
  ir::dump(fc.f);

  compiler::emit(context.arch, assembler, fc.f);

  uint8_t* code = reinterpret_cast
      <uint8_t*>(fc.f.symbol.getLocation().get().baseOffset
                 + fc.compilation.module.baseAddress.get());
  size_t size = fc.f.symbol.getLocation().get().size;

  fprintf(stderr, "code: '-s %p -e %p'\n", code, code + size);

  return code;
}

Parser::Parser() : a(StdAlloc::Instance)
{
  binopPrecedence[Token::LessThan] = 10;
  binopPrecedence[Token::Plus] = 20;
  binopPrecedence[Token::Minus] = 20;
  binopPrecedence[Token::Times] = 40;
}

void Parser::nextToken()
{
  curTok = lexer.getToken();
}

int Parser::getTokenPrecedence()
{
  if (curTok.type < Token::FirstOperator || curTok.type > Token::LastOperator) {
    return -1;
  } else {
    return binopPrecedence[curTok.type];
  }
}

ExprAst* Parser::error(const char* message)
{
  fprintf(stderr, "Error: %s\n", message);
  return 0;
}

PrototypeAst* Parser::errorP(const char* message)
{
  fprintf(stderr, "Error: %s\n", message);
  return 0;
}

ExprAst* Parser::parseIdentifierExpr()
{
  String name = curTok.text;
  nextToken();

  if (curTok.type != Token::LParen) {
    return new (a) VariableExprAst(name);
  } else {
    nextToken();
    ListBuilder<ExprAst*> args(a);
    if (curTok.type != Token::RParen) {
      while (true) {
        ExprAst* arg = parseExpr();
        if (!arg) {
          return 0;
        }
        args.append(arg);
        if (curTok.type == Token::RParen) {
          break;
        }
        if (curTok.type != Token::Comma) {
          return error("Expected ')' or ',' in argument list");
        }
        nextToken();
      }
    }
    nextToken();
    return new (a) CallExprAst(name, args.list);
  }
}

ExprAst* Parser::parseNumberExpr()
{
  ExprAst* v = new (a) NumberExprAst(curTok.number);
  nextToken();
  return v;
}

ExprAst* Parser::parseParenExpr()
{
  nextToken();

  ExprAst* v = parseExpr();

  if (!v) {
    return 0;
  }

  if (curTok.type != Token::RParen) {
    return error("expected ')'");
  }
  nextToken();
  return v;
}

ExprAst* Parser::parseIfExpr()
{
  nextToken();

  ExprAst* cond = parseExpr();
  if (!cond) {
    return 0;
  }

  if (curTok.type != Token::Then) {
    return error("expected then");
  }
  nextToken();

  ExprAst* then = parseExpr();
  if (then == 0) {
    return 0;
  }

  if (curTok.type != Token::Else) {
    return error("expected else");
  }

  nextToken();

  ExprAst* else_ = parseExpr();
  if (!else_) {
    return 0;
  }

  return new IfExprAst(cond, then, else_);
}

ExprAst* Parser::parseForExpr()
{
  nextToken();

  if (curTok.type != Token::Identifier) {
    return error("expected identifier after for");
  }

  String varname = curTok.text;
  nextToken();

  if (curTok.type != Token::Equals) {
    return error("expected '=' after 'for <identifier>'");
  }
  nextToken();

  ExprAst* start = parseExpr();
  if (start == 0) {
    return 0;
  }
  if (curTok.type != Token::Comma) {
    return error("expected ',' after start value");
  }

  nextToken();

  ExprAst* end = parseExpr();
  if (end == 0) {
    return 0;
  }

  ExprAst* step = 0;
  if (curTok.type == Token::Comma) {
    nextToken();
    step = parseExpr();
    if (step == 0) {
      return 0;
    }
  }

  if (curTok.type != Token::In) {
    return error("expected 'in' after for");
  }
  nextToken();

  ExprAst* body = parseExpr();
  if (body == 0) {
    return 0;
  }

  return new ForExprAst(varname, start, end, step, body);
}

ExprAst* Parser::parsePrimary()
{
  switch (curTok.type) {
  case Token::Identifier:
    return parseIdentifierExpr();
  case Token::Number:
    return parseNumberExpr();
  case Token::LParen:
    return parseParenExpr();
  case Token::If:
    return parseIfExpr();
  case Token::For:
    return parseForExpr();
  default:
    return error("unexpected token");
  }
}

ExprAst* Parser::parseBinOpRhs(int exprPrec, ExprAst* lhs)
{
  while (true) {
    int tokenPrec = getTokenPrecedence();

    if (tokenPrec < exprPrec) {
      return lhs;
    }

    Token::Type binOp = curTok.type;
    nextToken();

    ExprAst* rhs = parsePrimary();
    if (!rhs) {
      return 0;
    }

    int nextPrec = getTokenPrecedence();
    if (tokenPrec < nextPrec) {
      rhs = parseBinOpRhs(tokenPrec + 1, rhs);
      if (!rhs) {
        return 0;
      }
    }

    lhs = new (a) BinaryExprAst(binOp, lhs, rhs);
  }
}

ExprAst* Parser::parseExpr()
{
  ExprAst* lhs = parsePrimary();
  if (!lhs) {
    return 0;
  }
  return parseBinOpRhs(0, lhs);
}

PrototypeAst* Parser::parsePrototype()
{
  if (curTok.type != Token::Identifier) {
    return errorP("expected function name in prototype");
  }

  String fnName = curTok.text;
  nextToken();

  if (curTok.type != Token::LParen) {
    return errorP("expected '(' in prototype");
  }

  ListBuilder<String> args(a);
  while (true) {
    nextToken();
    if (curTok.type != Token::Identifier) {
      break;
    }
    args.append(curTok.text);
  }

  if (curTok.type != Token::RParen) {
    return errorP("expected ')' in prototype");
  }

  nextToken();

  return new (a) PrototypeAst(fnName, args.list);
}

FunctionAst* Parser::parseDefinition()
{
  nextToken();

  PrototypeAst* proto = parsePrototype();
  if (!proto) {
    return 0;
  }

  if (ExprAst* body = parseExpr()) {
    return new (a) FunctionAst(proto, body);
  } else {
    return 0;
  }
}

FunctionAst* Parser::parseTopLevelExpr()
{
  if (ExprAst* v = parseExpr()) {
    PrototypeAst* proto = new (a) PrototypeAst(String("toplevelexpr"), 0);
    return new (a) FunctionAst(proto, v);
  } else {
    return 0;
  }
}

PrototypeAst* Parser::parseExtern()
{
  nextToken();
  return parsePrototype();
}

void Machine::handleDefinition()
{
  if (FunctionAst* f = parser.parseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");

    f->codegen(context);
  } else {
    parser.nextToken();
  }
}

void Machine::handleExtern()
{
  if (parser.parseExtern()) {
    fprintf(stderr, "Parsed an extern.\n");
  } else {
    parser.nextToken();
  }
}

Optional<double> Machine::handleTopLevelExpr()
{
  if (FunctionAst* f = parser.parseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr.\n");

    uint8_t* code = f->codegen(context);
    double result = ((double (*)())code)();
    printf("%g\n", result);
    return result;
  } else {
    parser.nextToken();
    return Optional<double>();
  }
}

Optional<double> Machine::line(const char* line)
{
  parser.lexer.setInput(line);
  parser.nextToken();

  switch (parser.curTok.type) {
  case Token::Eof:
    break;
  case Token::Semicolon:
    parser.nextToken();
    break;
  case Token::Def:
    handleDefinition();
    break;
  case Token::Extern:
    handleExtern();
    break;
  default:
    return handleTopLevelExpr();
  }
  return Optional<double>();
}

void Machine::mainLoop()
{
  size_t size = 100;
  char* l = (char*)malloc(size);

  while (true) {
    fprintf(stderr, "ready> ");

    if (getline(&l, &size, stdin) != -1) {
      line(l);
    } else {
      return;
    }
  }
}

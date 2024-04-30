// parser.hpp : header file for the parser
#pragma once

#include <map>
#include "codeGenerator.hpp"
#include "AST.hpp"
#include "KaleidoscopeJIT.h"
#include "llvm.h"

// Namespaces
using namespace llvm;
using namespace llvm::orc;
using namespace std;

// External variables
extern int CurTok;
extern std::map<char, int> BinopPrecedence;

//function prototypes
int getNextToken();
extern std::unique_ptr<AST::ExprAST> LogError(const char *Str);
extern std::unique_ptr<AST::PrototypeAST> ParsePrototype();
extern std::unique_ptr<AST::FunctionAST> ParseDefinition();
extern std::unique_ptr<AST::FunctionAST> ParseTopLevelExpr();
extern std::unique_ptr<AST::PrototypeAST> ParseExtern();
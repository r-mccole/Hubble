// codeGeneration.hpp : header file for the code generator
#pragma once

#include "AST.hpp"
#include "KaleidoscopeJIT.h"
#include "llvm.h"

// Namespaces
using namespace llvm;
using namespace llvm::orc;
using namespace std;

// Function prototypes
extern std::unique_ptr<LLVMContext> TheContext;
extern std::unique_ptr<Module> TheModule;
extern std::unique_ptr<IRBuilder<>> Builder;
// std::map<std::string, Value *> NamedValues;
extern std::unique_ptr<KaleidoscopeJIT> TheJIT;
extern std::unique_ptr<FunctionPassManager> TheFPM;
extern std::unique_ptr<LoopAnalysisManager> TheLAM;
extern std::unique_ptr<FunctionAnalysisManager> TheFAM;
extern std::unique_ptr<CGSCCAnalysisManager> TheCGAM;
extern std::unique_ptr<ModuleAnalysisManager> TheMAM;
extern std::unique_ptr<PassInstrumentationCallbacks> ThePIC;
extern std::unique_ptr<StandardInstrumentations> TheSI;
extern std::map<std::string, std::unique_ptr<AST::PrototypeAST>> FunctionProtos;
extern ExitOnError ExitOnErr;
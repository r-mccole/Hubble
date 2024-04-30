#undef _DEBUG //required for debugging with cmake

#include "../inc/AST.hpp"
#include "../inc/codeGenerator.hpp"
#include "../inc/common.h"
#include "../inc/KaleidoscopeJIT.h"
#include "../inc/lexer.hpp"
#include "../inc/llvm.h"
#include "../inc/parser.hpp"
#include "../inc/topParser.hpp"

using namespace llvm;
using namespace llvm::orc;
using namespace std;

/// top ::= definition | external | expression | ';'
static void MainLoop() {
  while (true) {
    fprintf(stderr, "> ");
    switch (CurTok) {
    case tok_eof:
      return;
    case ';': // ignore top-level semicolons.
      getNextToken();
      break;
    case tok_def:
      HandleDefinition();
      break;
    case tok_extern:
      HandleExtern();
      break;
    case tok_help:
      HandleHelp();
      break;
    default:
      HandleTopLevelExpression();
      break;
    }
  }
}

//===----------------------------------------------------------------------===//
// "Library" functions that can be "extern'd" from user code.
//===----------------------------------------------------------------------===//

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

/// putchard - putchar that takes a double and returns 0.
extern "C" DLLEXPORT double putchard(double X) {
  fputc((char)X, stderr);
  return 0;
}

/// printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" DLLEXPORT double printd(double X) {
  fprintf(stderr, "%f\n", X);
  return 0;
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main() {
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();
  InitializeNativeTargetAsmParser();

  // Install standard binary operators.
  // 1 is lowest precedence.
  BinopPrecedence['<'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40; // highest.

  //header
  printf("+----------------------------------------------------+\n");
  printf("|  __  __  __  __  ______  ______  __      ______    |\n");
  printf("| /\\ \\_\\ \\/\\ \\/\\ \\/\\  == \\/\\  == \\/\\ \\    /\\  ___\\   |\n");
  printf("| \\ \\  __ \\ \\ \\_\\ \\ \\  __<\\ \\  __<\\ \\ \\___\\ \\  __\\   |\n");
  printf("|  \\ \\_\\ \\_\\ \\_____\\ \\_____\\ \\_____\\ \\_____\\ \\_____\\ |\n");
  printf("|   \\/_/\\/_/\\/_____/\\/_____/\\/_____/\\/_____/\\/_____/ |\n");
  printf("|                                                    |\n");
  printf("|                                                    |\n");
  printf("| ECE 501 Master's Project                           |\n");
  printf("| By: Riordan McCole                                 |\n");
  printf("|                                                    |\n");
  printf("| Type 'help' for help                               |\n");
  printf("+----------------------------------------------------+\n\n");


  // Prime the first token.
  fprintf(stderr, "> ");
  getNextToken();

  TheJIT = ExitOnErr(KaleidoscopeJIT::Create());

  InitializeModuleAndManagers();

  // Run the main "interpreter loop" now.
  MainLoop();

  // Print out ModuleID, source_filename, and target datalayout
  TheModule->print(errs(), nullptr);

  return 0;
}
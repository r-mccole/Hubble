#undef _DEBUG //required for debugging with cmake

#include "../inc/codeGenerator.hpp"
#include "../inc/common.h"
#include "../inc/parser.hpp"
//#include "../inc/topParser.hpp"

using namespace llvm;
using namespace llvm::orc;
using namespace std;

//===----------------------------------------------------------------------===//
// Top-Level parsing and JIT Driver
//===----------------------------------------------------------------------===//

void InitializeModuleAndManagers() {
  // Open a new context and module.
  TheContext = std::make_unique<LLVMContext>();
  TheModule = std::make_unique<Module>("KaleidoscopeJIT", *TheContext);
  TheModule->setDataLayout(TheJIT->getDataLayout());

  // Create a new builder for the module.
  Builder = std::make_unique<IRBuilder<>>(*TheContext);

  // Create new pass and analysis managers.
  TheFPM = std::make_unique<FunctionPassManager>();
  TheLAM = std::make_unique<LoopAnalysisManager>();
  TheFAM = std::make_unique<FunctionAnalysisManager>();
  TheCGAM = std::make_unique<CGSCCAnalysisManager>();
  TheMAM = std::make_unique<ModuleAnalysisManager>();
  ThePIC = std::make_unique<PassInstrumentationCallbacks>();
  TheSI = std::make_unique<StandardInstrumentations>(*TheContext,
                                                     /*DebugLogging*/ true);
  TheSI->registerCallbacks(*ThePIC, TheMAM.get());

  // Add transform passes.
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  TheFPM->addPass(InstCombinePass());
  // Reassociate expressions.
  TheFPM->addPass(ReassociatePass());
  // Eliminate Common SubExpressions.
  TheFPM->addPass(GVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  TheFPM->addPass(SimplifyCFGPass());

  // Register analysis passes used in these transform passes.
  PassBuilder PB;
  PB.registerModuleAnalyses(*TheMAM);
  PB.registerFunctionAnalyses(*TheFAM);
  PB.crossRegisterProxies(*TheLAM, *TheFAM, *TheCGAM, *TheMAM);
}

void HandleDefinition() {
  if (auto FnAST = ParseDefinition()) {
    if (auto *FnIR = FnAST->codegen()) {
      // Shows the LLVM intermediate representation (IR)
      fprintf(stderr, "Read function definition:");
      FnIR->print(errs());
      fprintf(stderr, "\n");
      ExitOnErr(TheJIT->addModule(
          ThreadSafeModule(std::move(TheModule), std::move(TheContext))));
      InitializeModuleAndManagers();
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

void HandleExtern() {
  if (auto ProtoAST = ParseExtern()) {
    if (auto *FnIR = ProtoAST->codegen()) {
      // Shows the LLVM intermediate representation (IR)
      fprintf(stderr, "Read extern: ");
      FnIR->print(errs());
      fprintf(stderr, "\n");
      FunctionProtos[ProtoAST->getName()] = std::move(ProtoAST);
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (auto FnAST = ParseTopLevelExpr()) {
    if (auto *FnIR = FnAST->codegen()) {
      // Shows the LLVM intermediate representation (IR)
      fprintf(stderr, "Read top-level expression:");
      FnIR->print(errs());
      //fprintf(stderr, "\n");
      
      // Create a ResourceTracker to track JIT'd memory allocated to our
      // anonymous expression -- that way we can free it after executing.
      auto RT = TheJIT->getMainJITDylib().createResourceTracker();

      auto TSM = ThreadSafeModule(std::move(TheModule), std::move(TheContext));
      ExitOnErr(TheJIT->addModule(std::move(TSM), RT));
      InitializeModuleAndManagers();

      // Search the JIT for the __anon_expr symbol.
      auto ExprSymbol = ExitOnErr(TheJIT->lookup("__anon_expr"));

      // Get the symbol's address and cast it to the right type (takes no
      // arguments, returns a double) so we can call it as a native function.
      double (*FP)() = ExprSymbol.getAddress().toPtr<double (*)()>();
      fprintf(stderr, "\nEvaluated to %f\n", FP());

      // Delete the anonymous expression module from the JIT.
      ExitOnErr(RT->remove());
    }
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

void HandleHelp()
{
    std::string command = "";
    printf("\n\nShort summaries of various commands:\n"
        //"CFI\tControl Flow Integrity Flag, enables or disables CFI during compilation\n"
        "def \t\tUsed to create a function definition\n"
        "extern \t\tUsed to call an external function  from the LLVM libraries\n"
        "<equations> \tCan input mathmatical equations using <, +, -, *\n"
        "if-then-else \tCreate if, then, else statements to path code execution\n"
        "for \t\tUse for loops to iterate over a sequence\n"
        "ctrl \t\tUsed to exit the program gracefully and show the ModuleID\n\n"
        "For more information on a specific command, type the above command-name\n"
        "If done type exit;\n\n");

    // Displays various command information 
    while (true)
    {
        std::cout << "help> ";
        std::cin >> command;
        std::transform(command.begin(), command.end(), command.begin(), ::tolower); //make lowercase
        
        if (command == "def")
        {
            printf("\nFunctions definitions are used to perform certain actions, and they\n"
                   "are useful for reusing code: Define the code once, and use it many times.\n");
            printf("Syntax: 'def foo(a b) a+b;'\n\n");
        }
        else if (command == "extern")
        {
            printf("\nLeverages the intermediate representation (IR) from LLVM to call\n"
                   "various established features. This IR that serves as a portable,\n" 
                   "high-level assembly language that can be optimized with a variety\n"
                   "of transformations over multiple passes.\n");
            printf("Syntax: 'extern cos(x);'\n\n");

        }
        else if (command == "<equations>")
        {
            printf("\nCan input mathmatical equations using a few standard operands such as\n"
                   "addition '+', subtraction '-', multiplication '*', and bool to double '<'\n");
            printf("Syntax: '3+4;'\n\n");
        }
        else if (command == "if-then-else")
        {
            printf("\nif-then-else statement provides a secondary path of execution when \n"
                   "an 'if' clause evaluates to false\n");
            printf("Syntax: 'if 1 then 2 else 3;'\n\n");
        }
        else if (command == "for")
        {
            printf("\nfor loops are used when you have a block of code which you want to \n"
                   "repeat a fixed number of times\n");
            printf("Syntax: 'for i = 0, i < ###, 1.0 in function_name(i)'\n\n");
        }
        else if (command == "ctrl")
        {
            printf("\nQuit the program gracefully (by sending an EOF via CTRL+D on Linux or"
                   "CTRL+Z\n and ENTER on Windows), it will show the ModuleID,"
                   "source_filename, and target datalayout.\n\n");
            printf("Syntax: '^Z;'\n\n");
        }
        else if (command == "exit")
        {
            //getNextToken(); // does not parse well with cin, records linefeed (10)
            CurTok = 59; // force ; to return parser back to request new input
            break;
        }
        else
        {
            printf("\nPlease type a command or 'exit'\n");
        }
    }
}
// topParser.hpp : header file for the top-level parser
#pragma once

#include "codeGenerator.hpp"
#include "KaleidoscopeJIT.h"
#include "llvm.h"

//Namespaces
using namespace llvm;

// function prototypes
void InitializeModuleAndManagers();
void HandleDefinition();
void HandleExtern();
void HandleHelp();
void HandleTopLevelExpression();
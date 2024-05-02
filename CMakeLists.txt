cmake_minimum_required(VERSION 3.8)

project(
    Hubble
    VERSION 0.1
    DESCRIPTION "Hubble, a simple LLVM supported custom compiler/language")


# Set the variable SOURCES to be the list of source files
set(SOURCES
"src/codeGenerator.cpp"
"src/lexer.cpp" 
"src/main.cpp"
"src/parser.cpp"
"src/topParser.cpp"
) 

SET(LLVM_DIR "C:/repos/clang+llvm-18.1.4-x86_64-pc-windows-msvc/lib/cmake/llvm")
set(CMAKE_BUILD_TYPE Debug)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set project compile flags.
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Build an executable 'PROJECT_NAME' from the sources
add_executable(${PROJECT_NAME} ${SOURCES})

# Add the folder 'inc' as a search path for the compiler
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc)

# Find the libraries that correspond to the LLVM components needed
#Common options: llvm_libs, Analysis, ExecutionEngine, InstCombine, Object, OrcJIT, Passes, RuntimeDyld, Core, Support, native
llvm_map_components_to_libnames(llvm_libs OrcJIT native) # required libraries

# Link against LLVM libraries
target_link_libraries(${PROJECT_NAME} ${llvm_libs})

# minimum required version is std 17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17) 
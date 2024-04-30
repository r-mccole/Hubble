# Hubble
Hubble is a procedural language which operates within a self contained terminal session. Similar to an interactive compiler much like one would find when running a Python session, or any other interpretation language that allows code to be processes, compiled, and run in real-time. Hubble is formally a fork of part of LLVM's popular 'Kaleidoscope' language. Focused on a larger scoped area, much like the 'Hubble' telescope. This project aims to create a strong foundational base in learning how to create a language using LLVM, and building projects with CMake.

![image](https://github.com/r-mccole/Hubble/assets/18635735/14728ece-f66d-491c-a02e-54aaf6c39725)



# Setup and Use
If you are just interested in testing out and running Hubble on Windows, you're in luck! Just simply download the latest precompiled executable from here: https://github.com/r-mccole/Hubble/releases/latest

# Build from Source
Running on a different operating system then windows? You will have to build this project from source, this is untested, but hopefully these steps will provide a good starting place. 

## Build Steps:
1. Install prerequisite programs:
    * Visual Studio Code is the recommended environment to edit, build, and debug this project in as it makes interfacing with CMake and debugging much easier. Download and install it from here:\
      https://code.visualstudio.com/download
    * This guide primarily focuses on using the Microsoft Visual C++ (MSVC) compiler, and it is the recommended compiler to use. Follow this tutorial to get the MSVC toolset, or the respective C++ compiler for your system:\
      https://code.visualstudio.com/docs/languages/cpp
    * CMake is an open source, cross-platform family of tools designed to build, test, and package software. It is the primary way that Hubble is packaged. Download and install CMake for your respective operating system:\
      https://cmake.org/download/

3. Clone the Hubble repository to a desired known directory:\
https://github.com/r-mccole/Hubble.git \
  Or download the .zip file of the repository: \
  https://github.com/r-mccole/Hubble/archive/refs/heads/main.zip

3. Download LLVM libraries: To build this project you need to download the required dependencies of LLVM. Luckily, LLVM provides precompiled libraries with the required headers used for this project. For windows use the version described as: 'clang+llvm-<version>-x86_64-pc-windows-msvc.tar.xz' Note, this version for windows/MSVC is different, but necessary to use, then the normal LLVM-<version>-win64.exe binary. This is because it includes the required headers and llvm-config.cmake build file. For Linux: 'clang+llvm-<version>-<architecture>-linux-<optional_version>.tar.xz'. They can be downloaded from the llvm-project release page:\
https://github.com/llvm/llvm-project/releases

4. Extract the files using a program that supports .tar.xz (7-zip is a good program for this). Then within the cloned /Hubble directory in the CMakeLists.txt file set the LLVM_DIR variable to the location of the 'llvm-config.cmake' typically under:
   ```
   <extracted_dir>/clang+llvm-<version>-x86_64-pc-windows-msvc/lib/cmake/llvm
   ```
5. Open the folder using VS Code, ensure that CMake is set to Debug, select and choose the [Visual Studio Build Tools 2022 Release -amd64] kit, hit build, wait, then the launch button. 
![image](https://github.com/r-mccole/Hubble/assets/18635735/b5772bcf-04b2-4e71-8272-3de2e9fe9b76)

6. You should now have a build and running version of Hubble! Test out the functionality using some of the scripts included in the /scripts folder. Copy and paste them in full directly into Hubble input area. 

# Tips and Tricks:
* If you would like to use VS Code's powerful and useful CMake Debugger install https://marketplace.visualstudio.com/items?itemName=twxs.cmake. Then build Hubble in VS Code as described in the 'Build Steps' section. To properly use the CMake debugger and interact with Hubble, you must apply either of the following cmake.debugConfig to the Workspace settings.json, typically generated by VS code. If this is not generated you can force this within VS Code by ctrl+shift+p, then >open Open Workspace Settings (JSON).

  Here is an example of where to put debug the setting:
	```
	{
	   "files.associations": {
		   "<example>": "cpp",
		   ... 
		   "<example n>": "cpp", },
 
		   "cmake.debugConfig": { "console": "integratedTerminal"}, // uses VS codes integrated terminal 
		   //"cmake.debugConfig": { "console": "externalTerminal" }, // uses the OS default detached terminal 
	}
	```

* If you get a linker error using the MSVC like below:\
  LINK : fatal error LNK1104: cannot open file 'C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\DIA SDK\lib\amd64\diaguids.lib' 

  For some reason the compiler assumes the location of diaguids.lib to be in the \Visual Studio\2019\Professional directory struture. It possible either the LLVM libraries or default MSCV libraries were compiled with the Professional Edition. In some cases the 'Community' version of Visual Studio is installed vs the 'Professional' in that case create the directory structure as it shows and place the diaguids.lib there. 


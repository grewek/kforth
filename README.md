# kforth

Warning the interpreter is under development and is in a very fragile state. It might crash or
hang on usage. 

It currently only supports arithmetic expressions, functions and if statements. 


# Usage
Run a kforth program.

`kforth <filepath>`


# Build

## Linux
1. Make sure you have either gcc or clang installed.
2. On the commandline type the following command.

`make kforth`

3. You find the executable compiler inside the bin folder of the project.
4. If you want you can try to execute the examples inside the examples folder.

## Windows
1. Open the supplied solution file in Visual Studio 2019
2. Just build the project and find the executable file inside the Debug or Release folders. 
# kforth

Under heavy development, currently only supports arithmetic expression.
# Usage
Run a kforth program.

`kforth <filepath>`


# TODO:

A high level overview of what needs to be done

* The parser has some repetition that needs to be cleaned up !
* I lost string support in the new vm so i need add it again.
* I still need to handle variables, memory and while loops.
* The parser needs more and better error handling.
* The replaceTable structure should be moved out of the parser, instead
we should add some kind of "linking" phase that resolves all symbols.
* We need to free memory valgrind is a bit concerned about my lighthearted memory usage :)


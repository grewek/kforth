# kforth

Under heavy development, currently only supports arithmetic expression.
# Usage
Run a kforth program.

`kforth <filepath>`


# TODO:

A high level overview of what needs to be done

* Cleanup the Codebase, currently it's really really messy and this makes it hard to add further functionality.
* Cleanup memory layout and usage.
* Currently we pass a constant size for a forth function but we shouldn't the lexer should tell us how big the
  function is and then we should reserve memory for that size
* Add a function to the hashmap that can add a new empty function under a given key and
return a pointer to it so the user can fill it up with instructions.
* Makefile refactoring, not really my cup of tea but it needs to be done...
* Error Handling, we really need a way to react to all kind of errors from the lexer, parser, evaluator buuut it's C and in C land error
handling is a real pain...
* Implement more forth stuff we still need branches and loops and probably more intrinsics
* I need to handle strings sooner or later.
* Write some real examples to test if my interpreter is turing complete...
* Parsing needs to be done in a different way i think, it works currently but it could very well blow up the stack when there are to many
functions calls inside each other.
* The lexer needs to learn error handling so we can tell the user where his/her/they program failed.
* The same is true for the parser and evaluator
* Add a backend for x86_64 compilations ?

: foo 1 1 + ;
: bar 2 2 - ;
: baz 3 3 * ;
(This is the main entry point of the application)
: main foo . bar . baz . ;

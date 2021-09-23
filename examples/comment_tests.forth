(This is a multiline comment and should be ignored
by my interpreter) 1 1 + . (The expression in the middle should be evauluated)
(2 2 + . This will not be evaluated)
(: bar 42 * ; This should also be ignored)
: foo 32 * ; (This is a valid one)
2 foo .
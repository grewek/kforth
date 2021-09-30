: greaterFive dup 5 > if . ."is greater than five" else . ."is not greater than five" then ;
: greaterFour dup 4 > if . ."is greater than four" then ;
6 greaterFive .
3 greaterFive . 
1 1 + greaterFive .
5 greaterFour .
: plusFour 4 + ;
2 plusFour greaterFive .
(If statement examples taken from:
https://www.forth.com/starting-forth/4-conditional-if-then-statements/)
: full  12 = if  ."It's full"  then ;
: toohot 220 > if ."Danger -- reduce heat" then ;
: day  32 < if  ."Looks good" else  ."no way" then ;


12 full .
130 toohot
224 toohot .
10 day .
32 day .
17 eggsize .

--------------------------------------------------------------------------
Error-0: Missing declaration for symbol 'b' at Line 5:9 to Line 5:10.
      5:              a = b;
                          ^ 
--------------------------------------------------------------------------
Error-1: Variable assignment for 'a' at Line 5:5 to Line 5:11, has expression of unknown type at Line 5:9 to Line 5:10 due to previous error(s).
      5:              a = b;
                      ^^^^^^
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION a int)
    )
    (STATEMENTS
        (ASSIGN int a 3)
        (ASSIGN ANY_TYPE a b)
    )
)
Failed to compile

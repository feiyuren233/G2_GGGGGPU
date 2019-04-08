
--------------------------------------------------------------------------
Error-0: Operands in binary expression at Line 3:9 to Line 3:15 have non-compatible type.
      3:              a = a<<1.2;
                          ^^^^^^ 

Info: Expecting operands on both sides of operator '<<' to have same type, but they are 'int' and 'float'.
--------------------------------------------------------------------------
Error-1: Variable assignment for 'a' at Line 3:5 to Line 3:16, has expression of unknown type at Line 3:9 to Line 3:15 due to previous error(s).
      3:              a = a<<1.2;
                      ^^^^^^^^^^^
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION a int 20)
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE a (BINARY ANY_TYPE << a 1.200000))
    )
)
Failed to compile

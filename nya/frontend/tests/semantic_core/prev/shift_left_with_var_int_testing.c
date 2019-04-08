
--------------------------------------------------------------------------
Error-0: Operands in binary expression at Line 4:9 to Line 4:13 have non-compatible type.
      4:              a = a<<c;
                          ^^^^ 

Info: Expression at rhs of operator '<<' at Line 4:12 to Line 4:13 needs to be of constant int type:
      4:              a = a<<c;
                             ~ 
--------------------------------------------------------------------------
Error-1: Variable assignment for 'a' at Line 4:5 to Line 4:14, has expression of unknown type at Line 4:9 to Line 4:13 due to previous error(s).
      4:              a = a<<c;
                      ^^^^^^^^^
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION a int 20)
        (DECLARATION c int 10)
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE a (BINARY ANY_TYPE << a c))
    )
)
Failed to compile

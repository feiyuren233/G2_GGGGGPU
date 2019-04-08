
--------------------------------------------------------------------------
Error-0: Missing declaration for symbol 'a' at Line 6:5 to Line 6:6.
      6:              a = 2 + 2.0;
                      ^           
--------------------------------------------------------------------------
Error-1: Operands in binary expression at Line 6:9 to Line 6:16 have non-compatible type.
      6:              a = 2 + 2.0;
                          ^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same type, but they are 'int' and 'float'.
--------------------------------------------------------------------------
Error-2: Variable assignment for 'a' at Line 6:5 to Line 6:17, has unknown type on both sides of assignment operator due to pervious errors.
      6:              a = 2 + 2.0;
                      ^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-3: Missing declaration for symbol 'a' at Line 7:5 to Line 7:6.
      7:              a = 2;
                      ^     
--------------------------------------------------------------------------
Error-4: Variable assignment for 'a' at Line 7:5 to Line 7:11, has variable of unknown type at Line 7:5 to Line 7:6 due to previous error.
      7:              a = 2;
                      ^^^^^^
--------------------------------------------------------------------------
Error-5: Operands in binary expression at Line 8:12 to Line 8:19 have non-compatible type.
      8:              inta = 2 + 2.0;
                             ^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same type, but they are 'int' and 'float'.
--------------------------------------------------------------------------
Error-6: Variable assignment for 'inta' at Line 8:5 to Line 8:20, has expression of unknown type at Line 8:12 to Line 8:19 due to previous error(s).
      8:              inta = 2 + 2.0;
                      ^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-7: Invalid variable assignment for 'inta' at Line 9:5 to Line 9:16, has expression of non-compatible type 'const float' at Line 9:12 to Line 9:15.
      9:              inta = 2.0;
                      ^^^^^^^^^^^

Info: Variable 'inta' at Line 9:5 to Line 9:9 has type 'int', and is declared at Line 3:5 to Line 3:14:
      3:              int inta;
                      ~~~~~~~~~
--------------------------------------------------------------------------
Error-8: Invalid variable assignment for const-qualified variable 'floata' at Line 11:5 to Line 11:18.
     11:              floata = 3.0;
                      ^^^^^^       

Info: Variable 'floata' at Line 11:5 to Line 11:11 has type 'const float', and is declared at Line 4:5 to Line 4:30:
      4:              const float floata = 2.0;
                      ~~~~~~~~~~~~~~~~~~~~~~~~~
--------------------------------------------------------------------------
Info: Optimization for declaration of const-qualified symbol 'floata' of type 'const float' successful at Line 4:5 to Line 4:30.
(SCOPE
    (DECLARATIONS
        (DECLARATION inta int)
        (DECLARATION floata const float 2.000000)
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE a (BINARY ANY_TYPE + 2 2.000000))
        (ASSIGN ANY_TYPE a 2)
        (ASSIGN ANY_TYPE inta (BINARY ANY_TYPE + 2 2.000000))
        (ASSIGN ANY_TYPE inta 2.000000)
        (ASSIGN ANY_TYPE floata 3.000000)
    )
)
Failed to compile

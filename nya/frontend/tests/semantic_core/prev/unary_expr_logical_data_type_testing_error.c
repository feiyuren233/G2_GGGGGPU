
--------------------------------------------------------------------------
Error-0: Operand in unary expression at Line 9:13 to Line 9:18 has non-compatible type at Line 9:14 to Line 9:18.
      9:              boola = -true;
                              ^^^^^ 

Info: Expecting arithmetic type on right-hand side of operator '-'
--------------------------------------------------------------------------
Error-1: Variable assignment for 'boola' at Line 9:5 to Line 9:19, has expression of unknown type at Line 9:13 to Line 9:18 due to previous error(s).
      9:              boola = -true;
                      ^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Warning-2: Read of potentially unassigned variable 'boolb' of type 'bool' at Line 7:14 to Line 7:19.
      7:              boola = !boolb;
                               ^^^^^ 
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION boola bool)
        (DECLARATION boolb bool)
    )
    (STATEMENTS
        (ASSIGN bool boola (UNARY bool ! boolb))
        (ASSIGN bool boola (UNARY bool ! false))
        (ASSIGN ANY_TYPE boola (UNARY ANY_TYPE - true))
    )
)
Failed to compile

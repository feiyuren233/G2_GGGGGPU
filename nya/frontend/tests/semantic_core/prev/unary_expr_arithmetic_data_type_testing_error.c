
--------------------------------------------------------------------------
Error-0: Operand in unary expression at Line 9:14 to Line 9:18 has non-compatible type at Line 9:15 to Line 9:18.
      9:              floata = !1.3;
                               ^^^^ 

Info: Expecting boolean type on right-hand side of operator '!'
--------------------------------------------------------------------------
Error-1: Variable assignment for 'floata' at Line 9:5 to Line 9:19, has expression of unknown type at Line 9:14 to Line 9:18 due to previous error(s).
      9:              floata = !1.3;
                      ^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-2: Operand in unary expression at Line 10:14 to Line 10:21 has non-compatible type at Line 10:15 to Line 10:21.
     10:              floata = !floatb;
                               ^^^^^^^ 

Info: Expecting boolean type on right-hand side of operator '!'
--------------------------------------------------------------------------
Error-3: Variable assignment for 'floata' at Line 10:5 to Line 10:22, has expression of unknown type at Line 10:14 to Line 10:21 due to previous error(s).
     10:              floata = !floatb;
                      ^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Warning-4: Read of potentially unassigned variable 'floatb' of type 'float' at Line 7:15 to Line 7:21.
      7:              floata = -floatb;
                                ^^^^^^ 
--------------------------------------------------------------------------
Warning-5: Read of potentially unassigned variable 'floatb' of type 'float' at Line 10:15 to Line 10:21.
     10:              floata = !floatb;
                                ^^^^^^ 
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION floata float)
        (DECLARATION floatb float)
    )
    (STATEMENTS
        (ASSIGN float floata (UNARY float - floatb))
        (ASSIGN float floata (UNARY float - 1.200000))
        (ASSIGN ANY_TYPE floata (UNARY ANY_TYPE ! 1.300000))
        (ASSIGN ANY_TYPE floata (UNARY ANY_TYPE ! floatb))
    )
)
Failed to compile

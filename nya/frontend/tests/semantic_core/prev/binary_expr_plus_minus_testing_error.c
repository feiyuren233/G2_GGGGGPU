
--------------------------------------------------------------------------
Error-0: Operands in binary expression at Line 14:14 to Line 14:27 have non-compatible type.
     14:              floata = floata + inta;
                               ^^^^^^^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same type, but they are 'float' and 'int'.
--------------------------------------------------------------------------
Error-1: Variable assignment for 'floata' at Line 14:5 to Line 14:28, has expression of unknown type at Line 14:14 to Line 14:27 due to previous error(s).
     14:              floata = floata + inta;
                      ^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Warning-2: Read of potentially unassigned variable 'inta' of type 'int' at Line 14:23 to Line 14:27.
     14:              floata = floata + inta;
                                        ^^^^ 
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION floata float 0.000000)
        (DECLARATION floatb float 1.000000)
        (DECLARATION floatc float 2.000000)
        (DECLARATION inta int)
    )
    (STATEMENTS
        (ASSIGN float floata (BINARY float + floata floatb))
        (ASSIGN float floata (BINARY float - 1.200000 floatb))
        (ASSIGN float floata (BINARY float + (UNARY float - 1.200000) floatc))
        (ASSIGN ANY_TYPE floata (BINARY ANY_TYPE + floata inta))
    )
)
Failed to compile

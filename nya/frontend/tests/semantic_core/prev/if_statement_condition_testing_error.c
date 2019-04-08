
--------------------------------------------------------------------------
Error-0: Operands in binary expression at Line 13:9 to Line 13:19 have non-compatible type.
     13:              if (inta + 2.0)
                          ^^^^^^^^^^ 

Info: Expecting operands on both sides of operator '+' to have same type, but they are 'int' and 'float'.
--------------------------------------------------------------------------
Error-1: If-statement condition expression has unknown type at Line 13:9 to Line 13:19. Expecting type 'bool'.
     13:              if (inta + 2.0)
                          ^^^^^^^^^^ 
--------------------------------------------------------------------------
Error-2: If-statement condition expression has type 'float' at Line 16:9 to Line 16:15. Expecting type 'bool'.
     16:              if (floata)
                          ^^^^^^ 
--------------------------------------------------------------------------
Warning-3: Read of potentially unassigned variable 'boola' of type 'bool' at Line 10:9 to Line 10:14.
     10:              if (boola)
                          ^^^^^ 
--------------------------------------------------------------------------
Warning-4: Read of potentially unassigned variable 'inta' of type 'int' at Line 13:9 to Line 13:13.
     13:              if (inta + 2.0)
                          ^^^^       
--------------------------------------------------------------------------
Warning-5: Read of potentially unassigned variable 'floata' of type 'float' at Line 16:9 to Line 16:15.
     16:              if (floata)
                          ^^^^^^ 
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION boola bool)
        (DECLARATION inta int)
        (DECLARATION floata float)
    )
    (STATEMENTS
        (IF true )
        (IF boola )
        (IF (BINARY ANY_TYPE + inta 2.000000) )
        (IF floata )
    )
)
Failed to compile

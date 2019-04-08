
--------------------------------------------------------------------------
Warning-0: Read of potentially unassigned variable 'a' of type 'int' at Line 2:13 to Line 2:14.
      2:              int a = a;
                              ^ 
--------------------------------------------------------------------------
Warning-1: Self-initialization involves read of potentially unassigned variable 'a' of type 'int' at Line 2:5 to Line 2:15.
      2:              int a = a;
                      ^^^^^^^^^^
--------------------------------------------------------------------------
Warning-2: Read of potentially unassigned variable 'a' of type 'int' at Line 3:13 to Line 3:14.
      3:              int b = a;
                              ^ 
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION a int a)
        (DECLARATION b int a)
    )
    (STATEMENTS
    )
)

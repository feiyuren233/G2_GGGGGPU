
--------------------------------------------------------------------------
Error-0: Variable declaration of 'int intb' at Line 4:5 to Line 4:20, is initialized to a noncompatible type 'float' at Line 4:16 to Line 4:19.
      4:              int intb = 2.0;
                      ^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-1: Const qualified variable 'const int intd' is initialized to a non-const qualified expression at Line 5:22 to Line 5:26.
      5:              const int intd = inta;
                      ^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-2: Const qualified variable 'const int intg' is initialized to a non-const qualified expression at Line 8:22 to Line 8:30.
      8:              const int intg = inta + 1;
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Warning-3: Read of potentially unassigned variable 'inta' of type 'int' at Line 5:22 to Line 5:26.
      5:              const int intd = inta;
                                       ^^^^ 
--------------------------------------------------------------------------
Warning-4: Read of potentially unassigned variable 'inta' of type 'int' at Line 8:22 to Line 8:26.
      8:              const int intg = inta + 1;
                                       ^^^^     
--------------------------------------------------------------------------
Info: Optimization for declaration of const-qualified symbol 'inte' of type 'const int' successful at Line 6:5 to Line 6:24.
Info: Optimization for declaration of const-qualified symbol 'intf' of type 'const int' successful at Line 7:5 to Line 7:27.
(SCOPE
    (DECLARATIONS
        (DECLARATION inta int)
        (DECLARATION intb int 2.000000)
        (DECLARATION intd const int inta)
        (DECLARATION inte const int 1)
        (DECLARATION intf const int 1)
        (DECLARATION intg const int (BINARY int + inta 1))
    )
    (STATEMENTS
    )
)
Failed to compile

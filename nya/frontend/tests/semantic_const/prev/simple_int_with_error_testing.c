
--------------------------------------------------------------------------
Error-0: Const qualified variable 'const int inta' is initialized to a non-const qualified expression at Line 3:22 to Line 3:27.
      3:              const int inta = 2 + a;
                      ^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Info: Optimization for declaration of const-qualified symbol 'intb' of type 'const int' successful at Line 4:5 to Line 4:24.
Info: Optimization for declaration of const-qualified symbol 'intc' of type 'const int' successful at Line 5:5 to Line 5:39.
(SCOPE
    (DECLARATIONS
        (DECLARATION a int 2)
        (DECLARATION inta const int (BINARY int + 2 a))
        (DECLARATION intb const int 3)
        (DECLARATION intc const int 12)
    )
    (STATEMENTS
    )
)
Failed to compile

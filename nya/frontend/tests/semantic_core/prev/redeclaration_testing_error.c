
--------------------------------------------------------------------------
Error-0: Duplicate declaration of 'const int inta' at Line 4:5 to Line 4:24. Previously declared at Line 3:5 to Line 3:14.
      4:              const int inta = 2;
                      ^^^^^^^^^^^^^^^^^^^

Info: Previously declared here:
      3:              int inta;
                      ~~~~~~~~~
--------------------------------------------------------------------------
Info: Optimization for declaration of const-qualified symbol 'inta' of type 'const int' successful at Line 4:5 to Line 4:24.
(SCOPE
    (DECLARATIONS
        (DECLARATION inta int)
        (DECLARATION inta const int 2)
    )
    (STATEMENTS
    )
)
Failed to compile

Info: Optimization for declaration of const-qualified symbol 'c' of type 'const int' successful at Line 3:5 to Line 3:20.
(SCOPE
    (DECLARATIONS
        (DECLARATION a int 20)
        (DECLARATION c const int 10)
    )
    (STATEMENTS
        (ASSIGN int a (BINARY int << a c))
    )
)

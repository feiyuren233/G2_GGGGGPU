Info: Optimization for declaration of const-qualified symbol 'cond' of type 'const bool' successful at Line 4:5 to Line 4:28.
(SCOPE
    (DECLARATIONS
        (DECLARATION a int)
        (DECLARATION b int)
        (DECLARATION cond const bool true)
    )
    (STATEMENTS
        (IF cond (SCOPE
            (DECLARATIONS
            )
            (STATEMENTS
                (ASSIGN int a 1)
            )
        ) (SCOPE
            (DECLARATIONS
            )
            (STATEMENTS
                (ASSIGN int b 2)
            )
        ))
        (ASSIGN int b a)
    )
)

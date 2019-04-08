(SCOPE
    (DECLARATIONS
        (DECLARATION a int)
        (DECLARATION b int)
    )
    (STATEMENTS
        (IF true (SCOPE
            (DECLARATIONS
            )
            (STATEMENTS
                (ASSIGN int a 1)
            )
        ))
        (ASSIGN int b a)
    )
)

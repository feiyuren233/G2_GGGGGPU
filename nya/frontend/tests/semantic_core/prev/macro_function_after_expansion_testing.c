(SCOPE
    (DECLARATIONS
        (DECLARATION inta int 3)
    )
    (STATEMENTS
        (SCOPE
            (DECLARATIONS
            )
            (STATEMENTS
                (ASSIGN int inta 2)
            )
        )
        
        (ASSIGN int inta (BINARY int + inta 3))
    )
)

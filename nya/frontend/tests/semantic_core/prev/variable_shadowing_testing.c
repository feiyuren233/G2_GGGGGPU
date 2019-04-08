
--------------------------------------------------------------------------
Warning-0: Read of potentially unassigned variable 'a' of type 'int' at Line 4:17 to Line 4:18.
      4:                  int a = a;
                                  ^ 
--------------------------------------------------------------------------
Warning-1: Self-initialization involves read of potentially unassigned variable 'a' of type 'int' at Line 4:9 to Line 4:19.
      4:                  int a = a;
                          ^^^^^^^^^^
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION a int 2)
    )
    (STATEMENTS
        (SCOPE
            (DECLARATIONS
                (DECLARATION a int a)
            )
            (STATEMENTS
            )
        )
    )
)

(SCOPE
    (DECLARATIONS
        (DECLARATION a int)
    )
    (STATEMENTS
        (ASSIGN int a (CALL blkdim))
        (ASSIGN int a (CALL blkidx))
        (ASSIGN int a (CALL tidx))
        (ASSIGN int a (CALL exit))
        (ASSIGN int a (CALL store 123 128 16))
        (ASSIGN int a (CALL store 3.141593 256 32))
    )
)

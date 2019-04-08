
--------------------------------------------------------------------------
Error-0: Invalid variable assignment for 'floatb' at Line 5:5 to Line 5:23, has expression of non-compatible type 'int' at Line 5:14 to Line 5:22.
      5:              floatb = blkdim();
                      ^^^^^^^^^^^^^^^^^^

Info: Variable 'floatb' at Line 5:5 to Line 5:11 has type 'float', and is declared at Line 4:5 to Line 4:18:
      4:              float floatb;
                      ~~~~~~~~~~~~~
--------------------------------------------------------------------------
Error-1: Unmatched function parameters when calling function 'blkidx' at Line 6:12 to Line 6:23.
      6:              inta = blkidx(123);
                             ^^^^^^^^^^^ 

Info: Expecting 0 function argument.
--------------------------------------------------------------------------
Error-2: Variable assignment for 'inta' at Line 6:5 to Line 6:24, has expression of unknown type at Line 6:12 to Line 6:23 due to previous error(s).
      6:              inta = blkidx(123);
                      ^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-3: Invalid variable assignment for 'floatb' at Line 8:5 to Line 8:21, has expression of non-compatible type 'int' at Line 8:14 to Line 8:20.
      8:              floatb = exit();
                      ^^^^^^^^^^^^^^^^

Info: Variable 'floatb' at Line 8:5 to Line 8:11 has type 'float', and is declared at Line 4:5 to Line 4:18:
      4:              float floatb;
                      ~~~~~~~~~~~~~
--------------------------------------------------------------------------
Error-4: Unmatched function parameters when calling function 'store' at Line 9:12 to Line 9:33.
      9:              inta = store(123, 128.1, 16);
                             ^^^^^^^^^^^^^^^^^^^^^ 

Info: Expecting function arguments 'int/float, int, const int'.
--------------------------------------------------------------------------
Error-5: Variable assignment for 'inta' at Line 9:5 to Line 9:34, has expression of unknown type at Line 9:12 to Line 9:33 due to previous error(s).
      9:              inta = store(123, 128.1, 16);
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
Error-6: Unmatched function parameters when calling function 'store' at Line 10:12 to Line 10:33.
     10:              inta = store(123, 128, inta);
                             ^^^^^^^^^^^^^^^^^^^^^ 

Info: Expecting function arguments 'int/float, int, const int'.
--------------------------------------------------------------------------
Error-7: Variable assignment for 'inta' at Line 10:5 to Line 10:34, has expression of unknown type at Line 10:12 to Line 10:33 due to previous error(s).
     10:              inta = store(123, 128, inta);
                      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
--------------------------------------------------------------------------
(SCOPE
    (DECLARATIONS
        (DECLARATION inta int)
        (DECLARATION floatb float)
    )
    (STATEMENTS
        (ASSIGN ANY_TYPE floatb (CALL blkdim))
        (ASSIGN ANY_TYPE inta (CALL blkidx 123))
        (ASSIGN int inta (CALL tidx))
        (ASSIGN ANY_TYPE floatb (CALL exit))
        (ASSIGN ANY_TYPE inta (CALL store 123 128.100006 16))
        (ASSIGN ANY_TYPE inta (CALL store 123 128 inta))
        (ASSIGN int inta (CALL store 3.141593 256 32))
    )
)
Failed to compile

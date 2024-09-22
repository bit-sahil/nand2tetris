#include "stack_init.asm"

// Let's try using stack instead
// and see how functions could be called recursively

    // skip if function is not called (just the declaration here)
    $$GOTO,MFEnd
(MF) //MF stands for multiplication function
    // we have MF0, MF1, and MF2 as global variables 
    // reserved for this function
    // Multiplies MF0 and MF1 and stores the result in MF2
    // caller functions will have to set MF0 and MF1

    // initialize MF1 and MF0 from stack
    $POP
    @MF1
    M=D
    $POP
    @MF0
    M=D

    //initialize MF2=0
    $$ASSIGN, MF2=0

    @MF0
    D=M
    @MF1
    D=D-M //MF0-MF1
    @MF1LessThanMF0
    D;JLT  //MF0 <= MF1
    
    // q = MF1
    $$ASSIGN, q=MF1
    $$ASSIGN, p=MF0
    $$GOTO,ENDMF1LessThanMF0

(MF1LessThanMF0) //R[1] < R[0]
    $$ASSIGN, p=MF1
    $$ASSIGN, q=MF0

(ENDMF1LessThanMF0)
    //r=1
    $$ASSIGN, r=1

(LOOP)
    @q
    D=M
    @ENDLoop
    D;JEQ // end when q=0

    @q
    D=M
    @r
    D=D&M
    @POSTADD
    D;JEQ //do not add if bit doesn't match

    @p
    D=M
    @MF2
    M=D+M // adding p to value at MF2

(POSTADD) //update p, q, r for next iteration
    @p
    D=M
    M=D+M //p = p+p
    
    @r
    D=!M
    @q
    M=D&M  //q=q&(!r)

    @r
    D=M
    M=D+M //r=r+r

    $$GOTO, LOOP

(ENDLoop)

    // Return statement 
    $RET

    // this is where control should come to, if function is not called
(MFEnd)

    // call MF with return address, M0 and M1
    $$CALL, c1EndAddress, 5, 7, MF
(c1EndAddress)
    //store result of calculation
    $$ASSIGN, R0=MF2


    // call MF with return address, M0 and M1
    $$CALL, c2EndAddress, 11, 32, MF
(c2EndAddress)
    //store result of calculation
    $$ASSIGN, R1=MF2

(END)
    $$GOTO, END


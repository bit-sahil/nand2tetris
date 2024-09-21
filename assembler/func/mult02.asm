#include "stack_init.asm"


// Let's try using stack instead
// and see how functions could be called recursively

    // skip if function is not called (just the declaration here)
    @MFEnd
    0;JMP
(MF) //MF stands for multiplication function
    // we have MF0, MF1, and MF2 as global variables 
    // reserved for this function
    // Multiplies MF0 and MF1 and stores the result in MF2
    // caller functions will have to set MF0 and MF1

    @0
    D=A
    @MF2
    M=D //initialize MF2=0

    @MF0
    D=M
    @MF1
    D=D-M //MF0-MF1
    @MF1LessThanMF0
    D;JLT  //MF0 <= MF1
    
    @MF1
    D=M
    @q
    M=D
    @MF0
    D=M
    @p
    M=D
    @ENDMF1LessThanMF0
    0;JMP

(MF1LessThanMF0) //R[1] < R[0]
    @MF1
    D=M
    @p
    M=D
    @MF0
    D=M
    @q
    M=D

(ENDMF1LessThanMF0)
    @1
    D=A
    @r
    M=D //r=1

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

    @LOOP
    0;JMP

(ENDLoop)

    // Jump to where function was called from using stack pointer
    // This is assuming that there is no overflow in SPC
    POP
    A=D
    0;JMP  // jump to current address

    // this is where control should come to, if function is not called
(MFEnd) 

    // initialize sum to 0
    @0
    D=A
    @sum
    M=D

    // 1st caller stores exit address (users are to write code in this way)
    @c1EndAddress
    D=A
    PUSH
    // set variables required for the job
    @5
    D=A
    @MF0
    M=D
    @7
    D=A
    @MF1
    M=D
    @MF
    0;JMP
(c1EndAddress)
    //store result of calculation
    @MF2
    D=M
    @R0
    M=D

    // 2nd caller stores exit address 
    @c2EndAddress
    D=A
    PUSH
    // set variables required for the job
    @11
    D=A
    @MF0
    M=D
    @32
    D=A
    @MF1
    M=D
    @MF
    0;JMP
(c2EndAddress)
    //store result of calculation
    @MF2
    D=M
    @R1
    M=D

(END)
    @END
    0;JMP




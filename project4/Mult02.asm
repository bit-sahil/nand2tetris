// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

//// Replace this comment with your code.
// figure out which is lesser and sum other number that many times?
// but why make things complicated? let's come back to this later
// for now add in R2, R1 times R0

    @0
    D=A
    @R2
    M=D //initialize R2=0

    @R0
    D=M
    @R1
    D=D-M //R[0]-R[1]
    @R1LessThanR0
    D;JLT
    //R[0] <= R[1]
    @R1
    D=M
    @q
    M=D
    @R0
    D=M
    @p
    M=D
    @ENDR1LessThanR0
    0;JMP

(R1LessThanR0) //R[1] < R[0]
    @R1
    D=M
    @p
    M=D
    @R0
    D=M
    @q
    M=D


(ENDR1LessThanR0)
    @1
    D=A
    @r
    M=D //r=1

(LOOP)
    @q
    D=M
    @END
    D;JEQ // end when q=0

    @q
    D=M
    @r
    D=D&M
    @POSTADD
    D;JEQ //do not add if bit doesn't match

    @p
    D=M
    @R2
    M=D+M // adding p to value at R2

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

(END)
    @END
    0;JMP






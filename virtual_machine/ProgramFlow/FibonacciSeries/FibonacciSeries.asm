//push argument 1
    // push value at segment i onto stack (segment among local, argument, this, that)
    @1 //i
    D=A
    @ARG //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//pop pointer 1
    // pop value from stack to segment i 
    @1 //i
    D=A
    @R3 //segment
    D=D+A
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @R14
    A=M
    M=D
//push constant 0
    // handles push constant i
    @0 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop that 0
    // pop value from stack to segment i 
    @0 //i
    D=A
    @THAT //segment
    D=D+M  // addr
    @SP
    AM=M-1 // point to address M-1, while simulataneously updating @SP
    D=D+M // addr+val
    A=D-M // addr+val-val = addr
    M=D-A // addr+val-addr = val
//push constant 1
    // handles push constant i
    @1 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop that 1
    // pop value from stack to segment i 
    @1 //i
    D=A
    @THAT //segment
    D=D+M  // addr
    @SP
    AM=M-1 // point to address M-1, while simulataneously updating @SP
    D=D+M // addr+val
    A=D-M // addr+val-val = addr
    M=D-A // addr+val-addr = val
//push argument 0
    // push value at segment i onto stack (segment among local, argument, this, that)
    @0 //i
    D=A
    @ARG //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 2
    // handles push constant i
    @2 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//sub
    // pop b | pop a | push a-b
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=M-D
//pop argument 0
    // pop value from stack to segment i 
    @0 //i
    D=A
    @ARG //segment
    D=D+M  // addr
    @SP
    AM=M-1 // point to address M-1, while simulataneously updating @SP
    D=D+M // addr+val
    A=D-M // addr+val-val = addr
    M=D-A // addr+val-addr = val
//label LOOP
(LOOP)
//push argument 0
    // push value at segment i onto stack (segment among local, argument, this, that)
    @0 //i
    D=A
    @ARG //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//if-goto COMPUTE_ELEMENT
    // goto <label> if anything other than 0 is stored at stack pointer, also pop
    @SP
    M=M-1
    A=M
    D=M
    @COMPUTE_ELEMENT
    D;JNE
//goto END
    // goto <label>
    @END
    0;JMP
//label COMPUTE_ELEMENT
(COMPUTE_ELEMENT)
//push that 0
    // push value at segment i onto stack (segment among local, argument, this, that)
    @0 //i
    D=A
    @THAT //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//push that 1
    // push value at segment i onto stack (segment among local, argument, this, that)
    @1 //i
    D=A
    @THAT //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//add
    // pop b| pop a| push a+b
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=D+M
//pop that 2
    // pop value from stack to segment i 
    @2 //i
    D=A
    @THAT //segment
    D=D+M  // addr
    @SP
    AM=M-1 // point to address M-1, while simulataneously updating @SP
    D=D+M // addr+val
    A=D-M // addr+val-val = addr
    M=D-A // addr+val-addr = val
//push pointer 1
    // push value at segment i onto stack (segment among local, argument, this, that)
    @1 //i
    D=A
    @R3 //segment
    A=D+A
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 1
    // handles push constant i
    @1 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//add
    // pop b| pop a| push a+b
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=D+M
//pop pointer 1
    // pop value from stack to segment i 
    @1 //i
    D=A
    @R3 //segment
    D=D+A
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @R14
    A=M
    M=D
//push argument 0
    // push value at segment i onto stack (segment among local, argument, this, that)
    @0 //i
    D=A
    @ARG //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 1
    // handles push constant i
    @1 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//sub
    // pop b | pop a | push a-b
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=M-D
//pop argument 0
    // pop value from stack to segment i 
    @0 //i
    D=A
    @ARG //segment
    D=D+M  // addr
    @SP
    AM=M-1 // point to address M-1, while simulataneously updating @SP
    D=D+M // addr+val
    A=D-M // addr+val-val = addr
    M=D-A // addr+val-addr = val
//goto LOOP
    // goto <label>
    @LOOP
    0;JMP
//label END
(END)

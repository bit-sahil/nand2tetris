//push constant 0
    // handles push constant i
    @0 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop local 0
    // pop value from stack to segment i 
    @0 //i
    D=A
    @LCL //segment
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
//push local 0
    // push value at segment i onto stack (segment among local, argument, this, that)
    @0 //i
    D=A
    @LCL //segment
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
//pop local 0
    // pop value from stack to segment i 
    @0 //i
    D=A
    @LCL //segment
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
//if-goto LOOP
    // goto <label> if anything other than 0 is stored at stack pointer, also pop
    @SP
    M=M-1
    A=M
    D=M
    @LOOP
    D;JNE
//push local 0
    // push value at segment i onto stack (segment among local, argument, this, that)
    @0 //i
    D=A
    @LCL //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D

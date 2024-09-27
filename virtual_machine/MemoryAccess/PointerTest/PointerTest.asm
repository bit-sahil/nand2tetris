//push constant 3030
    // handles push constant i
    @3030 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop pointer 0
    // pop value from stack to segment i 
    @0 //i
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
//push constant 3040
    // handles push constant i
    @3040 // i
    D=A
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
//push constant 32
    // handles push constant i
    @32 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop this 2
    // pop value from stack to segment i 
    @2 //i
    D=A
    @THIS //segment
    D=D+M
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @R14
    A=M
    M=D
//push constant 46
    // handles push constant i
    @46 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop that 6
    // pop value from stack to segment i 
    @6 //i
    D=A
    @THAT //segment
    D=D+M
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @R14
    A=M
    M=D
//push pointer 0
    // push value at segment i onto stack (segment among local, argument, this, that)
    @0 //i
    D=A
    @R3 //segment
    A=D+A
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
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
//add
    // pop b| pop a| push a+b
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    M=D+M
    @SP
    M=M+1
//push this 2
    // push value at segment i onto stack (segment among local, argument, this, that)
    @2 //i
    D=A
    @THIS //segment
    A=D+M
    D=M
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
    @SP
    M=M-1
    A=M
    M=M-D
    @SP
    M=M+1
//push that 6
    // push value at segment i onto stack (segment among local, argument, this, that)
    @6 //i
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
    @SP
    M=M-1
    A=M
    M=D+M
    @SP
    M=M+1

//push constant 10
    // handles push constant i
    @10 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//pop local 0
    // pop value from stack to segment i 
    @0 //i
    D=A
    @LCL //segment
    D=D+M
    @SP
    A=M
    M=D
    A=A-1
    D=M
    A=A+1
    A=M
    M=D
    @SP
    M=M-1
//push constant 21
    // handles push constant i
    @21 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 22
    // handles push constant i
    @22 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//pop argument 2
    // pop value from stack to segment i 
    @2 //i
    D=A
    @ARG //segment
    D=D+M
    @SP
    A=M
    M=D
    A=A-1
    D=M
    A=A+1
    A=M
    M=D
    @SP
    M=M-1
//pop argument 1
    // pop value from stack to segment i 
    @1 //i
    D=A
    @ARG //segment
    D=D+M
    @SP
    A=M
    M=D
    A=A-1
    D=M
    A=A+1
    A=M
    M=D
    @SP
    M=M-1
//push constant 36
    // handles push constant i
    @36 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//pop this 6
    // pop value from stack to segment i 
    @6 //i
    D=A
    @THIS //segment
    D=D+M
    @SP
    A=M
    M=D
    A=A-1
    D=M
    A=A+1
    A=M
    M=D
    @SP
    M=M-1
//push constant 42
    // handles push constant i
    @42 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 45
    // handles push constant i
    @45 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//pop that 5
    // pop value from stack to segment i 
    @5 //i
    D=A
    @THAT //segment
    D=D+M
    @SP
    A=M
    M=D
    A=A-1
    D=M
    A=A+1
    A=M
    M=D
    @SP
    M=M-1
//pop that 2
    // pop value from stack to segment i 
    @2 //i
    D=A
    @THAT //segment
    D=D+M
    @SP
    A=M
    M=D
    A=A-1
    D=M
    A=A+1
    A=M
    M=D
    @SP
    M=M-1
//push constant 510
    // handles push constant i
    @510 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//pop temp 6
    // pop value from stack to segment i 
    @6 //i
    D=A
    @R5 //segment
    D=D+A
    @SP
    A=M
    M=D
    A=A-1
    D=M
    A=A+1
    A=M
    M=D
    @SP
    M=M-1
//push local 0
    // push value at segment i onto stack (segment among local, argument, this, that)
    @0 //i
    D=A
    @LCL //segment
    A=D+M
    D=M
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push that 5
    // push value at segment i onto stack (segment among local, argument, this, that)
    @5 //i
    D=A
    @THAT //segment
    A=D+M
    D=M
    @SP
    A=M
    M=D
    @SP
    M=M+1
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
//push argument 1
    // push value at segment i onto stack (segment among local, argument, this, that)
    @1 //i
    D=A
    @ARG //segment
    A=D+M
    D=M
    @SP
    A=M
    M=D
    @SP
    M=M+1
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
//push this 6
    // push value at segment i onto stack (segment among local, argument, this, that)
    @6 //i
    D=A
    @THIS //segment
    A=D+M
    D=M
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push this 6
    // push value at segment i onto stack (segment among local, argument, this, that)
    @6 //i
    D=A
    @THIS //segment
    A=D+M
    D=M
    @SP
    A=M
    M=D
    @SP
    M=M+1
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
//push temp 6
    // push value at segment i onto stack (segment among local, argument, this, that)
    @6 //i
    D=A
    @R5 //segment
    A=D+A
    D=M
    @SP
    A=M
    M=D
    @SP
    M=M+1
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

//Processing file: MemoryAccess/BasicTest/BasicTest.vm
//push constant 10
    // handles push constant i
    @10 // i
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
//push constant 21
    // handles push constant i
    @21 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 22
    // handles push constant i
    @22 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop argument 2
    // pop value from stack to segment i 
    @2 //i
    D=A
    @ARG //segment
    D=D+M  // addr
    @SP
    AM=M-1 // point to address M-1, while simulataneously updating @SP
    D=D+M // addr+val
    A=D-M // addr+val-val = addr
    M=D-A // addr+val-addr = val
//pop argument 1
    // pop value from stack to segment i 
    @1 //i
    D=A
    @ARG //segment
    D=D+M  // addr
    @SP
    AM=M-1 // point to address M-1, while simulataneously updating @SP
    D=D+M // addr+val
    A=D-M // addr+val-val = addr
    M=D-A // addr+val-addr = val
//push constant 36
    // handles push constant i
    @36 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop this 6
    // pop value from stack to segment i 
    @6 //i
    D=A
    @THIS //segment
    D=D+M  // addr
    @SP
    AM=M-1 // point to address M-1, while simulataneously updating @SP
    D=D+M // addr+val
    A=D-M // addr+val-val = addr
    M=D-A // addr+val-addr = val
//push constant 42
    // handles push constant i
    @42 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 45
    // handles push constant i
    @45 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop that 5
    // pop value from stack to segment i 
    @5 //i
    D=A
    @THAT //segment
    D=D+M  // addr
    @SP
    AM=M-1 // point to address M-1, while simulataneously updating @SP
    D=D+M // addr+val
    A=D-M // addr+val-val = addr
    M=D-A // addr+val-addr = val
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
//push constant 510
    // handles push constant i
    @510 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop temp 6
    // pop value from stack to segment i 
    @6 //i
    D=A
    @R5 //segment
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
//push that 5
    // push value at segment i onto stack (segment among local, argument, this, that)
    @5 //i
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
//sub
    // pop b | pop a | push a-b
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=M-D
//push this 6
    // push value at segment i onto stack (segment among local, argument, this, that)
    @6 //i
    D=A
    @THIS //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//push this 6
    // push value at segment i onto stack (segment among local, argument, this, that)
    @6 //i
    D=A
    @THIS //segment
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
//sub
    // pop b | pop a | push a-b
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=M-D
//push temp 6
    // push value at segment i onto stack (segment among local, argument, this, that)
    @6 //i
    D=A
    @R5 //segment
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
    A=A-1
    M=D+M

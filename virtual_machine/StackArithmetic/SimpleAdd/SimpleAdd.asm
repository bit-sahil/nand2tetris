//push constant 7
    // handles push constant i
    @7 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 8
    // handles push constant i
    @8 // i
    D=A
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

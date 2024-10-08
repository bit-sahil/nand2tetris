//Processing file: StackArithmetic/SimpleAdd/SimpleAdd.vm
//push constant 7
    // handles push constant i
    @7 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 8
    // handles push constant i
    @8 // i
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

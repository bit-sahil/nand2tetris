    // call <func> num
    // before call, arguments are already pushed on stack
    @%s%d  // return address label
    D=A
    @SP
    M=M+1
    A=M-1
    M=D  // pushed return address
    @LCL
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed LCL
    @ARG
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed ARG
    @THIS
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THIS
    @THAT
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THAT
    @5
    D=A
    @%d  // num
    D=D+A
    @SP
    D=M-D
    @ARG
    M=D  // pointed ARG to argument 0
    @SP
    D=M
    @LCL
    M=D  // pointing local to current SP
    @%s
    0;JMP  // jump to function execution
(%s%d)  // return address label

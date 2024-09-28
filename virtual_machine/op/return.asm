    // return
    // store return value in argument 0, restore context, jump to return address
    @LCL
    D=M-1 //address of THAT
    @R13
    M=D  // M, D both point at location storing address of THAT
    @4
    A=D-A // points to return address stored
    D=M // return address
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @ARG
    A=M
    M=D  // stored return value at argument 0
    D=A+1
    @SP
    M=D  // restored SP to argument 0 + 1
    @R13
    A=M
    D=M  // addr of THAT
    @THAT
    M=D
    @R13
    AM=M-1
    D=M
    @THIS
    M=D // addr of THIS
    @R13
    AM=M-1
    D=M
    @ARG
    M=D
    @R13
    AM=M-1
    D=M
    @LCL
    M=D
    @R14
    A=M
    0;JMP // cannot be combined with above statement!

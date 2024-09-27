    // pop value from stack to segment i 
    @%d //i
    D=A
    @%s //segment
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

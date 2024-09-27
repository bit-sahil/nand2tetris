    // pop value from stack to segment i 
    @%d //i
    D=A
    @%s //segment
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

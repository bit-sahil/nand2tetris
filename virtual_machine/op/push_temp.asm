    // push value at segment i onto stack (segment among local, argument, this, that)
    @%d //i
    D=A
    @%s //segment
    A=D+A
    D=M
    @SP
    M=M+1
    A=M-1
    M=D

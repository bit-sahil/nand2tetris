    // push value at segment i onto stack (segment among local, argument, this, that)
    @%d //i
    D=A
    @%s //segment
    A=D+A
    D=M
    @SP
    A=M
    M=D
    @SP
    M=M+1

    // pop b | pop a | push a<b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    D=M-D // D=a-b
    @LT_%d
    D;JLT // if a<b
    @END_%d
    D=0;JMP
(LT_%d)
    D=-1
(END_%d)
    @SP
    A=M-1
    M=D

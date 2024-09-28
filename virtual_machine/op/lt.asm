    // pop b | pop a | push a<b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    D=M-D // D=a-b
    @%s$LT_%d
    D;JLT // if a<b
    @%s$END_%d
    D=0;JMP
(%s$LT_%d)
    D=-1
(%s$END_%d)
    @SP
    A=M-1
    M=D

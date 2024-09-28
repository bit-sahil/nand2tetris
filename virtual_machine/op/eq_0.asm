    // pop b | pop a | push a==b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    D=M-D // D=a-b
    @%s$EQ_%d
    D;JEQ // if a==b
    @SP
    A=M
    M=0 // if a!=b
    @%s$END_%d
    0;JMP
(%s$EQ_%d)
    @SP
    A=M
    M=-1
(%s$END_%d)
    @SP
    M=M+1

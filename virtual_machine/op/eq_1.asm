    // pop b | pop a | push a==b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M  // D=b
    A=A-1
    D=D-M // D=a-b
    @%s$EQ_%d
    D;JEQ //jump if a==b, i.e. D=0
    D=-1
(%s$EQ_%d)
    D=!D  // !0=-1
    @SP
    A=M-1
    M=D
    // %s$%d, %s$%d - to not change code formatting file with 4 values

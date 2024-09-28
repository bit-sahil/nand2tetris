    // pop b | pop a | push a==b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M-1  // D=b-1
    A=A-1
    MD=D-M // D=a-b-1; M=-1 if a==b
    @%s$EQ_%d
    D=D+1;JEQ //jump if a==b, i.e. (a-b-1)+1==0
    @SP
    A=M-1
    M=0
(%s$EQ_%d)
    // %s$%d, %s$$%d - to not change code formatting file with 4 values

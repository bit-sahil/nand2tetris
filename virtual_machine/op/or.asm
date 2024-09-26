    // pop b| pop a| push a|b (bitwise or)
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    M=D|M
    @SP
    M=M+1

    // pop b| pop a| push a|b (bitwise or)
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=D|M

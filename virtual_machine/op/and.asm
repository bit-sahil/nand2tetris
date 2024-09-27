    // pop b| pop a| push a&b (bitwise and)
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=D&M

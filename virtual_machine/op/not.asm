    // pop a | push !a (bitwise not)
    @SP
    M=M-1
    A=M
    M=!M
    @SP
    M=M+1

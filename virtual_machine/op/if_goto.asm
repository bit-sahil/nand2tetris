    // goto <label> if anything other than 0 is stored at stack pointer, also pop
    @SP
    M=M-1
    A=M
    D=M
    @%s
    D;JNE

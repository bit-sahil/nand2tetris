    // pop a single numbers from stack, store back negative of number on stack
    // pop a | push -a
    @SP
    M=M-1
    A=M
    M=-M //M = -a
    @SP
    M=M+1

    // pop value from stack to @file_name.i 
    @SP
    M=M-1
    A=M
    D=M
    @%s.%d //<fvar>.<i>
    M=D

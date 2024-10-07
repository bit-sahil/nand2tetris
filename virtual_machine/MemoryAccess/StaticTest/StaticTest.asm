//Processing file: MemoryAccess/StaticTest/StaticTest.vm
//push constant 111
    // handles push constant i
    @111 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 333
    // handles push constant i
    @333 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 888
    // handles push constant i
    @888 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//pop static 8
    // pop value from stack to @file_name.i 
    @SP
    M=M-1
    A=M
    D=M
    @StaticTest.8 //<fvar>.<i>
    M=D
//pop static 3
    // pop value from stack to @file_name.i 
    @SP
    M=M-1
    A=M
    D=M
    @StaticTest.3 //<fvar>.<i>
    M=D
//pop static 1
    // pop value from stack to @file_name.i 
    @SP
    M=M-1
    A=M
    D=M
    @StaticTest.1 //<fvar>.<i>
    M=D
//push static 3
    // push value at <file_name>.i onto stack
    @StaticTest.3 //i
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//push static 1
    // push value at <file_name>.i onto stack
    @StaticTest.1 //i
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//sub
    // pop b | pop a | push a-b
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=M-D
//push static 8
    // push value at <file_name>.i onto stack
    @StaticTest.8 //i
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//add
    // pop b| pop a| push a+b
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=D+M

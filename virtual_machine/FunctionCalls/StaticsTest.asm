    // bootstrap code
    @256
    D=A
    @SP
    M=D
    // todo: call Sys.init
    // call <func> num
    // before call, arguments are already pushed on stack
    @Sys.init$Ret.5  // return address label
    D=A
    @SP
    M=M+1
    A=M-1
    M=D  // pushed return address
    @LCL
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed LCL
    @ARG
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed ARG
    @THIS
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THIS
    @THAT
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THAT
    @5
    D=A
    @0  // num
    D=D+A
    @SP
    D=M-D
    @ARG
    M=D  // pointed ARG to argument 0
    @SP
    D=M
    @LCL
    M=D  // pointing local to current SP
    @Sys.init
    0;JMP  // jump to function execution
(Sys.init$Ret.5)  // return address label
//Processing file: FunctionCalls/StaticsTest/Class1.vm
//function Class1.set 0
(Class1.set)
//push argument 0
    // push value at segment i onto stack (segment among local, argument, this, that)
    @0 //i
    D=A
    @ARG //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//pop static 0
    // pop value from stack to @file_name.i 
    @SP
    M=M-1
    A=M
    D=M
    @Class1.0 //<fvar>.<i>
    M=D
//push argument 1
    // push value at segment i onto stack (segment among local, argument, this, that)
    @1 //i
    D=A
    @ARG //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//pop static 1
    // pop value from stack to @file_name.i 
    @SP
    M=M-1
    A=M
    D=M
    @Class1.1 //<fvar>.<i>
    M=D
//push constant 0
    // handles push constant i
    @0 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//return
    // return
    // store return value in argument 0, restore context, jump to return address
    @LCL
    D=M-1 //address of THAT
    @R13
    M=D  // M, D both point at location storing address of THAT
    @4
    A=D-A // points to return address stored
    D=M // return address
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @ARG
    A=M
    M=D  // stored return value at argument 0
    D=A+1
    @SP
    M=D  // restored SP to argument 0 + 1
    @R13
    A=M
    D=M  // addr of THAT
    @THAT
    M=D
    @R13
    AM=M-1
    D=M
    @THIS
    M=D // addr of THIS
    @R13
    AM=M-1
    D=M
    @ARG
    M=D
    @R13
    AM=M-1
    D=M
    @LCL
    M=D
    @R14
    A=M
    0;JMP // cannot be combined with above statement!
//function Class1.get 0
(Class1.get)
//push static 0
    // push value at <file_name>.i onto stack
    @Class1.0 //i
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//push static 1
    // push value at <file_name>.i onto stack
    @Class1.1 //i
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
//return
    // return
    // store return value in argument 0, restore context, jump to return address
    @LCL
    D=M-1 //address of THAT
    @R13
    M=D  // M, D both point at location storing address of THAT
    @4
    A=D-A // points to return address stored
    D=M // return address
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @ARG
    A=M
    M=D  // stored return value at argument 0
    D=A+1
    @SP
    M=D  // restored SP to argument 0 + 1
    @R13
    A=M
    D=M  // addr of THAT
    @THAT
    M=D
    @R13
    AM=M-1
    D=M
    @THIS
    M=D // addr of THIS
    @R13
    AM=M-1
    D=M
    @ARG
    M=D
    @R13
    AM=M-1
    D=M
    @LCL
    M=D
    @R14
    A=M
    0;JMP // cannot be combined with above statement!
//Processing file: FunctionCalls/StaticsTest/Sys.vm
//function Sys.init 0
(Sys.init)
//push constant 6
    // handles push constant i
    @6 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 8
    // handles push constant i
    @8 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//call Class1.set 2
    // call <func> num
    // before call, arguments are already pushed on stack
    @Class1.set$Ret.4  // return address label
    D=A
    @SP
    M=M+1
    A=M-1
    M=D  // pushed return address
    @LCL
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed LCL
    @ARG
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed ARG
    @THIS
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THIS
    @THAT
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THAT
    @5
    D=A
    @2  // num
    D=D+A
    @SP
    D=M-D
    @ARG
    M=D  // pointed ARG to argument 0
    @SP
    D=M
    @LCL
    M=D  // pointing local to current SP
    @Class1.set
    0;JMP  // jump to function execution
(Class1.set$Ret.4)  // return address label
//pop temp 0
    // pop value from stack to segment i 
    @0 //i
    D=A
    @R5 //segment
    D=D+A
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @R14
    A=M
    M=D
//push constant 23
    // handles push constant i
    @23 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 15
    // handles push constant i
    @15 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//call Class2.set 2
    // call <func> num
    // before call, arguments are already pushed on stack
    @Class2.set$Ret.8  // return address label
    D=A
    @SP
    M=M+1
    A=M-1
    M=D  // pushed return address
    @LCL
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed LCL
    @ARG
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed ARG
    @THIS
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THIS
    @THAT
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THAT
    @5
    D=A
    @2  // num
    D=D+A
    @SP
    D=M-D
    @ARG
    M=D  // pointed ARG to argument 0
    @SP
    D=M
    @LCL
    M=D  // pointing local to current SP
    @Class2.set
    0;JMP  // jump to function execution
(Class2.set$Ret.8)  // return address label
//pop temp 0
    // pop value from stack to segment i 
    @0 //i
    D=A
    @R5 //segment
    D=D+A
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @R14
    A=M
    M=D
//call Class1.get 0
    // call <func> num
    // before call, arguments are already pushed on stack
    @Class1.get$Ret.10  // return address label
    D=A
    @SP
    M=M+1
    A=M-1
    M=D  // pushed return address
    @LCL
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed LCL
    @ARG
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed ARG
    @THIS
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THIS
    @THAT
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THAT
    @5
    D=A
    @0  // num
    D=D+A
    @SP
    D=M-D
    @ARG
    M=D  // pointed ARG to argument 0
    @SP
    D=M
    @LCL
    M=D  // pointing local to current SP
    @Class1.get
    0;JMP  // jump to function execution
(Class1.get$Ret.10)  // return address label
//call Class2.get 0
    // call <func> num
    // before call, arguments are already pushed on stack
    @Class2.get$Ret.11  // return address label
    D=A
    @SP
    M=M+1
    A=M-1
    M=D  // pushed return address
    @LCL
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed LCL
    @ARG
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed ARG
    @THIS
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THIS
    @THAT
    D=M
    @SP
    M=M+1
    A=M-1
    M=D  // pushed THAT
    @5
    D=A
    @0  // num
    D=D+A
    @SP
    D=M-D
    @ARG
    M=D  // pointed ARG to argument 0
    @SP
    D=M
    @LCL
    M=D  // pointing local to current SP
    @Class2.get
    0;JMP  // jump to function execution
(Class2.get$Ret.11)  // return address label
//label END
(Sys$END)
//goto END
    // goto <label>
    @Sys$END
    0;JMP
//Processing file: FunctionCalls/StaticsTest/Class2.vm
//function Class2.set 0
(Class2.set)
//push argument 0
    // push value at segment i onto stack (segment among local, argument, this, that)
    @0 //i
    D=A
    @ARG //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//pop static 0
    // pop value from stack to @file_name.i 
    @SP
    M=M-1
    A=M
    D=M
    @Class2.0 //<fvar>.<i>
    M=D
//push argument 1
    // push value at segment i onto stack (segment among local, argument, this, that)
    @1 //i
    D=A
    @ARG //segment
    A=D+M
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//pop static 1
    // pop value from stack to @file_name.i 
    @SP
    M=M-1
    A=M
    D=M
    @Class2.1 //<fvar>.<i>
    M=D
//push constant 0
    // handles push constant i
    @0 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//return
    // return
    // store return value in argument 0, restore context, jump to return address
    @LCL
    D=M-1 //address of THAT
    @R13
    M=D  // M, D both point at location storing address of THAT
    @4
    A=D-A // points to return address stored
    D=M // return address
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @ARG
    A=M
    M=D  // stored return value at argument 0
    D=A+1
    @SP
    M=D  // restored SP to argument 0 + 1
    @R13
    A=M
    D=M  // addr of THAT
    @THAT
    M=D
    @R13
    AM=M-1
    D=M
    @THIS
    M=D // addr of THIS
    @R13
    AM=M-1
    D=M
    @ARG
    M=D
    @R13
    AM=M-1
    D=M
    @LCL
    M=D
    @R14
    A=M
    0;JMP // cannot be combined with above statement!
//function Class2.get 0
(Class2.get)
//push static 0
    // push value at <file_name>.i onto stack
    @Class2.0 //i
    D=M
    @SP
    M=M+1
    A=M-1
    M=D
//push static 1
    // push value at <file_name>.i onto stack
    @Class2.1 //i
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
//return
    // return
    // store return value in argument 0, restore context, jump to return address
    @LCL
    D=M-1 //address of THAT
    @R13
    M=D  // M, D both point at location storing address of THAT
    @4
    A=D-A // points to return address stored
    D=M // return address
    @R14
    M=D
    @SP
    M=M-1
    A=M
    D=M
    @ARG
    A=M
    M=D  // stored return value at argument 0
    D=A+1
    @SP
    M=D  // restored SP to argument 0 + 1
    @R13
    A=M
    D=M  // addr of THAT
    @THAT
    M=D
    @R13
    AM=M-1
    D=M
    @THIS
    M=D // addr of THIS
    @R13
    AM=M-1
    D=M
    @ARG
    M=D
    @R13
    AM=M-1
    D=M
    @LCL
    M=D
    @R14
    A=M
    0;JMP // cannot be combined with above statement!

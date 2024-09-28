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
//Processing file: FunctionCalls/FibonacciElement/Main.vm
//function Main.fibonacci 0
(Main.fibonacci)
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
//push constant 2
    // handles push constant i
    @2 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//lt
    // pop b | pop a | push a<b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    D=M-D // D=a-b
    @Main$LT_4
    D;JLT // if a<b
    @Main$END_4
    D=0;JMP
(Main$LT_4)
    D=-1
(Main$END_4)
    @SP
    A=M-1
    M=D
//if-goto N_LT_2
    // goto <label> if anything other than 0 is stored at stack pointer, also pop
    @SP
    M=M-1
    A=M
    D=M
    @Main$N_LT_2
    D;JNE
//goto N_GE_2
    // goto <label>
    @Main$N_GE_2
    0;JMP
//label N_LT_2
(Main$N_LT_2)
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
//label N_GE_2
(Main$N_GE_2)
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
//push constant 2
    // handles push constant i
    @2 // i
    D=A
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
//call Main.fibonacci 1
    // call <func> num
    // before call, arguments are already pushed on stack
    @Main.fibonacci$Ret.14  // return address label
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
    @1  // num
    D=D+A
    @SP
    D=M-D
    @ARG
    M=D  // pointed ARG to argument 0
    @SP
    D=M
    @LCL
    M=D  // pointing local to current SP
    @Main.fibonacci
    0;JMP  // jump to function execution
(Main.fibonacci$Ret.14)  // return address label
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
//push constant 1
    // handles push constant i
    @1 // i
    D=A
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
//call Main.fibonacci 1
    // call <func> num
    // before call, arguments are already pushed on stack
    @Main.fibonacci$Ret.18  // return address label
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
    @1  // num
    D=D+A
    @SP
    D=M-D
    @ARG
    M=D  // pointed ARG to argument 0
    @SP
    D=M
    @LCL
    M=D  // pointing local to current SP
    @Main.fibonacci
    0;JMP  // jump to function execution
(Main.fibonacci$Ret.18)  // return address label
//add
    // pop b| pop a| push a+b
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=D+M
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
//Processing file: FunctionCalls/FibonacciElement/Sys.vm
//function Sys.init 0
(Sys.init)
//push constant 4
    // handles push constant i
    @4 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//call Main.fibonacci 1
    // call <func> num
    // before call, arguments are already pushed on stack
    @Main.fibonacci$Ret.3  // return address label
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
    @1  // num
    D=D+A
    @SP
    D=M-D
    @ARG
    M=D  // pointed ARG to argument 0
    @SP
    D=M
    @LCL
    M=D  // pointing local to current SP
    @Main.fibonacci
    0;JMP  // jump to function execution
(Main.fibonacci$Ret.3)  // return address label
//label END
(Sys$END)
//goto END
    // goto <label>
    @Sys$END
    0;JMP

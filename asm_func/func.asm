// Now we want functions we can call multiple times
// to help modularize code

    // skip if function is not called
    @SumFunctionEnd
    0;JMP
(SumFunction)
    // starting with no params, or just global params (i.e. sum)
    // no passing/returning data to worry about
    // we still need to worry about caller and instruction to resume from
    // let's assume we keep separate variables for all invocations (not too bad)
    @SumFunctionBody
    0;JMP
    // Jump to resume where function was called from
    // SumFunctionCaller is address of resume instruction to jump to,
    // and is populated when function is called
(EndFunctionResume)
    @SumFunctionCallerExit
    A=M
    0;JMP
(SumFunctionBody)

    // for(i=0;i<5;i++) sum = i + sum
    @0
    D=A
    @i
    M=D
    @LoopCondition
    0;JMP
(LoopIteration)
    @i
    M=M+1
(LoopCondition)
    @5
    D=A
    @i
    D=M-D
    @LoopEnd
    D;JGE
    // (loop body) execute statements here
    @i
    D=M
    @sum
    M=D+M
    @LoopIteration
    0;JMP
(LoopEnd)

    // End this function and resume control at caller's location
    @EndFunctionResume
    0;JMP
    // this is where control should come to, if function is not called
(SumFunctionEnd) 

    // initialize sum to 0
    @0
    D=A
    @sum
    M=D

    // 1st caller stores exit address (users are to write code in this way)
    @c1EndAddress
    D=A
    @SumFunctionCallerExit
    M=D
    @SumFunction
    0;JMP
(c1EndAddress)

    // 2nd caller stores exit address 
    @c2EndAddress
    D=A
    @SumFunctionCallerExit
    M=D
    @SumFunction
    0;JMP
(c2EndAddress)

    // 3rd caller stores exit address 
    @c3EndAddress
    D=A
    @SumFunctionCallerExit
    M=D
    @SumFunction
    0;JMP
(c3EndAddress)

    // by here, we've added 10 to sum variable 3 times
(END)
    @END
    0;JMP


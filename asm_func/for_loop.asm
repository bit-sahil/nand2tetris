// emulating for(i=0;i<5;i++)
    // sum=0
    @0
    D=A
    @sum
    M=D

    // for loop begins here
    // i=0
    @0
    D=A
    @i
    M=D
    // don't increment value first time, and directly jump to condition
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
    // sum = sum + i
    @i
    D=M
    @sum
    M=D+M
    @LoopIteration  // jump to next iteration
    0;JMP
(LoopEnd)
    // just get to next instruction post loop


    // pop value from stack to segment i 
    @%d //i
    D=A
    @%s //segment
    D=D+M  // addr
    @SP
    AM=M-1 // point to address M-1, while simulataneously updating @SP
    D=D+M // addr+val
    A=D-M // addr+val-val = addr
    M=D-A // addr+val-addr = val

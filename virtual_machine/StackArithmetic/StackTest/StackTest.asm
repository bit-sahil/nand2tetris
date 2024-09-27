//push constant 17
    // handles push constant i
    @17 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 17
    // handles push constant i
    @17 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//eq
    // pop b | pop a | push a==b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M  // D=b
    A=A-1
    D=D-M // D=a-b
    @EQ_3
    D;JEQ //jump if a==b, i.e. D=0
    D=-1
(EQ_3)
    D=!D  // !0=-1
    @SP
    A=M-1
    M=D
    // 33 - to not change code formatting file with 4 values
//push constant 17
    // handles push constant i
    @17 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 16
    // handles push constant i
    @16 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//eq
    // pop b | pop a | push a==b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M  // D=b
    A=A-1
    D=D-M // D=a-b
    @EQ_6
    D;JEQ //jump if a==b, i.e. D=0
    D=-1
(EQ_6)
    D=!D  // !0=-1
    @SP
    A=M-1
    M=D
    // 66 - to not change code formatting file with 4 values
//push constant 16
    // handles push constant i
    @16 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 17
    // handles push constant i
    @17 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//eq
    // pop b | pop a | push a==b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M  // D=b
    A=A-1
    D=D-M // D=a-b
    @EQ_9
    D;JEQ //jump if a==b, i.e. D=0
    D=-1
(EQ_9)
    D=!D  // !0=-1
    @SP
    A=M-1
    M=D
    // 99 - to not change code formatting file with 4 values
//push constant 892
    // handles push constant i
    @892 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 891
    // handles push constant i
    @891 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//lt
    // pop b | pop a | push a<b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    D=M-D // D=a-b
    @LT_12
    D;JLT // if a<b
    @SP
    A=M
    M=0 // if a>b
    @END_12
    0;JMP
(LT_12)
    @SP
    A=M
    M=-1
(END_12)
    @SP
    M=M+1
//push constant 891
    // handles push constant i
    @891 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 892
    // handles push constant i
    @892 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//lt
    // pop b | pop a | push a<b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    D=M-D // D=a-b
    @LT_15
    D;JLT // if a<b
    @SP
    A=M
    M=0 // if a>b
    @END_15
    0;JMP
(LT_15)
    @SP
    A=M
    M=-1
(END_15)
    @SP
    M=M+1
//push constant 891
    // handles push constant i
    @891 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 891
    // handles push constant i
    @891 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//lt
    // pop b | pop a | push a<b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    D=M-D // D=a-b
    @LT_18
    D;JLT // if a<b
    @SP
    A=M
    M=0 // if a>b
    @END_18
    0;JMP
(LT_18)
    @SP
    A=M
    M=-1
(END_18)
    @SP
    M=M+1
//push constant 32767
    // handles push constant i
    @32767 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 32766
    // handles push constant i
    @32766 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//gt
    // pop b | pop a | push a>b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    D=M-D // D=a-b
    @GT_21
    D;JGT // if a>b
    @SP
    A=M
    M=0 // if a<b
    @END_21
    0;JMP
(GT_21)
    @SP
    A=M
    M=-1
(END_21)
    @SP
    M=M+1
//push constant 32766
    // handles push constant i
    @32766 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 32767
    // handles push constant i
    @32767 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//gt
    // pop b | pop a | push a>b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    D=M-D // D=a-b
    @GT_24
    D;JGT // if a>b
    @SP
    A=M
    M=0 // if a<b
    @END_24
    0;JMP
(GT_24)
    @SP
    A=M
    M=-1
(END_24)
    @SP
    M=M+1
//push constant 32766
    // handles push constant i
    @32766 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 32766
    // handles push constant i
    @32766 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//gt
    // pop b | pop a | push a>b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    D=M-D // D=a-b
    @GT_27
    D;JGT // if a>b
    @SP
    A=M
    M=0 // if a<b
    @END_27
    0;JMP
(GT_27)
    @SP
    A=M
    M=-1
(END_27)
    @SP
    M=M+1
//push constant 57
    // handles push constant i
    @57 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 31
    // handles push constant i
    @31 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//push constant 53
    // handles push constant i
    @53 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//add
    // pop b| pop a| push a+b
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    M=D+M
    @SP
    M=M+1
//push constant 112
    // handles push constant i
    @112 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//sub
    // pop b | pop a | push a-b
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    M=M-D
    @SP
    M=M+1
//neg
    // pop a single numbers from stack, store back negative of number on stack
    // pop a | push -a
    @SP
    M=M-1
    A=M
    M=-M //M = -a
    @SP
    M=M+1
//and
    // pop b| pop a| push a&b (bitwise and)
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    M=D&M
    @SP
    M=M+1
//push constant 82
    // handles push constant i
    @82 // i
    D=A
    @SP
    A=M
    M=D
    @SP
    M=M+1
//or
    // pop b| pop a| push a|b (bitwise or)
    @SP
    M=M-1
    A=M
    D=M
    @SP
    M=M-1
    A=M
    M=D|M
    @SP
    M=M+1
//not
    // pop a | push !a (bitwise not)
    @SP
    M=M-1
    A=M
    M=!M
    @SP
    M=M+1

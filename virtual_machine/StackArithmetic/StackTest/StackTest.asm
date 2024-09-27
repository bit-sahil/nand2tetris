//push constant 17
    // handles push constant i
    @17 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 17
    // handles push constant i
    @17 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
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
    M=M+1
    A=M-1
    M=D
//push constant 16
    // handles push constant i
    @16 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
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
    M=M+1
    A=M-1
    M=D
//push constant 17
    // handles push constant i
    @17 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
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
    M=M+1
    A=M-1
    M=D
//push constant 891
    // handles push constant i
    @891 // i
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
    @LT_12
    D;JLT // if a<b
    @END_12
    D=0;JMP
(LT_12)
    D=-1
(END_12)
    @SP
    A=M-1
    M=D
//push constant 891
    // handles push constant i
    @891 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 892
    // handles push constant i
    @892 // i
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
    @LT_15
    D;JLT // if a<b
    @END_15
    D=0;JMP
(LT_15)
    D=-1
(END_15)
    @SP
    A=M-1
    M=D
//push constant 891
    // handles push constant i
    @891 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 891
    // handles push constant i
    @891 // i
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
    @LT_18
    D;JLT // if a<b
    @END_18
    D=0;JMP
(LT_18)
    D=-1
(END_18)
    @SP
    A=M-1
    M=D
//push constant 32767
    // handles push constant i
    @32767 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 32766
    // handles push constant i
    @32766 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//gt
    // pop b | pop a | push a>b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    D=M-D // D=a-b
    @GT_21
    D;JGT // if a>b
    @END_21
    D=0;JMP
(GT_21)
    D=-1
(END_21)
    @SP
    A=M-1
    M=D
//push constant 32766
    // handles push constant i
    @32766 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 32767
    // handles push constant i
    @32767 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//gt
    // pop b | pop a | push a>b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    D=M-D // D=a-b
    @GT_24
    D;JGT // if a>b
    @END_24
    D=0;JMP
(GT_24)
    D=-1
(END_24)
    @SP
    A=M-1
    M=D
//push constant 32766
    // handles push constant i
    @32766 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 32766
    // handles push constant i
    @32766 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//gt
    // pop b | pop a | push a>b (true or false)
    // true = -1, false = 0
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    D=M-D // D=a-b
    @GT_27
    D;JGT // if a>b
    @END_27
    D=0;JMP
(GT_27)
    D=-1
(END_27)
    @SP
    A=M-1
    M=D
//push constant 57
    // handles push constant i
    @57 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 31
    // handles push constant i
    @31 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//push constant 53
    // handles push constant i
    @53 // i
    D=A
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
//push constant 112
    // handles push constant i
    @112 // i
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
//neg
    // pop a single numbers from stack, store back negative of number on stack
    // pop a | push -a
    @SP
    A=M-1
    M=-M //M = -a
//and
    // pop b| pop a| push a&b (bitwise and)
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=D&M
//push constant 82
    // handles push constant i
    @82 // i
    D=A
    @SP
    M=M+1
    A=M-1
    M=D
//or
    // pop b| pop a| push a|b (bitwise or)
    @SP
    M=M-1
    A=M
    D=M
    A=A-1
    M=D|M
//not
    // pop a | push !a (bitwise not)
    @SP
    A=M-1
    M=!M

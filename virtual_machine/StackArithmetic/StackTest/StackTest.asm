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
    D=M-1  // D=b-1
    A=A-1
    MD=D-M // D=a-b-1; M=-1 if a==b
    @StackTest$EQ_3
    D=D+1;JEQ //jump if a==b, i.e. (a-b-1)+1==0
    @SP
    A=M-1
    M=0
(StackTest$EQ_3)
    // StackTest$3, StackTest$$3 - to not change code formatting file with 4 values
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
    D=M-1  // D=b-1
    A=A-1
    MD=D-M // D=a-b-1; M=-1 if a==b
    @StackTest$EQ_6
    D=D+1;JEQ //jump if a==b, i.e. (a-b-1)+1==0
    @SP
    A=M-1
    M=0
(StackTest$EQ_6)
    // StackTest$6, StackTest$$6 - to not change code formatting file with 4 values
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
    D=M-1  // D=b-1
    A=A-1
    MD=D-M // D=a-b-1; M=-1 if a==b
    @StackTest$EQ_9
    D=D+1;JEQ //jump if a==b, i.e. (a-b-1)+1==0
    @SP
    A=M-1
    M=0
(StackTest$EQ_9)
    // StackTest$9, StackTest$$9 - to not change code formatting file with 4 values
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
    @StackTest$LT_12
    D;JLT // if a<b
    @StackTest$END_12
    D=0;JMP
(StackTest$LT_12)
    D=-1
(StackTest$END_12)
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
    @StackTest$LT_15
    D;JLT // if a<b
    @StackTest$END_15
    D=0;JMP
(StackTest$LT_15)
    D=-1
(StackTest$END_15)
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
    @StackTest$LT_18
    D;JLT // if a<b
    @StackTest$END_18
    D=0;JMP
(StackTest$LT_18)
    D=-1
(StackTest$END_18)
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
    @StackTest$GT_21
    D;JGT // if a>b
    @StackTest$END_21
    D=0;JMP
(StackTest$GT_21)
    D=-1
(StackTest$END_21)
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
    @StackTest$GT_24
    D;JGT // if a>b
    @StackTest$END_24
    D=0;JMP
(StackTest$GT_24)
    D=-1
(StackTest$END_24)
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
    @StackTest$GT_27
    D;JGT // if a>b
    @StackTest$END_27
    D=0;JMP
(StackTest$GT_27)
    D=-1
(StackTest$END_27)
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

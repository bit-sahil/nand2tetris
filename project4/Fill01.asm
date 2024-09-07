// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.
//two loops - waits for key press
//if key pressed - make 1 - wait for 0
//if 0 - make 0 - wait for key press

    @16
    D=A
    @rows
    M=D

    @32
    D=A
    @columns
    M=D

    @8192
    D=A
    @bytes
    M=D

(WAIT_FOR_KEY_PRESS)
    @KBD
    D=M
    @FILL_SCREEN
    D;JNE
    @WAIT_FOR_KEY_PRESS
    0;JMP

(WAIT_FOR_KEY_RELEASE)
    @KBD
    D=M
    @UNFILL_SCREEN
    D;JEQ
    @WAIT_FOR_KEY_RELEASE
    0;JMP

(FILL_SCREEN)
    //routine to fill screen goes here
    @SCREEN
    D=A
    @bytes
    D=D+M
    @addr
    M=D //addr stores last byte address on screen + 1
    
(FILL_UNTIL)
    @KBD
    D=M
    @UNFILL_SCREEN
    D;JEQ //start unfilling if key is released in middle

    @SCREEN
    D=A
    @addr
    D=M-D
    @WAIT_FOR_KEY_RELEASE
    D;JEQ

    @addr
    M=M-1
    A=M
    M=-1
    @FILL_UNTIL
    0;JMP

    // now wait until key is released
    @WAIT_FOR_KEY_RELEASE
    0;JMP 

(UNFILL_SCREEN)
    //routine to unfill screen here
    
(UNFILL_UNTIL)
    @KBD
    D=M
    @FILL_SCREEN
    D;JNE //restart filling if key is pressed again in between

    @SCREEN
    D=A
    @bytes
    D=D+M
    @addr
    D=M-D
    @WAIT_FOR_KEY_PRESS
    D;JEQ

    @addr
    A=M
    M=0
    @addr
    M=M+1
    @UNFILL_UNTIL
    0;JMP

    //now wait until a key is pressed
    @WAIT_FOR_KEY_PRESS
    0;JMP





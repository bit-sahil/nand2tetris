    // Jump to where function was called from using stack pointer
    // This is assuming that there is no overflow in SPC
    // essentially return to last address stored in stack
    %POP 
    A=D
    0;JMP  // jump to current address

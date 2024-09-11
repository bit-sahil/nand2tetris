// In this file, we test bitwise shift operations,
// and also resiliency of ALU towards d=0 (d shouldn't affect our ALU)


load ALU2.hdl,
output-file ALU2.out,
compare-to ALU2.cmp,
output-list x%B1.16.1 y%B1.16.1 zx nx zy ny f no m d out%B1.16.1 zr ng ov;

set x %B0000000000000000,  // x = 0
set y %B1111111111111111;  // y = -1

// shift x by 1 bit (d=0, left)
set zx 1,
set nx 0,
set zy 1,
set ny 0,
set f  1,
set no 0,
set m 0,
set d 0,
eval,
output;

// shift x by 1 bit (d=1, right)
set zx 1,
set nx 0,
set zy 1,
set ny 0,
set f  1,
set no 0,
set m 0,
set d 1,
eval,
output;


set x %B1111111111111111;  // x = -1

// shift left
set zx 1,
set nx 1,
set zy 1,
set ny 1,
set f  1,
set no 1,
set m 0,
set d 0,
eval,
output;

// shift right
set zx 1,
set nx 1,
set zy 1,
set ny 1,
set f  1,
set no 1,
set m 0,
set d 1,
eval,
output;


set x %B1010101010101010;

// shift left
set zx 1,
set nx 1,
set zy 1,
set ny 1,
set f  1,
set no 1,
set m 0,
set d 0,
eval,
output;

// shift right
set zx 1,
set nx 1,
set zy 1,
set ny 1,
set f  1,
set no 1,
set m 0,
set d 1,
eval,
output;


// Test ALU for resilience

set x %B0000000000000000,  // x = 0
set y %B1111111111111111;  // y = -1

// Compute -1
set zx 1,
set nx 1,
set zy 1,
set ny 0,
set f  1,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute x
set zx 0,
set nx 0,
set zy 1,
set ny 1,
set f  0,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute y
set zx 1,
set nx 1,
set zy 0,
set ny 0,
set f  0,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute !x
set zx 0,
set nx 0,
set zy 1,
set ny 1,
set f  0,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute !y
set zx 1,
set nx 1,
set zy 0,
set ny 0,
set f  0,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute -x
set zx 0,
set nx 0,
set zy 1,
set ny 1,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute -y
set zx 1,
set nx 1,
set zy 0,
set ny 0,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute x + 1
set zx 0,
set nx 1,
set zy 1,
set ny 1,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute y + 1
set zx 1,
set nx 1,
set zy 0,
set ny 1,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute x - 1
set zx 0,
set nx 0,
set zy 1,
set ny 1,
set f  1,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute y - 1
set zx 1,
set nx 1,
set zy 0,
set ny 0,
set f  1,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute x + y
set zx 0,
set nx 0,
set zy 0,
set ny 0,
set f  1,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute x - y
set zx 0,
set nx 1,
set zy 0,
set ny 0,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute y - x
set zx 0,
set nx 0,
set zy 0,
set ny 1,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute x & y
set zx 0,
set nx 0,
set zy 0,
set ny 0,
set f  0,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute x | y
set zx 0,
set nx 1,
set zy 0,
set ny 1,
set f  0,
set no 1,
set m 1,
set d 0,
eval,
output;

set x %B000000000010001,  // x = 17
set y %B000000000000011;  // y =  3

// Compute 0
set zx 1,
set nx 0,
set zy 1,
set ny 0,
set f  1,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute 1
set zx 1,
set nx 1,
set zy 1,
set ny 1,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute -1
set zx 1,
set nx 1,
set zy 1,
set ny 0,
set f  1,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute x
set zx 0,
set nx 0,
set zy 1,
set ny 1,
set f  0,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute y
set zx 1,
set nx 1,
set zy 0,
set ny 0,
set f  0,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute !x
set zx 0,
set nx 0,
set zy 1,
set ny 1,
set f  0,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute !y
set zx 1,
set nx 1,
set zy 0,
set ny 0,
set f  0,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute -x
set zx 0,
set nx 0,
set zy 1,
set ny 1,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute -y
set zx 1,
set nx 1,
set zy 0,
set ny 0,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute x + 1
set zx 0,
set nx 1,
set zy 1,
set ny 1,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute y + 1
set zx 1,
set nx 1,
set zy 0,
set ny 1,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute x - 1
set zx 0,
set nx 0,
set zy 1,
set ny 1,
set f  1,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute y - 1
set zx 1,
set nx 1,
set zy 0,
set ny 0,
set f  1,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute x + y
set zx 0,
set nx 0,
set zy 0,
set ny 0,
set f  1,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute x - y
set zx 0,
set nx 1,
set zy 0,
set ny 0,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute y - x
set zx 0,
set nx 0,
set zy 0,
set ny 1,
set f  1,
set no 1,
set m 1,
set d 0,
eval,
output;

// Compute x & y
set zx 0,
set nx 0,
set zy 0,
set ny 0,
set f  0,
set no 0,
set m 1,
set d 0,
eval,
output;

// Compute x | y
set zx 0,
set nx 1,
set zy 0,
set ny 1,
set f  0,
set no 1,
set m 1,
set d 0,
eval,
output;

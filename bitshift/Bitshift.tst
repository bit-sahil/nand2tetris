// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/1/Bitshift.tst

load Bitshift.hdl,
output-file Bitshift.out,
compare-to Bitshift.cmp,
output-list in%B1.16.1 dir out%B1.16.1 ov;

set in %B0000000000000000,
set dir 0,
eval,
output;

set in %B0000000000000000,
set dir 1,
eval,
output;

set in %B1111111111111111,
set dir 0,
eval,
output;

set in %B1111111111111111,
set dir 1,
eval,
output;

set in %B1010101010101010,
set dir 0,
eval,
output;

set in %B1010101010101010,
set dir 1,
eval,
output;

set in %B0011110011000011,
set dir 0,
eval,
output;

set in %B0001001000110100,
set dir 1,
eval,
output;

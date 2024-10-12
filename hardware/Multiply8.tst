
load Multiply8.hdl,
output-file Multiply8.out,
compare-to Multiply8.cmp,
output-list a%B1.16.1 b%B1.16.1 out%B1.16.1;

set a %B0000000000000000,
set b %B0000000000000000,
eval,
output;

set a %B0000000000000000,
set b %B0000000011111111,
eval,
output;

set a %B0000000011111111,
set b %B0000000011111111,
eval,
output;

set a %B0000000010101010,
set b %B0000000001010101,
eval,
output;

set a %B00000000011000011,
set b %B0000000011110000,
eval,
output;

set a %B0000000000110100,
set b %B0000000001110110,
eval,
output;

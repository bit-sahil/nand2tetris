#### Project 1: Building Boolean Logic Gates
Building the following chips given Nand gate:
1. Not (1 Nand gate)
2. And (2 Nand gates)
3. Or (3 Nand gates)
4. Xor (5 Nand gates)
5. Mux (4 Nand gates)
6. DMux (5 Nand gates)
7. Not16
8. And16
9. Or16
10. Mux16
11. Or8Way
12. Mux4Way16
13. Mux8Way16
14. DMux4Way
15. DMux8Way

I tried to count Nand gates and come up with alternate implementation to reduce gate counts instead of going by 
exactly known routes with And and Or gates.
But sometimes using slightly more number of gates and keeping simplicity is a good idea.


#### Project 2: Building Boolean Arithmetic chips
Building the following chips using gates/chips and mux/demux built in previous project:
1. HalfAdder
2. FullAdder
3. Add16
4. Inc16
5. ALU

FullAdder is implemented without carry-forward, as recommended, but I do want to come back later and implement a carry forward version of it as well.
Implementing ALU did seem like a big task initially, but since you don't have to figure each operation yourself, it became relatively easy by targeting each control bit function one by one.


#### Project 3: Building Computer Memory
Given a clocked D flip-flop, implement a bit, register, ram, and program counter.
1. Bit
2. Register
3. RAM8
4. RAM64
5. RAM512
6. RAM4K
7. RAM16K
8. PC (Program Counter - reset to 0; set to any location/jump; just increment normally with each clock cycle)

Finally understood how Ram works. Implementing program counter was also fun.


#### Project 4: Getting hands on Assembly Language Programs
1. Fill.asm (an input/output task): This program runs an infinite loop that listens to the
keyboard. When a key is pressed (any key), the program blackens the entire screen by writing
"black" in every pixel.

2. Mult.asm (multiplication): The inputs of this program are the values stored in R0
and R1 (RAM[0] and RAM[1]). The program computes the product R0 * R1 and stores the result in
R2 (RAM[2]).

Writing code in assembly language is just crazy, and imagine how folks would be converting their programs without assemblers. Also, writing first assembler in binary, dealing with all sorts of symbols, lookup table etc. would be a big big task which I'd like to understand more deeply.

Still confused with ticks and tocks operation, but that's for computer architecture to explain it seems.


#### Project 5: Building von-Neumann computer architecture 
It's actually a slight modification of von-Neumann architecture where RAM and ROM are kept separate for simplicity. Here we build
1. Memory: The three main chip-parts of this chip are RAM16K, Screen, and Keyboard. They are given
as builtin chips, and there is no need to implement them. The main implementation task is
combining these three memory chip-parts into a single 32K address space.

2. CPU: The Central Processing Unit can be built from the ALU built in project 2, the Register and PC
chips built in project 3, and logic gates built in project 1.
CPU has been most challenging among all, simplified again by putting all instructions and what goes where in perspective and tackling one at a time.

3. Computer: Just assemble Memory (RAM), CPU, and ROM. Pretty straight-forward.


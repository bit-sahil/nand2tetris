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
  
#### Project 6: Writing an assembler
Developing an assembler that translates programs written in the Hack assembly language into Hack binary code. 
To simplify the code, we assume that the source assembly code is valid, and error handling is deferred.

Development plan:  
We need to read instructions, ignoring whitespaces and comments, and start with reading file line by line
and cleaning instructions.  

##### First Goal: Assembler without handling variables and labels
We start by parsing instruction, and figure out conversion of ASCII strings into 16-bit instructions.  
Handling a-instruction was pretty straigh-forward, and is simply conversion of char str -> int -> 16-bit binary  
Handling c-instruction was divided into 3 parts: ALU instruction, jump instruction, data storage instruction  
While dealing with ALU instruction, there were 28 cases and doing this with if-else statements is very messy,
so a table is helpful. And since we need a symbol table for resolving variables and labels anyway, I decided to
include a map first, to get 1st version of assembler up and running.  

##### 2nd Goal: Start handling variables and labels
For this we already had symbol table, ready to be customized. And finishing assembler was a lot of fun!  
I left symbol table un-optimized and a nice hash function could do well but 
I want to try to keep it as close as what our CPU and hack assembly language could support.  
  
That's also why I chose C as sufficiently low level language to write assembler in, and tried to not use libraries 
extensively. Instead, wrote my small (and not the best) functions to do even as basic operations as string handling 
and conversions.  
Far from perfect, but good to have a version up and running.  
  

#### Next goal (not part of course): understand how Hack could get to have an assembler for itself in hack assembly language.
This is because we wrote assembler in C, but C cannot be run on my computer yet, and I'm not even 100% certain that C can.
In coming up from hack assembly language to C, we've missed out on few gaps.
Perhaps writing it all in assembly would be ideal but I have been reading many accounts of TX-0 and PDP-11 to know that it 
has taken better people at least a few week's time.  
So I'll resort to verify that most things I've taken for granted in C, which are essential to writing an assembler could infact
be written in hack assembly language (or a modification of it, but essentially same hack binary machine code which can run on 
Hack computer we've built so far).  


##### Thinking through, (and starting without any whitespaces or comments), these seems to be essentially what we need
- Reading instruction in hack assembly language
  - Let's say one instruction at a time to simplify things
  - We'd need max 12 characters (longest string is “ADM=D+M;JGT\0”)
  - Then we need a starting address of where this string is being stored (can fix it to let's say address=8000)
  - These characters could be stored in ASCII, and we can do character matching by subtraction/JEQ combination

- Writing instruction back post-processing
  - can again, go into another fixed location (let's say address=8012)

- For a-instruction parsing
  - match '@' char to a=8000, and jump to next task
  - start reading character by character in a loop, and calculate c-'0'
  - initialize inst=0, and keep add value of c-'0'
  - In the loop, if next char is non-null, multiply inst by 10 (1010 in binary) and add next digit
 
- For c-instruction parsing
  - Looks like doing it in 3 passes could be helpful (and it might become more than 3)
  - Need to maintain a symbol table for each pass with key as relevant char sequence in that pass (ALU, jump, store) and value as 16-bit binary word
  - start with inst = 1110,0x,0x,0x and do bitwise or operation with inst with result of each of 3 passes

#####  Some C-constructs
- If/else conditionals
  - Already present, using jump
 
- While loop
  - Already there, using conditional checking and jump
 
- For loop
  - Not essential, but can be simulated (build it's format)
 
- Function
  - helps modularize code and gives ability to re-use routines
  - built function format with global variables (essentially RAM registers), where data could be passed in during invocation, and stored result is used post function completion
  - Main challenge was to be able to pass data back and forth, and pass control back and forth, even multiple times
  - Recursive functions (functions which can call themselves recursively) would take more ingenuity
  
- Multiplication/division
  - Efficient multiplication and division depend on bit-shift which out ALU does not have, so tried adding a Bitshift chip in ALU2.

// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

// for(int i = 0; i < R2; ++i){
//     R3 += R1
// }

@i
M=0
@R3
M=0

(LOOP)
@i
D=M
@R2
D=D-M
@END
D;JEQ
@R1
D=M
@R3
M=D+M
@i
M=M+1
@LOOP
0;JMP

(END)
@END
0;JMP
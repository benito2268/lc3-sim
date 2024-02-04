/*
* cpu.h ~ defs for the cpu component of the emulator
* author: Ben Staehle
* date: 6/20/23
*/

#pragma once
#include<cstdint>
#include<bitset>

#include"debug.h"

#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define RTEMP 8

//memory mapped registers
#define MCR 0xFFFE
#define KBSR 0xFE00
#define KBDR 0xFE02
#define DSR 0xFE04
#define DDR 0xFE06

/*
* notes:
*	- registers hold signed (two's compliment) words BUT are stored as unsigned
*	- RTEMP is only accessable internally - not from assembly
*/

namespace lc3
{
	struct lc_cpu {
	public:

		static inline std::shared_ptr<debug::Debugger> debugger;

		static inline uint16_t pc; //program counter
		static inline uint16_t ir; //instruction register

		//yay for initializer lists
		static inline uint16_t registers[9] = { 0 };
		static inline std::bitset<3> cc {"000"}; //condition codes
		static inline uint16_t mem[0xFFFF] = { 0 };

		static void init(); //sets up memory and loads the entry point
		static void run(); //starts the cpu
		static void stop(); //called by trap halt

		static void set_pc(uint16_t imm);
	};
}
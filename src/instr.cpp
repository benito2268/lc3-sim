/* 
 * instr.cpp ~ defines the set of lc3 instructions
 * author = ben staehle
 * date = 5/17/23
 */

#include"instr.h"
#include"cpu.h"
#include<iostream>
#include<iomanip>
#include<cstdint>
#include<map>

namespace lc3 
{
	using p = lc_cpu;

	//note that uint16_t represents signed data in this implementation
	uint16_t sign_ext_16(uint16_t var, int n) { 
		if ((var >> (n - 1)) & 1) {
			var |= (0xFFFF << n); //negetive
		}
		return var; //if positive, cast will make compiler auto sign extend
	}

	//var must be sign extended
	void set_cc(uint16_t var) {
		if (CHECK_BIT(var, 0)) {
			//positive
			p::cc[2] = 1;
		}
		else {
			if (!var) {
				//zero
				p::cc[1] = 1;
			}
			else {
				//negative
				p::cc[0] = 1;
			}
		}
	}

	//void load_table() {
	//	//instructions are stored at the index equal to the value of their opcode
	//	//note JSRR is excluded since it shares an opcode with JSR
	//	table = { {"BR", &BR}, {"ADD", &ADD}, {"LD", &LD}, {"ST", &ST}, {"JSR", &JSR},
	//			  {"AND", &AND}, {"LDR", &LDR}, {"STR", &STR}, {"???", &ILL}, {"NOT", &NOT},
	//			  {"LDI", &LDI}, {"STI", &STI}, {"JMP", &JMP}, {"???", &ILL}, {"LEA", &LEA},
	//			  {"TRAP", &TRAP} };
	//}

	void NOT() {
		uint16_t dst = (p::ir & 0xE00) >> 9;
		uint16_t src = (p::ir & 0x1FF) >> 6;

		p::registers[dst] = ~p::registers[src];
	}

	void ADD() {
		uint16_t dst = (p::ir & 0xE00) >> 9;
		uint16_t src1 = (p::ir & 0x1FF) >> 6;
		if (CHECK_BIT(p::ir, 11)) {
			//is ADD immediate
			p::registers[dst] = p::registers[src1] + sign_ext_16(p::ir & 0x1F, 5);
		}
		else {
			//is ADD register
			uint16_t src2 = (p::ir & 0x7);
			p::registers[dst] = p::registers[src1] + p::registers[src2];
 		}

		//set condition codes
		set_cc(p::registers[dst]);
	}

	void AND() {
		uint16_t dst = (p::ir & 0xE00) >> 9;
		uint16_t src1 = (p::ir & 0x1FF) >> 6;
		if (CHECK_BIT(p::ir, 11)) {
			//is AND immediate
			p::registers[dst] = p::registers[src1] & sign_ext_16(p::ir & 0x1F, 5);
		}
		else {
			//is AND register
			uint16_t src2 = (p::ir & 0x7);
			p::registers[dst] = p::registers[src1] & p::registers[src2];
		}

		//set condition codes
		set_cc(p::registers[dst]);
	}

	void LDR() {
		//calculate registers
		uint16_t dst = (p::ir & 0xE00) >> 9;
		uint16_t base = (p::ir & 0x1FF) >> 6;

		//calcualte effective address and load from memory
		uint16_t addr = p::registers[base] + (int16_t)(sign_ext_16(p::ir & 0x3F, 6));
		p::registers[dst] = p::mem[addr];

		//set cc's
		set_cc(p::registers[dst]);
	}

	void STR() {
		//calculate registers
		uint16_t src = (p::ir & 0xE00) >> 9;
		uint16_t base = (p::ir & 0x1FF) >> 6;
		
		//caclulate effective addr and store in memory
		uint16_t addr = p::registers[base] + (int16_t)(sign_ext_16(p::ir & 0x3F, 6)); //cast to signed
		p::mem[addr] = p::registers[src];

		//STR does not update cc's
	}

	void LD() {
		uint16_t dst = (p::ir & 0xE00) >> 9;

		//calc effective address
		uint16_t addr = p::pc + (int16_t)(sign_ext_16(p::ir & 0x1FF, 9)); //cast to signed
		p::registers[dst] = p::mem[addr];

		//LD sets the cc's
		set_cc(p::registers[dst]);
	}

	void ST() {
		uint16_t src = (p::ir & 0xE00) >> 9;

		//calc effective address
		uint16_t addr = p::pc + (int16_t)(sign_ext_16(p::ir & 0x1FF, 9)); //cast to signed
		p::mem[addr] = p::registers[src];

		//ST does not set cc's
	}

	void LDI() {
		uint16_t dst = (p::ir & 0xE00) >> 9;

		//find pointer and effective address
		uint16_t ptr = p::pc + (int16_t)(sign_ext_16(p::ir & 0x1FF, 9)); //cast to signed
		uint16_t addr = p::mem[ptr];
		p::registers[dst] = p::mem[addr];

		//LDI sets the cc's
		set_cc(p::registers[dst]);
	}

	void STI() {
		uint16_t src = (p::ir & 0xE00) >> 9;

		//find pointer and effective address
		uint16_t ptr = p::pc + (int16_t)(sign_ext_16(p::ir & 0x1FF, 9)); //cast to signed
		uint16_t addr = p::mem[ptr];
		p::mem[addr] = p::registers[src];

		//STI does not set the cc's
	}

	void LEA() {
		uint16_t dst = (p::ir & 0xE00) >> 9;

		//load pc + offset into the dst register
		p::registers[dst] = p::pc + (int16_t)(sign_ext_16(p::ir & 0x1FF, 9));

		//set cc's
		set_cc(p::registers[dst]);
	}

	void BR() {
		uint16_t nzp = (p::ir & 0xE00) >> 9;
		uint16_t cc = (uint16_t)p::cc.to_ulong(); //cast not really needed but ¯\_(ツ)_/¯

		if (nzp & cc) {
			//this means there is a match so we need to branch
			//note that this works because in c++ vals >= 1 count as true
			p::pc += int16_t(sign_ext_16(p::ir & 0x1FF, 9));
		}

		//BR does not update the cc's
	}

	void JMP() {
		uint16_t base = (p::ir & 0x1C0) >> 6;

		//load the pc with the value of the base register
		//MUST cast base to signed
		p::pc = int16_t(p::registers[base]);
	}

	void TRAP() {
		//which TRAP vector
		uint16_t vec = (p::ir & 0xFF);

		//lookup the trap vector and get subroutine address
		uint16_t addr = p::mem[vec];

		//store the pc in R7
		//note that RET has to be called in each trap routine
		p::registers[R7] = p::pc;

		//jump the processor
		p::pc = addr;
	}

	void JSR() {
		//stores the value of the PC in R7
		p::registers[R7] = p::pc;

		//calculate offset
		int16_t offset = int16_t(sign_ext_16(p::ir & 0x7FF, 11)); //cast to signed int

		//jump the pc to the offset
		p::pc += offset;
	}

	void JSRR() {
		//stores the value of the PC in R7
		p::registers[R7] = p::pc;

		//base register
		uint16_t base = (p::ir & 0x1C0) >> 6;

		//jump to value in base register
		//MUST cast the register to a signed int for this to work 
		p::pc += int16_t(p::registers[base]);
	}

	void ILL() {
		//illegal instruction
		std::cout << "LC3_ERROR: unrecognized opcode at " << std::hex << p::pc << std::endl;
		//dump code around the error to file/screen
		p::stop();
	}
}
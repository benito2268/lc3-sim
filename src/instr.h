#pragma once
#include<string>
#include<vector>

#define CHECK_BIT(var, pos)		\
	((var) & (1<<(pos)))		\

namespace lc3 
{
	/*
	* defines functions for each lc3 instruction
	* the instruction is read directly from the IR instead of passed in
	* Note: this version DOES implement opcode 1000 as "RTI" as thus, also implements interrupts
	*/

	uint16_t sign_ext_16(uint16_t var, int n);

	void NOT();
	void ADD();
	void AND();
	void LDR();
	void STR();
	void LD();
	void ST();
	void LDI();
	void STI();
	void LEA();
	void BR();
	void JMP();
	void TRAP();
	void JSR();
	void JSRR();
	void RTI();

	void ILL(); //catches any illegal instructions

	struct instruction {
		std::string name;
		void (*operation)(void) = nullptr;
	};

	static inline std::vector<instruction> table = { {"BR", &BR}, {"ADD", &ADD}, {"LD", &LD}, {"ST", &ST}, {"JSR", &JSR},
				  {"AND", &AND}, {"LDR", &LDR}, {"STR", &STR}, {"???", &ILL}, {"NOT", &NOT},
				  {"LDI", &LDI}, {"STI", &STI}, {"JMP", &JMP}, {"???", &ILL}, {"LEA", &LEA},
				  {"TRAP", &TRAP} };

	void load_table();
}
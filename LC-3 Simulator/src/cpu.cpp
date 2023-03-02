//lc3 simulator - file cpu.cpp
#include<cpu.h>
#include<instr.h>
#include<base.h>
#include<utils.h>
#include<iostream>

namespace lc3 {
    
    void Cpu::init() {
        //zero out all of the registers
        for(auto& element : this->registers) {
            element = std::bitset<16> {"0000000000000000"};
        }

       init_mem();

        //set the cc's to Z -> "010"
        this->cc = std::bitset<3> {"010"};

        //the pc starts at x3000 by default, although this MUST be specified in the assembly
        //0x3000 in bin is 0011000000000000
        this->pc = std::bitset<16> {"0011000000000000"};

        //the ir starts with BR none (x0000)
        //but again must be changed
        this->ir = std::bitset<16> {"0000000000000000"};

        //our little test program
        mem[0x3000] = std::bitset<16>{"1110000000000010"};
        mem[0x3001] = std::bitset<16>{"1111000000100010"};
        mem[0x3002] = std::bitset<16>{"1111000000100101"};
        mem[0x3003] = std::bitset<16>{"0000000001001000"};
    }

    void Cpu::init_mem() {
       //zero out all of the memory - this doesn't actually happen in lc3 normally
        for(auto& element : this->mem) {
            element = std::bitset<16> {"0000000000000000"};
        }
    }

    void Cpu::exec() {
        //fetch excecute cycle

        //FI stage
        //=====================================
        int inst_addr = conv::bin16_to_dec(pc);
        ir = mem[inst_addr];

        //std::cout << pc.to_string() << std::endl;

        //increment the pc
        util_funcs::addToPC(std::bitset<16>{"0000000000000001"});

        //DI stage
        //=====================================
        std::bitset<4> opcode{ir.to_string().substr(0, 4)}; 

        //EO stage
        //=====================================
        INSTR* instruction = util_funcs::getInstFromOpCode(ir);
        instruction->exec(ir);
        delete instruction;
    }

    void Cpu::halt() {
        std::cout << "*END OF PROGRAM*" << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "PC -> " << Cpu::pc.to_string() << std::endl;
        std::cout << "IR -> " << Cpu::ir.to_string() << std::endl;
        std::cout << "===============================" << std::endl;
        std::cout << "R0 -> " << Cpu::registers[0].to_string() << std::endl;
        std::cout << "R1 -> " << Cpu::registers[1].to_string() << std::endl;
        std::cout << "R2 -> " << Cpu::registers[2].to_string() << std::endl;
        std::cout << "R3 -> " << Cpu::registers[3].to_string() << std::endl;
        std::cout << "R4 -> " << Cpu::registers[4].to_string() << std::endl;
        std::cout << "R5 -> " << Cpu::registers[5].to_string() << std::endl;
        std::cout << "R6 -> " << Cpu::registers[6].to_string() << std::endl;
        std::cout << "R7 -> " << Cpu::registers[7].to_string() << std::endl;
        std::cout << "===============================" << std::endl;

        std::exit(0);
    }
}

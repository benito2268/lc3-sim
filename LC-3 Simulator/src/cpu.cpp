//lc3 simulator - file cpu.cpp
#include<cpu.h>
#include<base.h>
#include<utils.h>

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

        //increment the pc
        addToPC(std::bitset<16>{"0000000000000001"});

        //DI stage
        //=====================================
        std::bitset<4> opcode = {ir.to_string().sub_str(0, 4)};
        
    }

    void Cpu::halt() {

    }
}
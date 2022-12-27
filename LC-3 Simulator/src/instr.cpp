// lc-3 simulator - file instr.cpp
// defines the semantics of LC-3 intructions
#include<instr.h>
#include<base.h>
#include<cpu.h>
#include<utils.h>
#include<string>

namespace lc3 {

    void NOT::exec(Instruction i) {
        //for not we only care about bits 11-6
        //TODO check this code (it doesnt work)
        int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});
        int src = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});

        //test code
        Cpu::registers[src] = std::bitset<16>{"1111111100000000"};

        //perform not and store in dst
        for(int i = 0; i < WORDSIZE; i++) {
            //not each bit
            Cpu::registers[dst][i] = !Cpu::registers[src][i];
        }

        //set cc's
        setCC(Cpu::registers[dst]);
    }

    void ADD::exec(Instruction i) {
        int tmp_dst = 0;        
        //instruction is immediate if bit 5 is 1
        if(i[5] == 1) {
            //the instruction is "add immediate"
            //interperate the instruction
            int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});
            tmp_dst = dst;
            int src1 = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});

            //get immediate value into bitset
            std::bitset<WORDSIZE> imm(std::string(i.to_string().substr(11, 5)));

            int carry = 0;

            //perform not and store in dst
            for(int i = 0; i < WORDSIZE || carry != 0; i++) {
                int bitsum = Cpu::registers[src1][i] + imm[i] + carry;
                switch(bitsum) {
                    case 0:
                        Cpu::registers[dst][i] = 0;
                        break;
                    case 1:
                        Cpu::registers[dst][i] = 1;
                        if(carry) {carry = 0;}
                        //carry is cleared if it was set
                        break;
                    case 2:
                        Cpu::registers[dst][i] = 0;
                        if(!carry) {
                            carry = 1;
                        }
                        //carry remains 1 if it was, otherwise is set to 1
                        break;
                    case 3:
                        Cpu::registers[dst][i] = 1;
                        //carry remains 1
                        break;
                    default:
                        //throw some kind of error
                        break;
                }   
            }
            
        } else {
            //the instruction is add "add register"
            //interperate the instruction
            int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});
            tmp_dst = dst;
            int src1 = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});
            int src2 = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(13, 3)});

            int carry = 0;

            //perform not and store in dst
            for(int i = 0; i < WORDSIZE; i++) {
                int bitsum = Cpu::registers[src1][i] + Cpu::registers[src2][i] + carry;
                switch(bitsum) {
                    case 0:
                        Cpu::registers[dst][i] = 0;
                        break;
                    case 1:
                        Cpu::registers[dst][i] = 1;
                        if(carry) {carry = 0;}
                        //carry is cleared if it was set
                        break;
                    case 2:
                        Cpu::registers[dst][i] = 0;
                        if(!carry) {
                            carry = 1;
                        }
                        //carry remains 1 if it was, otherwise is set to 1
                        break;
                    case 3:
                        Cpu::registers[dst][i] = 1;
                        //carry remains 1
                        break;
                    default:
                        //throw some kind of error
                        break;
                }      
            }
        }

        //set cc's
        setCC(Cpu::registers[tmp_dst]);
    }

    void AND::exec(Instruction i) {
        int tmp_dst = 0;
        if(i[5] == 1) {
            //the instruction is "and immediate"
            //interperate the instruction
            int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});
            tmp_dst = dst;
            int src1 = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});

            //get immediate value into bitset
            std::bitset<WORDSIZE> imm(std::string(i.to_string().substr(11, 5)));

            //loop through and and 
            for(int i = 0; i < WORDSIZE; i++) {
                Cpu::registers[dst][i] = Cpu::registers[src1][i] & imm[i];
            }

        } else {
            //the instruction is "and register"
            int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});
            tmp_dst = dst;
            int src1 = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});
            int src2 = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(13, 3)});

            //loop through and and 
            for(int i = 0; i < WORDSIZE; i++) {
                Cpu::registers[dst][i] = Cpu::registers[src1][i] & Cpu::registers[src2][i];
            }
        }

        //set cc's
        setCC(Cpu::registers[tmp_dst]);
    }

    void BR::exec(Instruction i) {
        //check the cc
        if(Cpu::cc[0] == 1) {
            //positive
            if(i[9] == 1) {
                goto do_branch;
            }

        } else if(Cpu::cc[2] == 1) {   
            //negetive
            if(i[10] == 1) {
                goto do_branch;
            }
        } else if(Cpu::cc[1] == 1) {
            //zero
            if(i[11] == 1) {
                goto do_branch;
            }
        } else {
            //else it's BR(never) so just return
            return;
        }
        //cursed label
        do_branch:
            //do the branch - add pc to offset
            addToPC(std::bitset<16>{i.to_string().substr(7, 9)});
    }

    void JMP::exec(Instruction i) {
        //really simple, just goes to the address in the base reg
        int base_r = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});
        Cpu::pc = Cpu::registers[base_r];
    }

    void JSR::exec(Instruction i) {
        //save the return addr in R7
        Cpu::registers[R7] = Cpu::pc;

        //add offset to pc
        addToPC(std::bitset<16>{i.to_string().substr(6)});
    }

    void JSRR::exec(Instruction i) {
        //set the temp reg
        Cpu::registers[RTEMP] = Cpu::registers[R7];

        //same as jmp except save R7
        int base_r = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});

        //set the pc
        Cpu::pc = Cpu::registers[base_r];

        //save R7
        Cpu::registers[R7] = Cpu::registers[RTEMP];
    }

    void LD::exec(Instruction i) {
        //seperate the dst register
        int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});

        //isolate the offset
        std::bitset<16> offset = std::bitset<16>{i.to_string().substr(7)};
        std::bitset<16> bin_addr = addPCTo(offset);

        //convert the effective address to decimal
        int dec_addr = conv::bin16_to_dec(bin_addr);

        //load the binary value in memory into the destination register
        Cpu::registers[dst] = Cpu::mem[dec_addr];

        //set the cc's
        setCC(Cpu::registers[dst]);
    }

    void LDI::exec(Instruction i) {
        //seperate the dst register
        int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});

        //isolate the offset
        std::bitset<16> offset = std::bitset<16>{i.to_string().substr(7)};
        std::bitset<16> bin_addr = addPCTo(offset);

        //convert the effective address to decimal
        int dec_addr = conv::bin16_to_dec(bin_addr);

        //load the binary value in memory into the destination register
        std::bitset<16> b_effetive_addr = Cpu::mem[dec_addr];

        //do it again - and this time actually load the value
        int d_effective_addr = conv::bin16_to_dec(b_effetive_addr);
        Cpu::registers[dst] = Cpu::mem[d_effective_addr];

        //set the cc's
        setCC(Cpu::registers[dst]);
    }

    void LDR::exec(Instruction i) {
        //compute the destination and base registers
        int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});
        int base = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});

        //compute the offset
        int offset = conv::bin6_to_dec(std::bitset<6>{i.to_string().substr(10)});
        int base_addr = conv::bin16_to_dec(Cpu::registers[base]);
        
        //store the result
        Cpu::registers[dst] = Cpu::mem[offset + base_addr];

        //set the cc's
        setCC(Cpu::registers[dst]);
    }

    void LEA::exec(Instruction i) {
        //calculate the dst register
        int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});

        //calculate the offset and add it to PC
        std::bitset<16> offset{i.to_string().substr(7)};
        
        //put the effective address in the dst register
        Cpu::registers[dst] = addPCTo(offset);

        //set the cc's
        setCC(Cpu::registers[dst]);
    }

    void RET::exec(Instruction i) {
        //same as jmp R7
        Cpu::pc = Cpu::registers[R7];
    }

    void RTI::exec(Instruction i) {
        //currently unused
    }

    void ST::exec(Instruction i) {
        //determine src register
        int src = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});

        //determine offset
        std::bitset<16> offset = std::bitset<16>{i.to_string().substr(7)};
        std::bitset<16> bin_addr = addPCTo(offset);

        //convert the effective address to decimal
        int dec_addr = conv::bin16_to_dec(bin_addr);

        //store the contents of src in mem
        Cpu::mem[dec_addr] = Cpu::registers[src];
    }

    void STI::exec(Instruction i) {
        //determine src register
        int src = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});

        //determine offset
        std::bitset<16> offset = std::bitset<16>{i.to_string().substr(7)};
        std::bitset<16> bin_addr_ind = addPCTo(offset);

        //convert the indirect address to decimal
        int dec_addr_ind = conv::bin16_to_dec(bin_addr_ind);

        //compute the effective address
        std::bitset<16> bin_addr_eff = Cpu::mem[dec_addr_ind];
        int dec_addr_eff = conv::bin16_to_dec(bin_addr_eff);

        //store the contents
        Cpu::mem[dec_addr_eff] = Cpu::registers[src];
    }

    void STR::exec(Instruction i) {
        //compute the src and base registers
        int src = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});
        int base = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});

        //compute the offset
        int offset = conv::bin6_to_dec(std::bitset<6>{i.to_string().substr(10)});
        int base_addr = conv::bin16_to_dec(Cpu::registers[base]);

        Cpu::mem[base_addr + offset] = Cpu::registers[src];
    }

    void TRAP::exec(Instruction i) {
        //jump to the specified trap vector
        //JUMP Table layout
        //===========================
        //| CODE | VECTOR  | JUMPTO |
        //===========================
        //|OUT   | x0021   | x0430  |
        //|IN    | x0023   | x04A0  |
        //|HALT  | x0025   | xFD70  |
        //|PUTS  | x0022   | x0450  |
        //|GETC  | x0020   | x0400  |
        //===========================

        //load address from jump vector table
        int vector = conv::bin8_to_dec(i.to_string().substr(7));
        std::bitset<16> jmp_addr = Cpu::mem[vector];
        int jmp_addr_dec = bin16_to_dec(jmp_addr);

        //jump to the subroutine
        Cpu::pc = Cpu::mem[jmp_addr_dec];
    }
}
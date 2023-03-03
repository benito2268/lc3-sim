// lc-3 simulator - file instr.cpp
// defines the semantics of LC-3 intructions
#include<instr.h>
#include<base.h>
#include<cpu.h>
#include<utils.h>
#include<cstdio>
#include<iostream>
#include<string>

namespace lc3 {

    void NOT::exec(Instruction i) {
        //for not we only care about bits 6-11
        int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});
        int src = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});

        //perform not and store in dst
        for(int i = 0; i < WORDSIZE; i++) {
            //not each bit
            Cpu::registers[dst][i] = !Cpu::registers[src][i];
        }

        //set cc's
        util_funcs::setCC(Cpu::registers[dst]);
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

            //get immediate value from instruction
            std::bitset<WORDSIZE> imm(std::string(i.to_string().substr(11, 5)));

            int carry = 0;

            //perform add and store in dst
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

            //perform add and store in dst
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
        util_funcs::setCC(Cpu::registers[tmp_dst]);
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
        util_funcs::setCC(Cpu::registers[tmp_dst]);
    }

    //branch according the the cpu's control codes
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
            //else it's BR never so just return
            return;
        }
        //cursed label
        do_branch:
            //do the branch - add pc to offset
            util_funcs::addToPC(std::bitset<16>{i.to_string().substr(7, 9)});
    }

    //jump
    void JMP::exec(Instruction i) {
        //really simple, just goes to the address in the base reg
        int base_r = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});
        Cpu::pc = Cpu::registers[base_r];
    }

    //jump to subroutine
    void JSR::exec(Instruction i) {
        //save the return addr in R7
        Cpu::registers[R7] = Cpu::pc;

        //add offset to pc
        util_funcs::addToPC(std::bitset<16>{i.to_string().substr(6)});
    }

    //jump to subroutine register
    void JSRR::exec(Instruction i) {
        //set the temp reg
        Cpu::registers[RTEMP] = Cpu::registers[R7];

        //same as jmp except save R7
        int base_r = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});

        //set the pc
        Cpu::pc = Cpu::registers[base_r];

        //restore R7
        Cpu::registers[R7] = Cpu::registers[RTEMP];
    }

    //load pc-relative
    void LD::exec(Instruction i) {
        //seperate the dst register
        int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});

        //isolate the offset
        std::bitset<16> offset = std::bitset<16>{i.to_string().substr(7)};
        util_funcs::addPCTo(offset);

        //convert the effective address to decimal
        int dec_addr = conv::bin16_to_dec(offset);

        //load the binary value in memory into the destination register
        Cpu::registers[dst] = Cpu::mem[dec_addr];

        //set the cc's
        util_funcs::setCC(Cpu::registers[dst]);
    }

    //load indirect
    void LDI::exec(Instruction i) {
        //seperate the dst register
        int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});

        //isolate the offset
        std::bitset<16> offset = std::bitset<16>{i.to_string().substr(7)};
        util_funcs::addPCTo(offset);

        //convert the effective address to decimal
        int dec_addr = conv::bin16_to_dec(offset);

        //load the binary value in memory into the destination register
        std::bitset<16> b_effetive_addr = Cpu::mem[dec_addr];

        //do it again - and this time actually load the value
        int d_effective_addr = conv::bin16_to_dec(b_effetive_addr);
        Cpu::registers[dst] = Cpu::mem[d_effective_addr];

        //set the cc's
        util_funcs::setCC(Cpu::registers[dst]);
    }

    //load base-offset
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
        util_funcs::setCC(Cpu::registers[dst]);
    }

    //load effective address
    void LEA::exec(Instruction i) {
        //calculate the dst register
        int dst = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});

        //calculate the offset and add it to PC
        std::bitset<16> offset{i.to_string().substr(7)};
        
        //put the effective address in the dst register
        util_funcs::addPCTo(offset);
        Cpu::registers[dst] = offset;
        //set the cc's
        util_funcs::setCC(Cpu::registers[dst]);
    }

    //return
    void RET::exec(Instruction i) {
        //same as jmp R7
        Cpu::pc = Cpu::registers[R7];
    }

    //return from interrupt
    void RTI::exec(Instruction i) {
        //currently unused
    }

    //store pc-relative
    void ST::exec(Instruction i) {
        //determine src register
        int src = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});

        //determine offset
        std::bitset<16> offset = std::bitset<16>{i.to_string().substr(7)};
        util_funcs::addPCTo(offset);

        //convert the effective address to decimal
        int dec_addr = conv::bin16_to_dec(offset);

        //store the contents of src in mem
        Cpu::mem[dec_addr] = Cpu::registers[src];
    }

    //store indirect
    void STI::exec(Instruction i) {
        //determine src register
        int src = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});

        //determine offset
        std::bitset<16> offset = std::bitset<16>{i.to_string().substr(7)};
        util_funcs::addPCTo(offset);

        //convert the indirect address to decimal
        int dec_addr_ind = conv::bin16_to_dec(offset);

        //compute the effective address
        std::bitset<16> bin_addr_eff = Cpu::mem[dec_addr_ind];
        int dec_addr_eff = conv::bin16_to_dec(bin_addr_eff);

        //store the contents
        Cpu::mem[dec_addr_eff] = Cpu::registers[src];
    }

    //store base-offset
    void STR::exec(Instruction i) {
        //compute the src and base registers
        int src = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(4, 3)});
        int base = conv::bin3_to_dec(std::bitset<3>{i.to_string().substr(7, 3)});

        //compute the offset
        int offset = conv::bin6_to_dec(std::bitset<6>{i.to_string().substr(10)});
        int base_addr = conv::bin16_to_dec(Cpu::registers[base]);

        Cpu::mem[base_addr + offset] = Cpu::registers[src];
    }

    //jump to system service routine
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
        int vector = conv::bin8_to_dec(std::bitset<8>{i.to_string().substr(8)});
        std::bitset<16> jmp_addr = Cpu::mem[vector];
        int jmp_addr_dec = conv::bin16_to_dec(jmp_addr);

        //printf("%d\n", vector);
        //std::cout << i.to_string().substr(7) << std::endl;

        //jump to the subroutine ~ just kidding nerd
        //Cpu::pc = Cpu::mem[jmp_addr_dec];
        switch(vector) {
        case 0x20:
            break;
        case 0x21:
            break;
        case 0x22: {
            //puts ~ print what's AT THE ADDRESS in R0 as string
            char* str = util_funcs::binToStr(Cpu::registers[0]);
            printf("%s", str);
            delete str;
            break;
        }
        case 0x23:
            break;
        case 0x25:
            Cpu::halt();
            break;
        }
    }
}

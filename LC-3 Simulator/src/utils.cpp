//lc3 simulator - file utils.cpp
#include<utils.h>
#include<string>
#include<cpu.h>
#include<instr.h>

namespace lc3 {
     //add to PC helper function
    void util_funcs::addToPC(std::bitset<16> offset) {
        //note: offset sign extended to 16 bits
       int carry = 0;

        //perform add and store in dst
        for(int i = 0; i < WORDSIZE; i++) {
            int bitsum = Cpu::pc[i] + offset[i] + carry;
            switch(bitsum) {
                case 0:
                    Cpu::pc[i] = 0;
                    break;
                case 1:
                    Cpu::pc[i] = 1;
                    if(carry) {carry = 0;}
                    //carry is cleared if it was set
                    break;
                case 2:
                    Cpu::pc[i] = 0;
                    if(!carry) {
                        carry = 1;
                    }
                    //carry remains 1 if it was, otherwise is set to 1
                    break;
                case 3:
                    Cpu::pc[i] = 1;
                    //carry remains 1
                    break;
                default:
                    //throw some kind of error
                    break;
            }      
        }
    }

    void util_funcs::addPCTo(Register& result) {
        //adds the value of the PC to bitset provided
        //used mainly for calculating offsets
        //note: offset sign extended to 16 bits
        int carry = 0;

        //perform add and store in dst
        for(int i = 0; i < WORDSIZE; i++) {
            int bitsum = Cpu::pc[i] + result[i] + carry;
            switch(bitsum) {
                case 0:
                    result[i] = 0;
                    break;
                case 1:
                    result[i] = 1;
                    if(carry) {carry = 0;}
                    //carry is cleared if it was set
                    break;
                case 2:
                    result[i] = 0;
                    if(!carry) {
                        carry = 1;
                    }
                    //carry remains 1 if it was, otherwise is set to 1
                    break;
                case 3:
                    result[i] = 1;
                    //carry remains 1
                    break;
                default:
                    //throw some kind of error
                    break;
            }      
        }
    }

    void util_funcs::setCC(Register& result) {
        //clear the previous control code
        Cpu::cc = std::bitset<3>{"000"};

        //determine the correct bit to set
        std::bitset<16> zero{"0000000000000000"};
        if(result == zero) {
            //set the 'z' bit 
            Cpu::cc[1] = 1;
            return;
        } else if(result[15] == 1) {
            //set the 'n' bit
            Cpu::cc[0] = 1;
            return;
        } else {
            //else set the 'p' bit
            Cpu::cc[2] = 1;
        }
    }

    INSTR* util_funcs::getInstFromOpCode(std::bitset<16> instr) {
        std::string opstr = instr.to_string().substr(0, 4);
        if(opstr == "1001") {
            //NOT
            return new NOT();
        } else if(opstr == "0001") {
            //ADD
            return new ADD();
        } else if(opstr == "0101") {
            //AND
            return new AND();
        } else if(opstr == "0110") {
            //LDR
            return new LDR();
        } else if(opstr == "0111") {
            //STR
            return new STR();
        } else if(opstr == "0010") {
            //LD
            return new LD();
        } else if(opstr == "0011") {
            //ST
            return new ST();
        } else if(opstr == "1010") {
            //LDI
            return new LDI();
        } else if(opstr == "1011") {
            //STI
            return new STI();
        } else if(opstr == "1110") {
            //LEA
            return new LEA();
        } else if(opstr == "0000") {
            //BR
            return new BR();
        } else if(opstr == "1100") {
            //JMP or RET
            return new JMP();
        } else if(opstr == "1111") {
            //TRAP
            return new TRAP();
        } else if(opstr == "0100") {
            //JSR or JSRR
            if(instr[11] == 0) {
                //JSR
                return new JSR();
            } else {
                //JSRR
                return new JSRR();
            }
        }
        return nullptr;
    }
}

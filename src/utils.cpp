/*
* utils.cpp ~ random utilities stuff
* author: Ben Staehle
* date: 6/25/23
*/

#include"utils.h"
#include"instr.h"

#include<sstream>
#include<cstdint>
#include<iostream>

namespace utils
{

}

namespace asml
{
    std::string disassemble(uint16_t instr) {
        std::stringstream ret;
        ret << "";
        uint16_t nzp;

        uint16_t opcode = (instr & 0xF000) >> 12;
        
        //get the instruction name
        ret << lc3::table[opcode].name << " ";

        //decode the arguments
        switch (opcode) {
        case 0b0000:
            //BR
            //figure out the nzp bits
            ret << "(";
            nzp = (instr & 0x0E00) >> 9;
            if ((nzp & 0b001)) {
                //negetive
                ret << "n";
            }
            if ((nzp & 0b010) >> 1) {
                //zero
                ret << "z";
            }
            if ((nzp & 0b100) >> 2) {
                //positive
                ret << "p";
            }
            ret << ")";

            //offset
            ret << " ";
            ret << (int16_t)(instr & 0x01FF);
            break;
        case 0b0001:
        case 0b0101:
            //ADD or AND
            //decode dst and src registers
            ret << "R" << (uint16_t)((instr & 0x0E00) >> 9); //dst
            ret << ", " << "R" << (uint16_t)((instr & 0x01C0) >> 6);//src1
            ret << ", ";
            
            //handle immediate and register cases
            if (CHECK_BIT(instr, 11)) {
                //immediate
                ret << (int16_t)lc3::sign_ext_16(instr & 0x1F, 5);
            }
            else {
                //register
                ret << "R" << (uint16_t)(instr & 0x0007);
            }
            break;
        case 0b0110:
        case 0b0111:
            //LDR or STR
            //dst and base
            ret << "R" << (uint16_t)((instr & 0x0E00) >> 9); //dst
            ret << ", " << "R" << (uint16_t)((instr & 0x01C0) >> 6);//base
            ret << ", ";

            //compute offset
            ret << (int16_t)(lc3::sign_ext_16(instr & 0x003F, 6));

            break;
        case 0b0010:
        case 0b0011:
        case 0b1010:
        case 0b1011:
        case 0b1110:
            //LD, ST, LDI, STI, LEA
            ret << "R" << (uint16_t)((instr & 0x0E00) >> 9) << ", "; //dst

            //offset
            ret << (int16_t)lc3::sign_ext_16(instr & 0x01FF, 9);

            break;
        case 0b1100:
            //JMP
            //compute base reg
            ret << "R" << (uint16_t)((instr & 0x01C0) >> 6); //base
            break;
        case 0b1111:
            //TRAP
            //compute trap vector
            ret << std::hex << "0x" << (instr & 0x00FF);
            break;
        case 0b0100:
            //JSR or JSRR
            //check bit 11 to see which
            if (CHECK_BIT(instr, 11)) {
                //JSR
                ret << (int16_t)lc3::sign_ext_16(instr & 0x07FF, 11);
            }
            else {
                //JSRR
                ret << "R" << (uint16_t)((instr & 0x01C0) >> 6); //base
            }
            break;
        default:
            break;
        }

        return ret.str();
    }
}
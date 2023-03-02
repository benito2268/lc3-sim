//lc3 simulator - file utils.h
#pragma once
#include<bitset>
#define WORDSIZE 16

namespace lc3 {
    //forward decl
    struct INSTR;
    typedef std::bitset<WORDSIZE> Register;
    typedef std::bitset<WORDSIZE> Instruction;

    struct util_funcs {
        static void addToPC(std::bitset<16> offset);
        static void addPCTo(Register& result);
        static void setCC(Register& result);
        static INSTR* getInstFromOpCode(std::bitset<16> instr);
    };
    
}
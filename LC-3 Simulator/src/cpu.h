//lc3 simulator - file cpu.h
//defines the inner workings of the CPU
#pragma once
#include<bitset>
#define WORDSIZE 16

#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define RTEMP 8

namespace lc3 {
    class Cpu {
    public:
        static inline std::bitset<WORDSIZE> registers[9];
        static inline std::bitset<WORDSIZE> mem[0xFFFF]; //may cause a compiler error
        static inline std::bitset<3> cc;
        static inline std::bitset<WORDSIZE> pc;
        static inline std::bitset<WORDSIZE> ir;

        void init();
        void init_mem();
        void exec();
        void halt();
    };
}
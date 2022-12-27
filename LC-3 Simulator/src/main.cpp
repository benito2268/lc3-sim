#include<iostream>
#include<bitset>
#include"instr.h"
#include"cpu.h"

int main() {
    lc3::AND test;
    lc3::Cpu::registers[R1] = std::bitset<16>{"1010101010101010"};
    //R0 -> R1 + 1
    test.exec(std::bitset<16>{"0101000001111111"});
    std::cout << lc3::Cpu::registers[R0] << std::endl;
    return 0;
}
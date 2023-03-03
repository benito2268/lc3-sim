#include<iostream>
#include<bitset>
#include<instr.h>
#include<cpu.h>

int main() {
    lc3::Cpu cpu;
    cpu.init();

    while(1) {
        cpu.exec();
    }
    return 0;

}

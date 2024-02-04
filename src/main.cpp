#include<iostream>
#include"cpu.h"
#include"load.h"
#include"instr.h"
#include"resource.h"

int main()
{
	lc3::load_library("C:\\Users\\benst\\lc3libs\\libhalt.lc3"); //libhalt.lc3
	lc3::load_prog("C:\\Users\\benst\\lc3libs\\test.lc3");
	lc3::lc_cpu::init();
	lc3::lc_cpu::run();

	return 0;
}
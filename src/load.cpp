/*
* load.cpp ~ corresponds to load.h
* author: Ben Staehle
* date: 6/20/23
*/

#include"load.h"
#include"cpu.h"
#include"resource.h"
#include"cpp_load_res.h"

#include<iostream>
#include<iomanip>
#include<vector>
#include<iterator>
#include<algorithm>
#include<fstream>
#include<cstdint>
#include<iostream>

namespace lc3
{
	void load_prog(std::string filename) {
		std::ifstream in(filename, std::ios::binary);
		bool is_org = true;
		uint16_t addr;

		char c[2];
		if (in.good()) {
			while (in.read(c, 2*sizeof(char))) {
				if (is_org) {
					addr = static_cast<uint16_t>(c[0] << 8) | static_cast<uint16_t>(c[1] & 0x00FF);
					//set the program counter to the origin address
					lc_cpu::pc = addr;
					is_org = false;
				}
				else {
					lc_cpu::mem[addr] = static_cast<uint16_t>(c[0] << 8) | static_cast<uint16_t>(c[1] & 0x00FF);
					addr++;
				}
			}
		}

		in.close();
	}

	void load_library(std::string filename) {
		std::ifstream in(filename, std::ios::binary);
		bool is_org = true;
		uint16_t addr;

		char c[2];
		if (in.good()) {
			while (in.read(c, 2*sizeof(char))) {
				if (is_org) {
					//start at the origin address
					addr = static_cast<uint16_t>(c[0] << 8) | static_cast<uint16_t>(c[1] & 0x00FF);
					is_org = false;
				}
				else {
					//std::cout << std::hex  << (uint16_t)(c[0] << 8) << " " << std::setfill('0') << std::setw(4) << (uint16_t)(c[1]) << std::endl;
					lc_cpu::mem[addr] = (uint16_t)(c[0] << 8) | (uint16_t)(c[1] & 0x00FF);
					addr++;
				}
			}
		}

		in.close();
	}

	void load_library_res(int res_id, int res_name) {
		utils::Resource lib(res_id, res_name);
		uint16_t* data = lib.getResourceAsPtr<uint16_t>();
		uint16_t addr;

		for (int i = 0; i < lib.getResourceSize(); i++) {
			if (i == 0) {
				addr = data[i];
			}
			else {
				lc_cpu::mem[addr] = data[i];
			}
			addr++;
		}
	}

}
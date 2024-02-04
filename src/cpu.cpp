/*
 * cpu.cpp ~ main cpu implementation
 * author = ben staehle
 * date = 5/17/23
 */

#include"cpu.h"
#include"load.h"
#include"instr.h"

#include<cstdlib>
#include<string>
#include<cstdint>
#include<thread>
#include<chrono>

#include<iostream>

/*
* LC3 Memory Map
* --------------
*     0x0000	}
*		~		} Trap Vector Table 
*	  0x00FF	}
* --------------
*	  0x0100	}
*		~		} Interrupt Vector Table
*	  0x01FF	}
* --------------
*	  0x0200	}
*		~		} OS Space / Stack
*	  0x2FFF	}
* ---------------
*	  0x3000	}
*		~		} User Program Space
*	  0xFDFF	}
* ---------------
*	  0xFE00	}
*		~		} IO / Device Registers
*	  0xFFFF	}
* ---------------
* 
* Memory Mapped Registers
* =======================
* Machine Contol Register (16 bits) = 0xFFFE
* 
* Register Fucntions
* ==================
* R6	  = stack pointer
* R7      = holds return values
*/

namespace lc3 
{
	void lc_cpu::init() {
		//at this point registers and memory are zeroed out
		//set bit 15 (clock enable) of MCR
		lc_cpu::mem[MCR] = 0x8000;

		//lc3::load_table(); //load the opcode matrix

		//load trap table
		mem[0x0020] = 0x0400;
		mem[0x0021] = 0x0430;
		mem[0x0022] = 0x0450;
		mem[0x0023] = 0x04A0;
		mem[0x0024] = 0x04E0;
		mem[0x0025] = 0xFD70;

		//finally, set up the debugger
		debugger = std::make_shared<debug::Debugger>();
	}

	void lc_cpu::set_pc(uint16_t imm) {
		lc_cpu::pc = imm;
	}

	void lc_cpu::run() {
		using namespace debug;

		//cpu runs until HALT calls std::exit()
		while (1) {
			//check the state of the debugger
			switch (Debugger::state) {
			case RUN:
				break;
			case STEP:
				Debugger::state = STOP;
				break;
			case SLOW:
				std::this_thread::sleep_for(std::chrono::seconds(2)); //slow down
				break;
			default:
				break;
			}

			debugger->update();

			if (!(Debugger::state == STOP)) {
				ir = mem[pc];
				pc++;

				//decode the opcode
				uint16_t op = (ir & 0xF000) >> 12;

				//make instruction name avaliable to debugger
				std::string name = table[op].name;

				//update the debugger
				debugger->update();

				//do the operation
				table[op].operation();

				//check that the operation has not triggered a HALT
				if (!((mem[MCR] & 0x8000) >> 15)) {
					stop();
				}
			}
		}
	}

	void lc_cpu::stop() {
		std::exit(0);
	}
}
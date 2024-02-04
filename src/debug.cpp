/*
* debug.cpp ~ code for the debugging window
* author: Ben Staehle
* date: 7/13/23
*/

#include"debug.h"
#include"resource.h"
#include"cpp_load_res.h"
#include"gui_component.h"
#include"cpu.h"
#include"utils.h"

#include<iostream>
#include<iomanip>
#include<sstream>

namespace debug
{
	Debugger::Debugger() {
		window = std::make_shared<sf::RenderWindow>(sf::VideoMode(WIDTH, HEIGHT), "LC3 Debugger");

		utils::Resource r(IDR_TTF1, TTF);
		void* rptr = r.getResourceAsVoidPtr();
		std::size_t rsize = r.getResourceSize();

		if (!font.loadFromMemory(rptr, rsize)) {
			std::cout << "error loading debugger data" << std::endl;
			lc3::lc_cpu::stop();
		}

		//title
		title.setFont(font);
		title.setPosition(25, 10);
		title.setString("LC3 Debugger\n=============================================");
		text.push_back(&title);

		//memory window
		memwin_org = lc3::lc_cpu::pc;

		mem_win.setFont(font);
		mem_win.setCharacterSize(24);
		mem_win.setPosition(25, 75);
		mem_win.setString(get_mem_string(memwin_org)); //the windows shows memory at 0x3000 by default
		text.push_back(&mem_win);

		//register window
		reg_win.setFont(font);
		reg_win.setCharacterSize(24);
		reg_win.setPosition(575, 75);
		reg_win.setString(get_reg_string());
		text.push_back(&reg_win);

		//instruction window
		instr_win.setFont(font);
		instr_win.setCharacterSize(32);
		instr_win.setPosition(25, 350);
		instr_win.setString(asml::disassemble(lc3::lc_cpu::ir));
		text.push_back(&instr_win);

		//set up the step button
		stepb = std::make_shared<gui::Button>("STEP", 200, 500, 75, 50);
		stepb->setFillColor(0xBFBFBFFF);
		stepb->setPressFunc([this](void) -> void {
			this->state = STEP;
		});
		buttons.push_back(stepb);

		runb = std::make_shared<gui::Button>("RUN", 300, 500, 60, 50);
		runb->setFillColor(0x3AA600FF);
		runb->setPressFunc([this](void) -> void {
			this->state = RUN;
		});
		buttons.push_back(runb);

		stopb = std::make_shared<gui::Button>("STOP", 380, 500, 75, 50);
		stopb->setFillColor(0xC21700FF);
		stopb->setPressFunc([this](void) -> void {
			this->state = STOP;
		});
		buttons.push_back(stopb);
	}

	void Debugger::update() {
		//check if memory window needs to update
		using c = lc3::lc_cpu;
		if (c::pc > memwin_org + (memwin_rows * memwin_cols) || c::pc < memwin_org) {
			memwin_org = lc3::lc_cpu::pc;
			mem_win.setString(get_mem_string(memwin_org));
		}

		//update the displays
		reg_win.setString(get_reg_string());
		instr_win.setString(asml::disassemble(lc3::lc_cpu::ir));

		render();
	}

	//private helper functions
	void Debugger::render() {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window->close();

				//also stop the computer
				lc3::lc_cpu::stop();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					//check if any of the buttons got pressed
					for (auto& el : buttons) {
						if (el->isMouseOver(event.mouseButton.x, event.mouseButton.y)) {
							el->press();
						}
					}
				}
			}
		}

		window->clear();
		
		//draw all the sprites
		for (auto& el : text) {
			window->draw(*el);
		}

		//draw all the buttons
		for (auto& el : buttons) {
			el->render(*window);
		}

		window->display();
	}

	std::string Debugger::get_reg_string() {
		std::stringstream ret;
		ret << std::hex;

		ret << "PC: " << std::setfill('0') << std::setw(4) << lc3::lc_cpu::pc << "\n";

		//loop through and print each register
		for (int i = 0; i < 8; i++) {
			ret << "R" + std::to_string(i) << ":" << " ";
			ret << "0x" << std::setfill('0') << std::setw(4) << lc3::lc_cpu::registers[i] << "\n";
		}

		ret << "MCR: 0x" << std::setfill('0') << std::setw(4) << lc3::lc_cpu::mem[MCR] << "\n";

		return ret.str();
	}

	std::string Debugger::get_mem_string(int addr) {
		std::stringstream ret;
		ret << std::hex;
		
		//10 columns
		for (int i = 0; i < memwin_rows; i++) {
			ret << addr << ":" << " ";
			//10 rows
			for (int k = 0; k < memwin_cols; k++) {
				ret << std::setfill('0') << std::setw(2);
				ret << ((lc3::lc_cpu::mem[addr + k] & 0xFF00) >> 8);
				ret << " ";
				ret << std::setfill('0') << std::setw(2);
				ret << (lc3::lc_cpu::mem[addr + k] & 0x00FF);
				ret << " ";
			}
			ret << "\n";
			addr += memwin_cols;
		}

		return ret.str();
	}

	void Debugger::step() {
		state = STEP;
	}

}
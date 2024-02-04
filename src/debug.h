/*
* debug.h ~ header for the lc-3 debugger
* author: Ben Staehle
* date: 6/23/23
*/
#pragma once

#include<memory>
#include<string>
#include<SFML/Graphics.hpp>

namespace gui { class Button; }

namespace debug
{
    enum State {RUN, STOP, SLOW, STEP};

	class Debugger
	{
    public:
        const int WIDTH = 800;
        const int HEIGHT = 600;

        static inline State state = STOP;

        static inline const int memwin_cols = 5;
        static inline const int memwin_rows = 10;
        static inline int memwin_org; //first byte in the memory window

        std::shared_ptr<sf::RenderWindow> window;
        sf::Font font;

        sf::Text title;
        sf::Text mem_win;
        sf::Text reg_win;
        sf::Text instr_win;

        std::shared_ptr<gui::Button> stepb;
        std::shared_ptr<gui::Button> runb;
        std::shared_ptr<gui::Button> stopb;

        std::vector<sf::Text*> text;
        std::vector<std::shared_ptr<gui::Button>> buttons;

        Debugger();
        void update();
        void step();

    private:
        //helper functions
        void render();
        std::string get_mem_string(int addr);
        std::string get_reg_string();
	};
}
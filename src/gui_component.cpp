/*
* gui_component.h ~ defines basic gui for the debugger
* author: Ben Staehle
* date: 7/17/23
*/
#pragma once

#include"gui_component.h"
#include"cpp_load_res.h"
#include"resource.h"

#include<iostream>

namespace gui
{
	Button::Button(std::string text, float xPos, float yPos, float xSize, float ySize) {
		this->xPos = xPos;
		this->yPos = yPos;
		this->xSize = xSize;
		this->ySize = ySize;

		this->rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(xSize, ySize));
		this->rect->setPosition(xPos, yPos);

		this->font = std::make_shared<sf::Font>();
		utils::Resource r(IDR_TTF1, TTF);
		void* rptr = r.getResourceAsVoidPtr();
		std::size_t rsize = r.getResourceSize();

		if (!font->loadFromMemory(rptr, rsize)) {
			std::cout << "error loading debugger data" << std::endl;
			std::exit(1);
		}

		this->text = std::make_shared<sf::Text>();
		this->text->setCharacterSize(24);
		this->text->setString(text);
		this->text->setFont(*font);
		this->text->setPosition(xPos + 10, yPos + 10);
	}

	Button::Button(std::string text, float xPos, float yPos) {
		this->xPos = xPos;
		this->yPos = yPos;
		this->xSize = 60;
		this->ySize = 30;

		this->rect = std::make_shared<sf::RectangleShape>(sf::Vector2f(xSize, ySize));
		this->rect->setPosition(xPos, yPos);

		this->font = std::make_shared<sf::Font>();
		utils::Resource r(IDR_TTF1, TTF);
		void* rptr = r.getResourceAsVoidPtr();
		std::size_t rsize = r.getResourceSize();

		if (!font->loadFromMemory(rptr, rsize)) {
			std::cout << "error loading debugger data" << std::endl;
			std::exit(1);
		}

		this->text = std::make_shared<sf::Text>();
		this->text->setCharacterSize(24);
		this->text->setString(text);
		this->text->setFont(*font);
		this->text->setPosition(xPos + 10, yPos + 10);
	}

	void Button::press() {
		this->pressFunc();
	}

	void Button::render(sf::RenderWindow& w) {
		w.draw(*rect);
		w.draw(*text);
	}

	void Button::setPressFunc(std::function<void(void)> lambda) {
		this->pressFunc = lambda;
	}

	void Button::setFillColor(unsigned color) {
		this->rect->setFillColor(sf::Color(color));
	}

	bool Button::isMouseOver(float x, float y) {
		if (x >= xPos && x <= (xPos + xSize) &&
			y >= yPos && y <= (yPos + ySize)) {
			return true;
		}
		return false;
	}
}
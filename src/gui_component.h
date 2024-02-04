/*
* gui_component.h ~ defines basic gui for the debugger
* author: Ben Staehle
* date: 7/17/23
*/
#pragma once

#include<memory>
#include<functional>
#include<string>
#include<sfml/Graphics.hpp>
#include<sfml/Window.hpp>

namespace gui
{
	class Button
	{
	private:
		float xPos;
		float yPos;
		float xSize;
		float ySize;

	public:
		Button(std::string text, float xPos, float yPos, float xSize, float ySize);
		Button(std::string text, float xPos, float yPos);

	private:
		std::function<void(void)> pressFunc;

		std::shared_ptr<sf::Text> text;
		std::shared_ptr<sf::Font> font;
		std::shared_ptr<sf::RectangleShape> rect;

	public:
		void render(sf::RenderWindow& w);
		void setPressFunc(std::function<void(void)> lambda);
		void setFillColor(unsigned color);
		void press();
		bool isMouseOver(float x, float y);
	};
}

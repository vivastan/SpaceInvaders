#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <SFML/Graphics.hpp>
#include <string>

class MyWindow {
public:
	MyWindow() {
		Set("Space Invaders", sf::Vector2u(800, 800));
	}

	~MyWindow() {
		Destroy();
	}

	void clear() {
		window.clear(sf::Color::Black);
	}
	void show() {
		window.display();
	}

	void update() {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				open = false;
			}
		}
	}

	bool isOpen() const {
		return open;
	}

	sf::Vector2u getSize() {
		return size;
	}

	void draw(sf::Drawable& d) {
		window.draw(d);
	}

	sf::RenderWindow &getWindow() {
		return window;
	}

private:
	std::string title;
	sf::RenderWindow window;
	sf::Vector2u size;
	bool open;

	void Create() {
		window.create(sf::VideoMode(size.x, size.y), title);
	}
	void Destroy() {
		window.close();
	}

	void Set(std::string _title, sf::Vector2u _size) {
		title = _title;
		size = _size;
		open = true;
		Create();
	}
};

#endif // !MYWINDOW_H
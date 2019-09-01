#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <SDL_ttf.h>
#include <vector>

#include "Object.h"
#include "MultipleObjects.h"
#include "Character.h"
#include "Guns.h"
#include "Hero.h"
#include "MagicCircles.h"
#include "Enemies.h"


class Console
{
	TTF_Font *font;
	std::string text;

	void create_text(SDL_Renderer* renderer, const char* text ="");

	SDL_Texture* texture_;
	SDL_Rect *self_rect;


	bool updated;
	bool visible;
    

    //commands
	template <class T> void add(std::vector<std::string> strings, std::vector<T> objects);
	template <class T> void list(std::vector<std::string> strings, std::vector<T> objects);
	template <class T> void erase(std::vector<std::string> strings, std::vector<T> objects);
	template <class T> void pop(std::vector<std::string> strings, std::vector<T> objects);



public:
	Console();

	bool create_font(const char* path, unsigned size);

	void place_console(int x, int y);

	void enable();
	void disable();
	bool is_enabled();

	void add_text(const char* text);

	void shortcuts(const SDL_Keycode key);


	void interpreter(std::vector<MultipleObjects*> objects);


	void render(SDL_Renderer* renderer);
	~Console();
};




#endif


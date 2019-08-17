#pragma once
#include "MultipleObjects.h"
class MagicCircles 
{
	std::vector<Circle*> magicCircles;
	SDL_Color color;
	unsigned int default_radius;
	SDL_Rect map_rect;
	unsigned int thickness;

public:
	MagicCircles(SDL_Rect map_rect, unsigned default_radius , unsigned thickness = 1);

	void set_circle_radius(unsigned int index,unsigned int radius);
	void set_circle_radius(unsigned int radius);

	void set_color(SDL_Color color);
	void set_thickness(unsigned thickness);

	void move(unsigned int index, unsigned int velocity, Direction dx, Direction dy, float delta);
	void place(unsigned index, int center_x, int center_y);

	Circle* get_circle(unsigned int index);
	void erase_circle(unsigned int index);
	void append_circle(Circle circle);
	void append_circle(int centerx, int centery);
	
	void pop_circle();

	void draw(SDL_Renderer* renderer);

	virtual ~MagicCircles();
};


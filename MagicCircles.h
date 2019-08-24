#ifndef MAGICCIRCLES_H
#define MAGICCIRCLES_H


#include "Object.h"
#include <vector>
class MagicCircles 
{
	std::vector<Circle*> magicCircles;
	SDL_Color color;
	unsigned int default_radius;
	SDL_Rect map_rect;
	unsigned int thickness;
	std::vector<std::pair<int, float>> time_stamps;
	float passed_time, life_time;
   

public:
	MagicCircles(SDL_Rect map_rect, unsigned default_radius , unsigned thickness = 1, float life_time = -1.f);

	void set_circle_radius(unsigned int index,unsigned int radius);
	void set_circle_radius(unsigned int radius);

	void set_color(SDL_Color color);
	void set_thickness(unsigned thickness);
	//minus values for infinity
	void set_lifetime(float life_time);

	void move(unsigned int index, int velocity, Direction dx, Direction dy, float delta);
	void place(unsigned index, int center_x, int center_y);

	std::vector<int> collision_list(SDL_Rect* rect);
	std::vector<int> collision_list(Circle* circle2);


	const unsigned get_size();
	Circle* get_circle(unsigned int index);
	void erase_circle(unsigned int index);
	void append_circle(Circle circle);
	void append_circle(int centerx, int centery);

	void add_time_stamp(int index);
	void erase_time_stamp(int index);

	void pop_circle();

	void draw( SDL_Rect *camera, SDL_Renderer* renderer, float delta = NULL);

	virtual ~MagicCircles();
};


#endif

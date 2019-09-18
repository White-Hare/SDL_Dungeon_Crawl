#ifndef ENEMIES_H
#define ENEMIES_H


#include "Character.h"
#include "MultipleObjects.h"
#include "Guns.h"


class Enemies :
	public Character
{
	int velocity;
	
    std::vector<std::pair<SDL_Rect*, Direction*>> locations_;



public:
	Enemies(const char* ID, SDL_Rect map_rect, int velocity = 0);


    //void function(SDL_Rect* self_rect, Direction* direction, int velocity,float delta, std::vector<SDL_Rect*> collided_rects, SDL_Rect* target)
	void behaviour(void (function)(SDL_Rect* self_rect, Direction*, int, float, std::vector<SDL_Rect*>, SDL_Rect* target), std::vector<Object*> objects, std::vector<MultipleObjects*> multiple_objects, std::vector<Guns*> bullets, float delta, SDL_Rect* target = nullptr);
	void set_velocity(int velocity);


	void add_enemy(SDL_Rect rect, Direction direction = RIGHT);
	void add_enemy(int x, int y, Direction direction = RIGHT);
	void erase_enemy(int index);
	void pop_enemy();

	SDL_Rect* get_rect(unsigned index);

	const unsigned get_size();

	std::vector<int> collision_list(SDL_Rect* rect2);
	std::vector<int> collision_list(Circle* circle);


	void place(unsigned index, int x, int y, bool place_in_map = true);
	void move(unsigned index, int dx, int dy, float delta, bool place_in_map = true);


	void render(SDL_Rect *camera, SDL_Renderer* renderer, float delta, bool flip_at_turn = true);

	~Enemies();
};


#endif

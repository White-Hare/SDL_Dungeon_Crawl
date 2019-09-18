#ifndef HERO_H
#define HERO_H


#include "Guns.h"
#include "Enemies.h"

class Hero :
	public Character
{
	Direction direction_;
	int velocity;
	float firing_time;
	
     

public:
	Hero(const char* ID, SDL_Rect map_rect, const int velocity);

	void controller(const Uint8* keystates, float delta, std::vector<Object*> objects, std::vector<MultipleObjects*> multiple_objects);
	void gun_controller(Guns* gun, void (function)(SDL_Rect* rect, Direction direction, float delta),  std::vector<Object*> objects, std::vector<Enemies*> enemies, float delta, const Uint8* keystate);


	void render_hero( SDL_Rect *camera, SDL_Renderer* renderer, float delta);

	~Hero();
};



#endif

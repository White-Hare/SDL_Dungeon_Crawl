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
	
    std::vector<std::pair<int, int>> frame_capes;
	int *current_frame_pair;
    
     

public:
	Hero(SDL_Rect map_rect, const int velocity);

	void contoller(const Uint8* keystates, float delta, std::vector<Object*> objects, std::vector<MultipleObjects*> multiple_objects);
	void gun_controller(Guns* gun, void (function)(SDL_Rect* rect, Direction direction, float delta),  std::vector<Object*> objects, std::vector<Enemies*> enemies, float delta, const Uint8* keystate);

    //[0,2],[2,5] ...
	bool assign_frame_sequence(std::vector<std::pair<int, int>> frames);

	void render( SDL_Rect *camera, SDL_Renderer* renderer, float delta) override;

	~Hero();
};



#endif

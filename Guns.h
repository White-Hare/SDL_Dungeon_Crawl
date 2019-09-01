#ifndef  GUNS_H
#define  GUNS_H


#include "Object.h"
#include <vector>
#include "MultipleObjects.h"


class Guns:
public MultipleObjects
{
	std::vector<Direction> directions_;
	SDL_Rect* boss_rect_;
	float firing_frequency;
public:
	Guns(const char* ID, SDL_Rect map_rect, Object* boss);
	Guns(const char* ID, SDL_Rect map_rect, SDL_Rect* boss);

	void change_boss(Object boss);
	void change_boss(SDL_Rect* boss);

	void set_firing_frequency(float frequency);
	const float get_firing_frequency();

	
    

	void add_bullet();
	void add_bullet(Direction direction);

	void erase_rect(int index) override;

    //function -> void function(SDL_Rect pos*, Direction direction, float delta)
	void behavior(void (function)(SDL_Rect*, Direction, float), float delta);

	void render( SDL_Rect * camera, SDL_Renderer* renderer, SDL_Rect* clip = nullptr) override;
	void render( SDL_Rect * camera, SDL_Renderer* renderer, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip = nullptr) override;

	~Guns();
};


#endif

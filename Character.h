#ifndef CHARACTERS_H
#define CHARACTERS_H


#include "Object.h"
class Character :
	public Object
{
protected:
	unsigned number_of_frames;
	unsigned current_frame;
	float animation_frequency;
	float animation_time;

	SDL_Rect* frames;

public:
	Character(SDL_Rect map_rect);

	void create_animation(float animation_frequency, unsigned rows, unsigned columns = 1);

	void render( SDL_Rect *camera, SDL_Renderer* renderer, float delta);
	void render( SDL_Rect *camera, SDL_Renderer* renderer, float delta, SDL_RendererFlip flip, double angle = 0.0, SDL_Point* center = nullptr);//SDL_FLIP_NONE for default

	~Character();
};


#endif

#ifndef CHARACTERS_H
#define CHARACTERS_H


#include "Object.h"

#include <vector>

class Character :
	public Object
{
protected:
	unsigned number_of_frames;
	unsigned current_frame;
	float animation_frequency;
	float animation_time;

	std::vector<std::pair<int, int>> frame_capes;
	int *current_frame_pair;

	SDL_Rect* frames;


public:
	Character(const char* ID, SDL_Rect map_rect);

	void create_animation(float animation_frequency, unsigned rows, unsigned columns = 1);

	bool change_current_frame_pair(unsigned first, unsigned last);
	bool change_current_frame_pair(unsigned sequence_number);

	//[0,2],[2,5] ...
	bool assign_frame_sequence(std::vector<std::pair<int, int>> frames);

	virtual void render( SDL_Rect *camera, SDL_Renderer* renderer, float delta);
	virtual void render( SDL_Rect *camera, SDL_Renderer* renderer, float delta, SDL_RendererFlip flip, double angle = 0.0, SDL_Point* center = nullptr);//SDL_FLIP_NONE for default

	~Character();
};


#endif

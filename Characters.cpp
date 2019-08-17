#include "Character.h"



Character::Character(SDL_Rect map_rect):Object(map_rect)
{
	frames = nullptr;
	number_of_frames = 0;
	current_frame = 0;
	animation_frequency = 0;
}


void Character::create_animation(unsigned number_of_frames, float animation_frequency)
{
	this->animation_frequency = animation_frequency;
	this->number_of_frames = number_of_frames;

	this->self_rect->w /= number_of_frames;
	this->frames = new SDL_Rect[number_of_frames];

	for (int i = 0; i < number_of_frames; i++)
		this->frames[i] = SDL_Rect{ i * self_rect->w, 0, self_rect->w, self_rect->h };
}



void Character::render(SDL_Renderer* renderer, float delta)
{
	animation_time += delta;
    
	SDL_RenderCopy(renderer, this->texture_, &this->frames[current_frame], this->self_rect);

	if (animation_time > animation_frequency) {
		current_frame++;
		current_frame %= number_of_frames;
		animation_time = 0;
	}
}

void Character::render(SDL_Renderer* renderer, float delta,SDL_RendererFlip flip, double angle, SDL_Point* center)
{
	animation_time += delta;

	SDL_RenderCopy(renderer, this->texture_, &this->frames[current_frame], this->self_rect);

	if (animation_time > animation_frequency) {
		current_frame++;
		current_frame %= number_of_frames;
		animation_time = 0;
	}
}



Character::~Character()
{
	if (frames)
		delete frames;

}

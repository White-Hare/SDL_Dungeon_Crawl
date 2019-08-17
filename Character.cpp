#include "Character.h"


Character::Character(SDL_Rect map_rect):Object(map_rect)
{
	frames = nullptr;
	number_of_frames = 0;
	current_frame = 0;
	animation_frequency = 0;
	animation_time = 0;
}


void Character::create_animation(float animation_frequency, unsigned rows, unsigned columns)
{
	this->animation_frequency = animation_frequency;
	this->number_of_frames = rows * columns;

	this->self_rect->w /= rows;
	this->self_rect->h /= columns;

	this->frames = new SDL_Rect[number_of_frames];


	for (int y = 0; y < columns; y++)
		for (int x = 0; x < rows; x++) {
			this->frames[y*rows + x] = SDL_Rect{ x * self_rect->w, y * self_rect->h, self_rect->w, self_rect->h };
		}
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


void Character::render(SDL_Renderer* renderer, float delta, SDL_RendererFlip flip, double angle, SDL_Point* center)
{
	animation_time += delta;

	SDL_RenderCopyEx(renderer, this->texture_, &this->frames[current_frame], this->self_rect, angle, center, flip);

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

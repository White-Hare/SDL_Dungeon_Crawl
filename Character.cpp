#include "Character.h"
#include <iostream>


Character::Character(const char* ID, SDL_Rect map_rect):Object(ID, map_rect)
{
	frames = nullptr;
	number_of_frames = 0;
	current_frame = 0;
	animation_frequency = 0;
	animation_time = 0;
	current_frame_pair = nullptr;
}


void Character::create_animation(float animation_frequency, unsigned rows, unsigned columns)
{
	this->animation_frequency = animation_frequency;
	this->number_of_frames = rows * columns;

	this->self_rect->w /= rows;
	this->self_rect->h /= columns;

	this->frames = new SDL_Rect[number_of_frames];

	current_frame_pair[0] = 0;
	current_frame_pair[1] = rows * (columns + 1) - 1;

	for (int y = 0; y < columns; y++)
		for (int x = 0; x < rows; x++) {
			this->frames[y*rows + x] = SDL_Rect{ x * self_rect->w, y * self_rect->h, self_rect->w, self_rect->h };
		}
}

bool Character::assign_frame_sequence(std::vector<std::pair<int, int>> frame_capes)
{
	if (frame_capes.size() == 0) {
		std::cout << "Animation haven't been created\n";
		return false;
	}

	try
	{
		for (auto fc : frame_capes)
			if (fc.first > fc.second)
				throw "Order is wrong";

	}
	catch (const char * err) {
		std::cout << err << '\n';
		return false;
	}

	this->frame_capes = frame_capes;
	current_frame_pair[0] = frame_capes[0].first;
	current_frame_pair[1] = frame_capes[0].second;

	return true;
}

bool Character::change_current_frame_pair(unsigned first, unsigned last)
{
	if (first > last) {
		std::cerr << "First number cannot be bigger than last.\n";
		return false;
	}

	if (last >= frame_capes.end()->second) {
		std::cerr << "Last number cannot be longer than sprite length.\n";
		return false;
	}


	this->current_frame_pair[0] = first;
	this->current_frame_pair[1] = last;

	return true;
}

bool Character::change_current_frame_pair(unsigned sequence_number)
{
	if (sequence_number >= this->frame_capes.size()) {
		std::cout << "Sequence number is out of bounds\n";
		return false;
	}

	this->current_frame_pair[0] = frame_capes[sequence_number].first;
	this->current_frame_pair[0] = frame_capes[sequence_number].second;

	return true;
}


void Character::render(SDL_Rect* camera,SDL_Renderer* renderer, float delta)
{
	animation_time += delta;

	if (is_collided(camera)) {
		SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + self_rect->x, -camera->y + self_rect->y, self_rect->w, self_rect->h };
		SDL_RenderCopy(renderer, this->texture_, &this->frames[current_frame + current_frame_pair[0]], tmp_rect);
		delete tmp_rect;
	}

	if (animation_time > animation_frequency) {
		current_frame++;
		current_frame %= current_frame_pair[1] - current_frame_pair[0];
		animation_time = 0;
	}
}


void Character::render(SDL_Rect* camera, SDL_Renderer* renderer, float delta, SDL_RendererFlip flip, double angle, SDL_Point* center)
{
	animation_time += delta;

	if (is_collided(camera)) {
		SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + self_rect->x, -camera->y + self_rect->y, self_rect->w, self_rect->h };
		SDL_RenderCopyEx(renderer, this->texture_, &this->frames[current_frame + current_frame_pair[0]], tmp_rect, angle, center, flip);
		delete tmp_rect;
	}

	if (animation_time > animation_frequency) {
		current_frame++;
		current_frame %= current_frame_pair[1] - current_frame_pair[0];
		animation_time = 0;
	}
}




Character::~Character()
{
	if (frames)
		delete frames;

}

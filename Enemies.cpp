#include "Enemies.h"
#include <iostream>
#include <SDL_image.h>

Enemies::Enemies(const char* ID, SDL_Rect map_rect, int velocity):Character(ID, map_rect)
{
	this->velocity = velocity;
	current_frame_pair = new int[2]{0,0};
	this->map_rect = map_rect;
	this->self_rect = nullptr;
	this->texture_ = nullptr;
}


void Enemies::behaviour(void (function)(SDL_Rect*, Direction*,int, float, std::vector<SDL_Rect*>, SDL_Rect*), std::vector<Object*> objects, std::vector<MultipleObjects*> multiple_objects, std::vector<Guns*> bullets, float delta, SDL_Rect* target)
{
	for (auto l : locations_) {

		std::vector<SDL_Rect*> rects;
		for (auto& m_obj : multiple_objects)
			for (int i : m_obj->collision_list(l.first)) 
				rects.push_back(m_obj->get_rect(i));

			
		
		for (auto& obj : objects)
			if (obj->is_collided(l.first))
				rects.push_back(obj->get_self_rect());


		function(l.first, l.second, this->velocity, delta, rects, target);

		place_in_map(l.first, map_rect);
	}
}

void Enemies::add_enemy(SDL_Rect rect, Direction direction)
{
	locations_.push_back({ new SDL_Rect{rect}, new Direction{direction} });
}

void Enemies::add_enemy(int x, int y, Direction direction)
{
	this->add_enemy({ x, y, this->self_rect->w, this->self_rect->h }, direction);
}

void Enemies::erase_enemy(int index)
{
	locations_.erase(locations_.begin() + index);
}

void Enemies::pop_enemy()
{
	locations_.pop_back();
}

void Enemies::move(unsigned index, int x_axis, int y_axis, float delta, bool pim)
{

	if (x_axis && y_axis) {
		locations_[index].first->x += velocity * x_axis * delta * COS45;
		locations_[index].first->y += velocity * y_axis * delta * SIN45;
	}

	else if (x_axis)
		locations_[index].first->x += velocity * x_axis * delta;

	else if (y_axis)
		locations_[index].first->y += velocity * y_axis* delta;


    if(pim)
	    place_in_map(locations_[index].first, map_rect);
}

void Enemies::place(unsigned index, int x, int y, bool pim)
{
	locations_[index].first->x = x;
	locations_[index].first->y = y;

    if(pim)
    	place_in_map(locations_[index].first, map_rect);
}

SDL_Rect* Enemies::get_rect(unsigned index)
{
	return locations_[index].first;
}

const unsigned Enemies::get_size()
{
	return locations_.size();
}

std::vector<int> Enemies::collision_list(SDL_Rect* rect2)
{
	std::vector<int> collided_objects;

	for (int i = 0; i < locations_.size(); i++)
	{
		if (locations_[i].first == nullptr || rect2 == nullptr)
			continue;

		if (locations_[i].first->x > rect2->x + rect2->w)
			continue;

		if (locations_[i].first->x + locations_[i].first->w < rect2->x)
			continue;

		if (locations_[i].first->y > locations_[i].first->y + rect2->h)
			continue;

		if (locations_[i].first->y + locations_[i].first->h < rect2->y)
			continue;

		collided_objects.push_back(i);
	}
	return collided_objects;
}

std::vector<int> Enemies::collision_list(Circle* circle)
{
	std::vector<int> collided_objects;


	for (int i = 0; i < locations_.size(); i++)
	{
		int closest_x, closest_y;

		//for finding closest x
		if (circle->center_x < locations_[i].first->x)
			closest_x = locations_[i].first->x;
		else if (circle->center_x > locations_[i].first->x + locations_[i].first->w)
			closest_x = locations_[i].first->x + locations_[i].first->w;
		else
			closest_x = circle->center_x;


		//for finding closest y
		if (circle->center_y < locations_[i].first->y)
			closest_y = locations_[i].first->y;
		else if (circle->center_y > locations_[i].first->y + locations_[i].first->h)
			closest_y = locations_[i].first->y + locations_[i].first->h;
		else
			closest_y = circle->center_y;

		const int dx = circle->center_x - closest_x;
		const int dy = circle->center_y - closest_y;


		if (dx*dx + dy * dy < circle->radius*circle->radius)
			collided_objects.push_back(i);
	}

	return collided_objects;

}

bool Enemies::assign_frame_sequence(std::vector<std::pair<int, int>> frame_capes)
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

bool Enemies::change_current_frame_pair(unsigned first, unsigned last)
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

bool Enemies::change_current_frame_pair(unsigned sequence_number)
{
	if (sequence_number >= this->frame_capes.size()) {
		std::cout << "Sequence number is out of bounds\n";
        return false;
	}

	this->current_frame_pair[0] = frame_capes[sequence_number].first;
	this->current_frame_pair[0] = frame_capes[sequence_number].second;

	return true;
}


void Enemies::set_velocity(int velocity)
{
	this->velocity = velocity;
}

void Enemies::render(SDL_Rect* camera, SDL_Renderer* renderer, float delta)
{
	animation_time += delta;

	for (auto l : locations_) {
		SDL_RendererFlip flip = SDL_FLIP_NONE;


		if (*l.second == RIGHT || *l.second == DOWN_RIGHT || *l.second == UP_RIGHT)
			flip = SDL_FLIP_NONE;
		if (*l.second == LEFT || *l.second == UP_LEFT || *l.second == DOWN_LEFT)
			flip = SDL_FLIP_HORIZONTAL;


		SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + l.first->x, -camera->y + l.first->y, l.first->w, l.first->h };
		SDL_RenderCopyEx(renderer, this->texture_, &this->frames[current_frame + current_frame_pair[0]], tmp_rect, 0, nullptr, flip);
		delete tmp_rect;

	}


	if (animation_time > animation_frequency) {
		current_frame++;
		current_frame %= current_frame_pair[1] - current_frame_pair[0];
		animation_time = 0;
	}
}

Enemies::~Enemies()
{
    
}

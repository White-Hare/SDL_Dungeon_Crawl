#include "MultipleObjects.h"
#include <iostream>
#include <complex>


MultipleObjects::MultipleObjects(SDL_Rect map_rect):Object(map_rect)
{
}

void MultipleObjects::append_rect(int x, int y)
{
	rects.push_back(new SDL_Rect{ x, y, self_rect->w,self_rect->h });
}

void MultipleObjects::append_rect(SDL_Rect* rect)
{
	rects.push_back(rect);
}

void MultipleObjects::pop_rect()
{
	rects.pop_back();
}

void MultipleObjects::erase_rect(int index)
{
	try {
		rects.erase(rects.begin() + index);
	}
	catch (const std::out_of_range& oor)
	{
		std::cout << "Already Erased\n";
	}
}

Object* &MultipleObjects::operator[](int i)
{    

    Object* obj = new Object(map_rect);
	obj->assign_texture(texture_, rects[i]);
	this->erase_rect(i);

	return obj;
}

SDL_Rect* MultipleObjects::get_rect(int index)
{
	return rects[index];
}

const unsigned MultipleObjects::get_size()
{
	return rects.size();
}


void MultipleObjects::move(unsigned index, unsigned int velocity, NormalVector x_axis, NormalVector y_axis, float delta)
{


    if(x_axis && y_axis){
		rects[index]->x += x_axis * delta * COS45;
		rects[index]->y += y_axis * delta * SIN45;
    }

    else if(x_axis)
        rects[index]->x += x_axis * delta;

    else if(y_axis)
        rects[index]->y += y_axis * delta;

	if (rects[index]->x < map_rect.x)
		rects[index]->x = map_rect.x;
	if (rects[index]->x + rects[index]->w > map_rect.x + map_rect.w)
		rects[index]->x = map_rect.x + map_rect.w - rects[index]->w;

	if (rects[index]->y < map_rect.y)
		rects[index]->y = map_rect.y;
	if (rects[index]->y + rects[index]->h > map_rect.y + map_rect.h)
		rects[index]->y = map_rect.y + map_rect.h - rects[index]->h;
}

void MultipleObjects::place(unsigned index, int x, int y)
{
	rects[index]->x = x;
	rects[index]->y = y;

	if (rects[index]->x < map_rect.x)
		rects[index]->x = map_rect.x;
	if (rects[index]->x + rects[index]->w > map_rect.x + map_rect.w)
		rects[index]->x = map_rect.x + map_rect.w - rects[index]->w;

	if (rects[index]->y < map_rect.y)
		rects[index]->y = map_rect.y;
	if (rects[index]->y + rects[index]->h > map_rect.y + map_rect.h)
		rects[index]->y = map_rect.y + map_rect.h - rects[index]->h;
}

std::vector<int> MultipleObjects::collision_list(SDL_Rect* rect2)
{
	std::vector<int> collided_objects;

	for (int i = 0; i < rects.size(); i++)
	{
		if (rects[i] == nullptr || rect2 == nullptr)
			continue;

		if (rects[i]->x > rect2->x + rect2->w)
			continue;

		if (rects[i]->x + rects[i]->w < rect2->x)
			continue;

		if (rects[i]->y > rect2->y + rect2->h)
			continue;

		if (rects[i]->y + rects[i]->h < rect2->y)
			continue;

		collided_objects.push_back(i);
	}
	return collided_objects;
}

std::vector<int> MultipleObjects::collision_list(Circle* circle)
{
	std::vector<int> collided_objects;


	for (int i = 0; i < rects.size(); i++)
	{
		int closest_x, closest_y;

		//for finding closest x
		if (circle->center_x < rects[i]->x)
			closest_x = rects[i]->x;
		else if (circle->center_x > rects[i]->x + rects[i]->w)
			closest_x = rects[i]->x + rects[i]->w;
		else
			closest_x = circle->center_x;


		//for finding closest y
		if (circle->center_y < rects[i]->y)
			closest_y = rects[i]->y;
		else if (circle->center_y > rects[i]->y + rects[i]->h)
			closest_y = rects[i]->y + rects[i]->h;
		else
			closest_y = circle->center_y;

		const int dx = circle->center_x - closest_x;
		const int dy = circle->center_y - closest_y;


		if (dx*dx + dy * dy < circle->radius*circle->radius)
			collided_objects.push_back(i);
	}

	return collided_objects;
}

void MultipleObjects::render(SDL_Rect* camera,SDL_Renderer* renderer, SDL_Rect* clip)
{
	for (int i = 0; i < rects.size(); i++) {
		SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + rects[i]->x, -camera->y + rects[i]->y, rects[i]->w, rects[i]->h };
	    SDL_RenderCopy(renderer, this->texture_, clip, tmp_rect);
		delete tmp_rect;
	}
}

void MultipleObjects::render(SDL_Rect* camera,SDL_Renderer* renderer, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip)
{
	for (int i = 0; i < rects.size(); i++) {
		SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + rects[i]->x, -camera->y + rects[i]->y, rects[i]->w, rects[i]->h };
	    SDL_RenderCopyEx(renderer, this->texture_, clip, this->rects[i], angle, center, flip);
		delete tmp_rect;
	}
}

MultipleObjects::~MultipleObjects()
{
	this->rects.clear();
}

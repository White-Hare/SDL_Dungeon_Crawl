#include "MagicCircles.h"
#include <gfx/SDL2_gfxPrimitives.h>

#include <iostream>

MagicCircles::MagicCircles(SDL_Rect map_rect, unsigned default_radius, unsigned thickness, float life_time)
{
	this->color = { 0,0,0,255 };
	this->default_radius = default_radius;
	this->map_rect = map_rect;
	this->thickness = thickness;
	this->life_time = life_time;
	this->passed_time = 0;
}

void MagicCircles::set_color(SDL_Color color)
{
	this->color = *new SDL_Color{ color };
}

void MagicCircles::set_thickness(unsigned thickness)
{
	this->thickness = thickness;
}

void MagicCircles::set_circle_radius(unsigned radius)
{
	this->default_radius = radius;

    for (int i = 0; i < magicCircles.size(); i++)
		magicCircles[i]->radius = radius;
}

void MagicCircles::set_circle_radius(unsigned i, unsigned radius)
{
	magicCircles[i]->radius = radius;
}

void MagicCircles::set_lifetime(float life_time)
{
	this->life_time = life_time;
}

const unsigned MagicCircles::get_size()
{
	return magicCircles.size();
}

Circle* MagicCircles::get_circle(unsigned index)
{
	return  magicCircles[index];
}

void MagicCircles::append_circle(Circle circle)
{
	magicCircles.push_back(new Circle{circle});
	if (life_time >= 0)
		add_time_stamp(magicCircles.size() - 1);
}

void MagicCircles::append_circle(int centerx, int centery)
{
	append_circle({ centerx, centery, this->default_radius });
}

void MagicCircles::pop_circle()
{
	magicCircles.pop_back();
}

void MagicCircles::erase_circle(unsigned index)
{
	try {
		magicCircles.erase(magicCircles.begin() + index);
	}
	catch (const std::out_of_range& oor)
	{
		std::cout << "Already Erased\n";
	}
}

void MagicCircles::add_time_stamp(int index)
{
	time_stamps.push_back(std::pair<int, float>(index, passed_time + life_time));
}

void MagicCircles::erase_time_stamp(int index)
{
	time_stamps.erase(time_stamps.begin() + index);

	for (auto& ts : time_stamps) {
	    if (ts.first >= time_stamps[index].first)
			ts.first--;
	}
}

void MagicCircles::move(unsigned index, unsigned velocity, Direction x_axis, Direction y_axis, float delta)
{
	if (x_axis && y_axis) {
		magicCircles[index]->center_x += x_axis * delta * COS45;
		magicCircles[index]->center_y += y_axis * delta * SIN45;
	}

	else if (x_axis)
		magicCircles[index]->center_x += x_axis * delta;

	else if (y_axis)
		magicCircles[index]->center_y += y_axis * delta;

	if (magicCircles[index]->center_x < map_rect.x)
		magicCircles[index]->center_x = map_rect.x;
	if (magicCircles[index]->center_x + magicCircles[index]->radius * 2 > map_rect.x + map_rect.w)
		magicCircles[index]->center_x = map_rect.x + map_rect.w - magicCircles[index]->radius * 2;

	if (magicCircles[index]->center_y < map_rect.y)
		magicCircles[index]->center_y = map_rect.y;
	if (magicCircles[index]->center_y + magicCircles[index]->radius * 2 > map_rect.y + map_rect.h)
		magicCircles[index]->center_y = map_rect.y + map_rect.h - magicCircles[index]->radius * 2;
}

void MagicCircles::place(unsigned index, int x, int y)
{
	magicCircles[index]->center_x;
	magicCircles[index]->center_y;

	if (magicCircles[index]->center_x < map_rect.x)
		magicCircles[index]->center_x = map_rect.x;
	if (magicCircles[index]->center_x + magicCircles[index]->radius * 2 > map_rect.x + map_rect.w)
		magicCircles[index]->center_x = map_rect.x + map_rect.w - magicCircles[index]->radius * 2;

	if (magicCircles[index]->center_y < map_rect.y)
		magicCircles[index]->center_y = map_rect.y;
	if (magicCircles[index]->center_y + magicCircles[index]->radius * 2 > map_rect.y + map_rect.h)
		magicCircles[index]->center_y = map_rect.y + map_rect.h - magicCircles[index]->radius * 2;
}

std::vector<int> MagicCircles::collision_list(Circle* circle2)
{
	std::vector<int> coll_list;

    for(int i=0; i < magicCircles.size(); i++)
    {
		int dx = magicCircles[i]->center_x - circle2->center_x;
		int dy = magicCircles[i]->center_y - circle2->center_y;
		int sum_of_radii = magicCircles[i]->radius + circle2->radius;

		if (dx*dx + dy * dy < sum_of_radii*sum_of_radii)
			coll_list.push_back(i);
    }

	return  coll_list;
}

std::vector<int> MagicCircles::collision_list(SDL_Rect* rect)
{
	std::vector<int> coll_list;


	for (int i = 0; i < magicCircles.size(); i++)
	{
		int closest_x, closest_y;

		//for finding closest x
		if (magicCircles[i]->center_x < rect->x)
			closest_x = rect->x;
		else if (magicCircles[i]->center_x > rect->x + rect->w)
			closest_x = rect->x + rect->w;
		else
			closest_x = magicCircles[i]->center_x;


		//for finding closest y
		if (magicCircles[i]->center_y < rect->y)
			closest_y = rect->y;
		else if (magicCircles[i]->center_y > rect->y + rect->h)
			closest_y = rect->y + rect->h;
		else
			closest_y = magicCircles[i]->center_y;

		const int dx = magicCircles[i]->center_x - closest_x;
		const int dy = magicCircles[i]->center_y - closest_y;


		if (dx*dx + dy * dy < magicCircles[i]->radius*magicCircles[i]->radius)
			coll_list.push_back(i);
	}

	return coll_list;
}


void MagicCircles::draw(SDL_Rect* camera, SDL_Renderer* renderer, float delta)
{
	if (delta && life_time >= 0) {
		passed_time += delta;

		int i = 0;
		while (i < time_stamps.size()) {
			if (passed_time > time_stamps[i].second) {
				erase_circle(time_stamps[i].first);
				erase_time_stamp(i);
				i--;
			}
			i++;
		}
	}

	SDL_Color *renderer_color = new SDL_Color;
	SDL_GetRenderDrawColor(renderer, &renderer_color->r, &renderer_color->g, &renderer_color->b, &renderer_color->a);

	SDL_SetRenderDrawColor(renderer, 0,0,0,255);


	for (auto& obj : this->magicCircles) {

		float radius_of_star = obj->radius - thickness;
		Sint16 star_x[5];
		Sint16 star_y[5];
		unsigned number_of_sides = 5;


		for (int t = 0; t < this->thickness; t++) {
			circleRGBA(renderer, obj->center_x - camera->x , obj->center_y - camera->y , obj->radius - t, this->color.r, this->color.g, this->color.b, this->color.a);


            for(int i = 0; i<number_of_sides; i++){
					star_x[i] = (radius_of_star - t)* std::cos(static_cast<float>(i) / number_of_sides * PI * 2) + static_cast<float>(obj->center_x) - camera->x;
					star_y[i] = (radius_of_star - t)* std::sin(static_cast<float>(i) / number_of_sides * PI * 2) + static_cast<float>(obj->center_y) - camera->y;

				}

				polygonRGBA(renderer, star_x, star_y, number_of_sides, this->color.r, this->color.g, this->color.b, this->color.a);
			}
		}



	SDL_SetRenderDrawColor(renderer, renderer_color->r, renderer_color->g, renderer_color->b, renderer_color->a);

}


MagicCircles::~MagicCircles()
{
	magicCircles.clear();
}

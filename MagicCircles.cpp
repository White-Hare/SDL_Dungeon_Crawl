#include "MagicCircles.h"
#include <gfx/SDL2_gfxPrimitives.h>

#include <iostream>

MagicCircles::MagicCircles(SDL_Rect map_rect, unsigned default_radius, unsigned thickness)
{
	this->color = { 0,0,0,255 };
	this->default_radius = default_radius;
	this->map_rect = map_rect;
	this->thickness = thickness;
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

Circle* MagicCircles::get_circle(unsigned index)
{
	return  magicCircles[index];
}

void MagicCircles::append_circle(Circle circle)
{
	magicCircles.push_back(new Circle{circle});
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

void MagicCircles::draw(SDL_Renderer* renderer)
{
	SDL_Color *renderer_color = new SDL_Color;
	SDL_GetRenderDrawColor(renderer, &renderer_color->r, &renderer_color->g, &renderer_color->b, &renderer_color->a);

	SDL_SetRenderDrawColor(renderer, 0,0,0,255);


	for (auto& obj : this->magicCircles) {

		float radius_of_star = obj->radius - thickness;
		Sint16 star_x[5];
		Sint16 star_y[5];
		unsigned number_of_sides = 5;


		for (int t = 0; t < this->thickness; t++) {
			circleRGBA(renderer, obj->center_x, obj->center_y, obj->radius - t, this->color.r, this->color.g, this->color.b, this->color.a);


            for(int i = 0; i<number_of_sides; i++){
					star_x[i] = (radius_of_star - t)* std::cos(static_cast<float>(i) / number_of_sides * PI * 2) + static_cast<float>(obj->center_x);
					star_y[i] = (radius_of_star - t)* std::sin(static_cast<float>(i) / number_of_sides * PI * 2) + static_cast<float>(obj->center_y);

				}

				polygonRGBA(renderer, star_x, star_y, number_of_sides, this->color.r, this->color.g, this->color.b, this->color.a);
			}
		}



	SDL_SetRenderDrawColor(renderer, renderer_color->r, renderer_color->g, renderer_color->b, renderer_color->a);

}


MagicCircles::~MagicCircles()
{
}

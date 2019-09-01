#include "Object.h"
#include <SDL_image.h>
#include <iostream>


Object::Object(const char* ID, SDL_Rect map_rect)
{
	self_rect = nullptr;
	this->map_rect = map_rect;
	texture_ = nullptr;
	this->ID = ID;
}

bool Object::load_texture(const char* path, SDL_Renderer* renderer, SDL_Color* color_key)
{
	SDL_Surface* surface = IMG_Load(path);

	if (surface == nullptr)
    {
		std::cout << "FAILED TO LOAD " << path << '\n';
        return false;
    }
	else {
		self_rect = new SDL_Rect{ 0,0,surface->w, surface->h };

		if (color_key != nullptr)
			SDL_SetColorKey(surface, SDL_TRUE,SDL_MapRGB(surface->format, color_key->r, color_key->g, color_key->b));

	    texture_ = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_FreeSurface(surface);

        return true;
	}
}

void Object::assign_texture(SDL_Texture* texture, SDL_Rect* rect)
{
	this->texture_ = texture;
	this->self_rect = new SDL_Rect{ 0,0,rect->w, rect->h };
}

void Object::scale(int width)
{

	this->self_rect->h = width * (self_rect->h / self_rect->w);
	this->self_rect->w = width;
}

void Object::scale(int width, int height)
{
	this->self_rect->w = width;
	this->self_rect->h = height;
}

void Object::move(int velocity, NormalVector x_axis, NormalVector y_axis, float delta)
{
    
	if (x_axis && y_axis) {
		self_rect->x +=velocity * x_axis * delta * COS45;
		self_rect->y +=velocity * y_axis * delta * SIN45;
	}

	else if (x_axis) {
		self_rect->x += velocity * x_axis * delta;
	}
	else if (y_axis)
		self_rect->y +=velocity * y_axis * delta;

	place_in_map(self_rect, map_rect);
}

void Object::place(int x, int y)
{  
	self_rect->x = x;
	self_rect->y = y;

	place_in_map(self_rect, map_rect);
}




void Object::set_colorModulation(SDL_Color color)
{
	SDL_SetTextureColorMod(texture_, color.r, color.g, color.b);
}

//SDL_BLENDMODE_BLEND, SDL_BLENDMODE_NONE
void Object::set_blendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(texture_, blending);
}

//set blend mode first
void Object::set_Alpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(texture_, alpha);
}


SDL_Rect* Object::get_self_rect()
{
	return self_rect;
}


bool Object::is_collided(SDL_Rect* rect2)
{
	if (self_rect == nullptr || rect2 == nullptr)
        return false;

    if(self_rect->x > rect2->x + rect2->w)
        return false;

	if (self_rect->x + self_rect->w < rect2->x)
		return false;

	if (self_rect->y > rect2->y + rect2->h)
		return false;

	if (self_rect->y + self_rect->h < rect2->y)
		return false;

    return true;
}

bool Object::is_collided(Circle* circle)
{
	int closest_x, closest_y;

	//for finding closest x
	if (circle->center_x < self_rect->x)
		closest_x = self_rect->x;
	else if (circle->center_x > self_rect->x + self_rect->w)
		closest_x = self_rect->x + self_rect->w;
	else
		closest_x = circle->center_x;


	//for finding closest y
	if (circle->center_y < self_rect->y)
		closest_y = self_rect->y;
	else if (circle->center_y > self_rect->y + self_rect->h)
		closest_y = self_rect->y + self_rect->h;
	else
		closest_y = circle->center_y;

	const int dx = circle->center_x - closest_x;
	const int dy = circle->center_y - closest_y;


	if (dx*dx + dy * dy < circle->radius*circle->radius)
		return true;


	return false;
}


void Object::render(SDL_Rect* camera, SDL_Renderer* renderer, SDL_Rect* clip)
{
	SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + self_rect->x, -camera->y + self_rect->y, self_rect->w, self_rect->h };
	SDL_RenderCopy(renderer, this->texture_, clip, tmp_rect);
	delete tmp_rect;
}

void Object::render(SDL_Rect* camera, SDL_Renderer* renderer, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip)
{
	SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + self_rect->x, -camera->y + self_rect->y, self_rect->w, self_rect->h };
	SDL_RenderCopyEx(renderer, this->texture_, clip, tmp_rect, angle, center, flip);
	delete tmp_rect;
}



Object::~Object()
{
	SDL_DestroyTexture(texture_);
}

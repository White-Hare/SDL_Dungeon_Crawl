#include "Object.h"
#include <SDL_image.h>
#include <iostream>


Object::Object(SDL_Rect map_rect)
{
	self_rect = nullptr;
	this->map_rect = map_rect;
	texture_ = nullptr;
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

        return true;
	}
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



void Object::render(SDL_Renderer* renderer, SDL_Rect* clip)
{
	SDL_RenderCopy(renderer, this->texture_, clip, this->self_rect);
}


void Object::render(SDL_Renderer* renderer, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip)
{
	SDL_RenderCopyEx(renderer, this->texture_, clip, this->self_rect, angle, center, flip);
}



Object::~Object()
{
}

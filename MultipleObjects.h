#ifndef MULTIPLE_OBJECTS_H
#define MULTIPLE_OBJECTS_H

#include "Object.h"
#include <vector>

class MultipleObjects :
	public Object
{
protected:
	std::vector<SDL_Rect*> rects;

public:
	MultipleObjects(SDL_Rect map_rect);

	void append_rect(int x, int y);
	void append_rect(SDL_Rect* rect);
	void pop_rect();
	void erase_rect(int index);

	//pointer->operator[](index);
	Object* &operator[](int index);

	SDL_Rect* get_rect(int index);

	void move(unsigned index, unsigned int velocity, NormalVector x_axis, NormalVector y_axis, float delta);
	void place(unsigned index, int x, int y);

	std::vector<int> collision_list(SDL_Rect* rect2);
	std::vector<int> collision_list(Circle* circle);


	void render(SDL_Renderer* renderer, SDL_Rect* clip = nullptr) override;
	void render(SDL_Renderer* renderer, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip = nullptr) override;

    virtual ~MultipleObjects();
};



#endif

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
	MultipleObjects(const char* ID, SDL_Rect map_rect);

	void append_rect(int x, int y);
	void append_rect(SDL_Rect* rect);
	void pop_rect();
	virtual void erase_rect(int index);

	//pointer->operator[](index);
	Object* &operator[](int index);

	SDL_Rect* get_rect(int index);
	const unsigned get_size();

	void move(unsigned index, int velocity, NormalVector x_axis, NormalVector y_axis, float delta);
	void place(unsigned index, int x, int y);

	std::vector<int> collision_list(SDL_Rect* rect2);
	std::vector<int> collision_list(Circle* circle);


	void render( SDL_Rect *camera, SDL_Renderer* renderer, SDL_Rect* clip = nullptr) override;
	void render( SDL_Rect *camera, SDL_Renderer* renderer, SDL_RendererFlip flip, double angle, SDL_Point* center, SDL_Rect* clip = nullptr) override;

    virtual ~MultipleObjects();
};



#endif

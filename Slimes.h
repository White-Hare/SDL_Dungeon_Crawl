#ifndef SLIMES_H
#define SLIMES_H

#include "Object.h"
#include <complex>


inline void sliding_slime(SDL_Rect* self_rect, Direction* direction, int velocity,float delta, std::vector<SDL_Rect*> collided_rects, SDL_Rect* target)
{
    
	if (collided_rects.size() != 0) {
		if (*direction == LEFT) {
			*direction = RIGHT;
			self_rect->x = collided_rects[0]->x + collided_rects[0]->w;
		}
		else if (*direction == RIGHT) {
			*direction = LEFT;
			self_rect->x = collided_rects[0]->x - self_rect->w;
		}
	}


	if (*direction == LEFT)
		self_rect->x -= velocity * delta;
	else if (*direction == RIGHT)
		self_rect->x += velocity * delta;
	
}


inline void targeting_slime(SDL_Rect* self_rect, Direction* direction, int velocity, float delta, std::vector<SDL_Rect*> collided_rects, SDL_Rect* target)
{
	float dx = target->x - self_rect->x + target->w/2;
	float dy = target->y - self_rect->y + target->h/2;


    float ax = static_cast<float>(velocity/2) * delta * std::atan2(dx, dy);
    float ay = static_cast<float>(velocity/2)* delta * std::atan2(dy, dx);

	if (ax < 0)
		*direction = LEFT;
	else
		*direction = RIGHT;

	if (collided_rects.size() != 0) {
		ax = -ax;
		ay = -ay;
	}

	self_rect->x += ax;
	self_rect->y += ay;
    

}




#endif

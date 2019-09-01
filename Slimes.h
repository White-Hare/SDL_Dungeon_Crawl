#ifndef SLIMES_H
#define SLIMES_H

#include "Object.h"
#include <complex>


inline void sliding_slime(SDL_Rect* rect, Direction* direction, int velocity,float delta, bool is_collided, SDL_Rect* target)
{
    
	if (is_collided) {
		if (*direction == LEFT) {
			*direction = RIGHT;
			rect->x += 2;
		}
		else if (*direction == RIGHT) {
			*direction = LEFT;
			rect->x -= 2;
		}
	}


	if (*direction == LEFT)
		rect->x -= velocity * delta;
	else if (*direction == RIGHT)
		rect->x += velocity * delta;
	
}


inline void targeting_slime(SDL_Rect* self_rect, Direction* direction, int velocity, float delta, bool is_collided, SDL_Rect* target)
{
	float dx = target->x - self_rect->x + target->w/2;
	float dy = target->y - self_rect->y + target->h/2;


    float ax = static_cast<float>(velocity/2) * delta * std::atan2(dx, dy);
    float ay = static_cast<float>(velocity/2)* delta * std::atan2(dy, dx);

	if (ax < 0)
		*direction = LEFT;
	else
		*direction = RIGHT;

	if (is_collided) {
		ax = -ax;
		ay = -ay;
	}

	self_rect->x += ax;
	self_rect->y += ay;
    

}




#endif

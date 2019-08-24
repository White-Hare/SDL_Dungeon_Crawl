#ifndef SLIMES_H
#define SLIMES_H

#include "Object.h"

inline void slime(SDL_Rect* rect, Direction* direction, int velocity,float delta, bool is_collided)
{
    
	if (is_collided) {
		if (*direction == LEFT) {
			*direction = RIGHT;
			rect->x += 1;
		}
		else if (*direction == RIGHT) {
			*direction = LEFT;
			rect->x -= 1;
		}
	}


	if (*direction == LEFT)
		rect->x -= velocity * delta;
	else if (*direction == RIGHT)
		rect->x += velocity * delta;
	
}









#endif

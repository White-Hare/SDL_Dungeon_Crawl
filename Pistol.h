#ifndef PISTOL_H
#define PISTOL_H

#include <SDL.h>

const int pistol_velocity = 400;

inline void pistol(SDL_Rect* rect, Direction direction, float delta)
{
    if(direction == RIGHT)
	    rect->x += pistol_velocity * delta;
	if (direction == LEFT)
		rect->x -= pistol_velocity * delta;
	if (direction == UP)
		rect->y -= pistol_velocity * delta;
	if (direction == DOWN)
		rect->y += pistol_velocity * delta;
}



#endif

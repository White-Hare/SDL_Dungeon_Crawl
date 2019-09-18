#ifndef DUNGEONCRAWL_H
#define DUNGEONCRAWL_H





#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>
#include <SDL_image.h>


#include "Window.h"
#include "Object.h"
#include "MultipleObjects.h"
#include "Character.h"
#include "Guns.h"
#include "Camera.h"
#include "Enemies.h"
#include "Console.h"



static bool DungeonCrawl_INIT()
{
	//Initiliaze SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		std::cout << "SDL couldn't been initialized. SDL_ERROR::" << SDL_GetError() << '\n';
		return 0;
	}
	if (TTF_Init() == -1)
	{
		std::cout << "TTF couldn't been initialized. TTF_ERROR::" << TTF_GetError() << '\n';
		return  0;
	}

	int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(img_flags) & img_flags))
	{
		std::cout << "IMG_Loader couldn't been initialized. IMG_ERROR::" << IMG_GetError() << '\n';
		return  0;
	}
    return true;
}

#endif

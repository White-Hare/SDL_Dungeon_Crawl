#include "Game.h"
#include "Object.h"
#include "Character.h"
#include "MultipleObjects.h"
#include "Guns.h"


#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer/SDL_mixer.h>
#include <SDL_image.h>

#include <iostream>
#include "Pistol.h"
#include "Hero.h"
#include <ctime>


#define  WIDTH  640
#define  HEIGHT 480

SDL_Rect map_rect{ -WIDTH / 2, -HEIGHT / 2, WIDTH * 2, HEIGHT * 2  };



Game::Game()
{
	hero = nullptr;
	window = nullptr;
	renderer = nullptr;
	event = nullptr;
	running = true;
	last_time = 0;
	delta = 0;
}

bool Game::init(const char* name)
{
	srand(time(NULL));

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

	window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	event = new SDL_Event;

	return 0;
}

bool Game::load_objects()
{

	//Load Object
	MultipleObjects *bricks = new MultipleObjects(map_rect);
	if(!bricks->load_texture("images/brick.png", renderer))
        return false;
    bricks->scale(32, 32);

	for (int x = 0; x < WIDTH; x += 32)
		for (int y = 0; y < HEIGHT; y += 448)
			bricks->append_rect(x, y);

	for (int y = 0; y < HEIGHT; y += 32)
		for (int x = 0; x < WIDTH; x += 608)
			bricks->append_rect(x, y);



	Object *vase = new Object(map_rect);
	if(!vase->load_texture("images/vase.png", renderer))
        return false;	
	vase->scale(32, 32);
	vase->place(100, 140);

    

	hero = new Hero(map_rect, 200);
	if(!hero->load_texture("images/Hero1.png", renderer))
        return false;
	hero->create_animation(0.5, 4, 4);
	hero->assign_frame_sequence(std::vector < std::pair<int, int>>{ {0, 3}, { 4,7 }, {8,11}});
	hero->scale(64,64);
	hero->place(50, 50);



	Guns* pistol_bullet = new Guns(map_rect, hero);
	pistol_bullet->load_texture("images/bullet.png", renderer);
	pistol_bullet->set_firing_frequency(0.5f);



	MagicCircles* dark_magic_circle = new MagicCircles(map_rect, 10, 4);
	dark_magic_circle->set_circle_radius(30);
	dark_magic_circle->set_color({ 20, 20, 20, 210 });
	dark_magic_circle->set_lifetime(0.5f);
	


	objects_.push_back(vase);
    multiple_objects_.push_back(bricks);
	guns_.push_back(pistol_bullet);
	magic_circles_.push_back(dark_magic_circle);



	this->multiple_objects_[0]->place(7, 340, 200);
	this->multiple_objects_[0]->place(9, 100, 200);
	this->multiple_objects_[0]->place(11, 100, 150);



	last_time = SDL_GetTicks() / 1000.f; //for prevent too big delta

	camera = new Camera(map_rect, WIDTH, HEIGHT);

    return true;
}

void Game::timer()
{
	float current_time = SDL_GetTicks() / 1000.f;
	delta = current_time - last_time;
	last_time = current_time;

}

void Game::controlls()
{
	//Controls
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
			running = false;

		if (event->type == SDL_KEYDOWN)
			if (event->key.keysym.sym == SDLK_ESCAPE)
				running = false;

	}

	const Uint8* currentKeyState = SDL_GetKeyboardState(NULL);


	hero->contoller(currentKeyState, delta, objects_, multiple_objects_);
	hero->gun_controller(guns_[0], pistol, objects_, delta, currentKeyState);
}

void Game::move()
{
	if (this->magic_circles_[0]->collision_list(hero->get_self_rect()).size() != 0)
		std::cout << "YOU DIED\n";
	auto& dark_magic_circles = magic_circles_[0];

    while(dark_magic_circles->get_size() < 5)
		dark_magic_circles->append_circle(rand() % WIDTH,rand() % HEIGHT);

	camera->focus(hero);
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);


	for (auto& mc : magic_circles_)
		mc->draw(&this->camera->camera_rect, renderer, delta);

	for (auto& m : multiple_objects_)
		m->render(&this->camera->camera_rect,  renderer);

	for (auto& o : objects_)
		o->render(&this->camera->camera_rect,  renderer);

	for (auto& c : characters_)
		c->render(&this->camera->camera_rect,  renderer, delta);

	for (auto& b : guns_)
		b->render(&this->camera->camera_rect,  renderer, nullptr);
	
	hero->render(&this->camera->camera_rect,  renderer, delta);

	SDL_RenderPresent(renderer);
}

void Game::close()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	delete event;

	for (auto& a : objects_)
		delete a;

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}


Game::~Game()
{
	close();
}

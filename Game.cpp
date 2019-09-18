#include "Game.h"

#include <iostream>
#include <ctime>

#include "Slimes.h"
#include "Pistol.h"


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
	camera = new Camera(map_rect, WIDTH, HEIGHT);
	console = nullptr;
}

bool Game::init(const char* name)
{
	srand(time(NULL));


	if(!DungeonCrawl_INIT())
        return false;

	window = createWindow(window, name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);

	if (window == nullptr)
		return false;

	renderer = window->createRenderer();
	event = new SDL_Event;

	return true;
}

bool Game::load_objects()
{

	//Load Object
	MultipleObjects *bricks = new MultipleObjects("bricks" ,map_rect);
	if(!bricks->load_texture("images/brick.png", renderer))
        return false;
    bricks->scale(32, 32);

	for (int x = 0; x < WIDTH; x += 32)
		for (int y = 0; y < HEIGHT; y += 448)
			bricks->append_rect(x, y);

	for (int y = 0; y < HEIGHT; y += 32)
		for (int x = 0; x < WIDTH; x += 608)
			bricks->append_rect(x, y);

	MultipleObjects *glasses = new MultipleObjects("glasses", map_rect);
	glasses->load_texture("images/glass.png", renderer);
	glasses->scale(32);


	Object *vase = new Object("vase" ,map_rect);
	if(!vase->load_texture("images/vase.png", renderer))
        return false;	
	vase->scale(32, 32);
	vase->place(100, 140);

    

	hero = new Hero("hero", map_rect, 200);
	if(!hero->load_texture("images/Hero1.png", renderer))
        return false;
	hero->create_animation(0.5, 4, 4);
	hero->assign_frame_sequence(std::vector<std::pair<int, int>>{ { 0, 4 }, { 4,8 }, { 8,12 }, { 12,16 }});
    hero->scale(64,64);
	hero->place(50, 50);


	Enemies* slimes = new Enemies("slimes", map_rect, 150);
	slimes->load_texture("images/slime.png", renderer);
	slimes->create_animation(1.f, 2, 2);
	slimes->assign_frame_sequence(std::vector<std::pair<int, int>>{ { 0, 4}});
	slimes->scale(32,32);

	slimes->add_enemy(400, 400);
	slimes->add_enemy(300, 300);

    

	Guns* pistol_bullet = new Guns("pistol" ,map_rect, hero);
	if(!pistol_bullet->load_texture("images/bullet.png", renderer))
        return false;
	pistol_bullet->set_firing_frequency(0.5f);



	MagicCircles* dark_magic_circle = new MagicCircles(map_rect, 10, 4);
	dark_magic_circle->set_circle_radius(30);
	dark_magic_circle->set_color({ 20, 20, 20, 210 });
	dark_magic_circle->set_lifetime(0.5f);
	


	objects_.push_back(vase);
	multiple_objects_.push_back(bricks);
	multiple_objects_.push_back(glasses);
	enemies_.push_back(slimes);
	guns_.push_back(pistol_bullet);
	magic_circles_.push_back(dark_magic_circle);


	this->multiple_objects_[0]->place(7, 340, 200);
	this->multiple_objects_[0]->place(9, 100, 200);
	this->multiple_objects_[0]->place(11, 100, 150);


	console = new Console();
	console->create_font("arial.ttf", 20);


	last_time = SDL_GetTicks() / 1000.f; //for prevent too big delta


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



		if (event->type == SDL_KEYDOWN) {
			if (event->key.keysym.sym == SDLK_ESCAPE)
				running = false;

			if (event->key.keysym.sym == SDLK_BACKQUOTE)
				console->enable();
            
			if (event->key.keysym.sym == SDLK_RETURN && console->is_enabled()) {
				console->interpreter(multiple_objects_);
				console->disable();
			}

			console->shortcuts(event->key.keysym.sym);

		}

		if (event->type == SDL_TEXTINPUT && console->is_enabled())
			console->add_text(event->text.text);


        if (event->button.button == SDL_BUTTON_LEFT)
			enemies_[0]->add_enemy(event->motion.x + this->camera->camera_rect.x, event->motion.y + this->camera->camera_rect.y);



		window->handleWindowEvents(event,renderer);
	}

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);


	hero->controller(currentKeyStates, delta, objects_, multiple_objects_);
	hero->gun_controller(guns_[0], pistol, objects_, enemies_, delta, currentKeyStates);
	
}

void Game::move()
{
	if (this->magic_circles_[0]->collision_list(hero->get_self_rect()).size() != 0)
		//std::cout << "YOU DIED\n";
		;
	auto& dark_magic_circles = magic_circles_[0];

    while(dark_magic_circles->get_size() < 5)
		dark_magic_circles->append_circle(rand() % WIDTH,rand() % HEIGHT);

	camera->focus(hero);


	for (auto& e : enemies_) {
		//e->behaviour(targeting_slime, objects_, multiple_objects_, guns_, delta, hero->get_self_rect());
		e->behaviour(sliding_slime, objects_, multiple_objects_, guns_, delta);
	    for (int i = 0; i < e->get_size(); i++) {
			std::vector<int> coll_list = this->guns_[0]->collision_list(e->get_rect(i));
			if (coll_list.size() != 0) {
				e->erase_enemy(i);
				for (auto c : coll_list) 
					guns_[0]->erase_rect(c);
				
			}
		}
	}


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

	for (auto& e : enemies_)
		e->render(&this->camera->camera_rect, renderer, delta);

	for (auto& b : guns_)
		b->render(&this->camera->camera_rect,  renderer, nullptr);
	
	hero->render(&this->camera->camera_rect,  renderer, delta);

	console->render(renderer);

	SDL_RenderPresent(renderer);
}

void Game::close()
{
	window->free();
    SDL_DestroyRenderer(renderer);

	delete event;


	delete hero;
    delete camera;

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}


Game::~Game()
{
	close();
}

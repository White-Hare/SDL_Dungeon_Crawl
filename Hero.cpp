#include "Hero.h"
#include <iostream>


Hero::Hero(const char* ID, SDL_Rect map_rect, const int velocity):Character(ID, map_rect)
{
	this->velocity = velocity;
	this->direction_ = UP;
	this->current_frame_pair = new int[2]{0,0};
	this->firing_time = 0;
}

void Hero::controller(const Uint8* keystates, float delta, std::vector<Object*> objects, std::vector<MultipleObjects*> multiple_objects)
{

	int dx = 0, dy = 0;

	if (keystates[SDL_SCANCODE_W]) dy -= 1;
	if (keystates[SDL_SCANCODE_S]) dy += 1;
	if (keystates[SDL_SCANCODE_A]) dx -= 1;
	if (keystates[SDL_SCANCODE_D]) dx += 1;

	if (dx == NEGATIVE)
		this->direction_ = LEFT;
	if (dx == POSITIVE)
		this->direction_ = RIGHT;

	if (dy == NEGATIVE)
		this->direction_ = UP;
	if (dy == POSITIVE)
		this->direction_ = DOWN;


	SDL_Rect* hero_rect_before = new SDL_Rect{ *this->get_self_rect() };
	this->move(this->velocity, static_cast<NormalVector>(dx), static_cast<NormalVector>(dy), delta);


	for (auto& m_obj : multiple_objects) {
		if (dx == 0 && dy == 0)
			break;


		if (m_obj->collision_list(this->get_self_rect()).size() == 0)
			continue;


		this->place(hero_rect_before->x, hero_rect_before->y);
		this->move(this->velocity, static_cast<NormalVector>(dx), NONE, delta);


		if (m_obj->collision_list(this->get_self_rect()).size() == 0) {
			dy = NONE;
			continue;
		}
		this->place(hero_rect_before->x, hero_rect_before->y);
		this->move(this->velocity, NONE, static_cast<NormalVector>(dy), delta);


		if (m_obj->collision_list(this->get_self_rect()).size() == 0) {
			dx = NONE;
			continue;
		}
		this->place(hero_rect_before->x, hero_rect_before->y);
		dx = NONE;
		dy = NONE;
	}



	for (auto& obj : objects) {
		if (dx == 0 && dy == 0)
			break;


		if (!obj->is_collided(this->get_self_rect()))
			continue;


		this->place(hero_rect_before->x, hero_rect_before->y);
		this->move(this->velocity, static_cast<NormalVector>(dx), NONE, delta);

		if (!obj->is_collided(this->get_self_rect()))
			continue;



		this->place(hero_rect_before->x, hero_rect_before->y);
		this->move(this->velocity, NONE, static_cast<NormalVector>(dy), delta);


		if (!obj->is_collided(this->get_self_rect()))
			continue;

		this->place(hero_rect_before->x, hero_rect_before->y);
	}
    
}

void Hero::gun_controller(Guns* gun, void function(SDL_Rect* rect, Direction direction, float delta), std::vector<Object*> objects, std::vector<Enemies*> enemies, float delta, const Uint8* keystate)
{
	this->firing_time += delta;

	Direction gun_direction = this->direction_;

	if (keystate[SDL_SCANCODE_SPACE] && firing_time > gun->get_firing_frequency()) {
		gun->add_bullet(gun_direction);
		firing_time = 0;
	}

	gun->behavior(function, delta);

	for (auto& obj : objects) {
		for (int i : gun->collision_list(obj->get_self_rect())) {
			do {
				objects[0]->place(rand() % (640 - 40 - objects[0]->get_self_rect()->w) + 40, rand() % (480 - 40 - objects[0]->get_self_rect()->h) + 40);
			} while (is_collided(obj[0].get_self_rect()));
			gun->erase_rect(i);

		}
	}
}

void Hero::render_hero(SDL_Rect* camera, SDL_Renderer* renderer, float delta)
{
	animation_time += delta;
	SDL_RendererFlip flip = SDL_FLIP_NONE;


    if(this->direction_ == RIGHT)
    {
		current_frame_pair[0] = frame_capes[1].first;
		current_frame_pair[1] = frame_capes[1].second;
		flip = SDL_FLIP_NONE;
    }

	if (this->direction_ == LEFT)
	{
		current_frame_pair[0] = frame_capes[1].first;
		current_frame_pair[1] = frame_capes[1].second;
		flip = SDL_FLIP_HORIZONTAL;
	}

	if (this->direction_ == UP)
	{
		current_frame_pair[0] = frame_capes[2].first;
		current_frame_pair[1] = frame_capes[2].second;
		flip = SDL_FLIP_HORIZONTAL;
	}

	if (this->direction_ == DOWN)
	{
		current_frame_pair[0] = frame_capes[0].first;
		current_frame_pair[1] = frame_capes[0].second;
		flip = SDL_FLIP_NONE;

	}
	render(camera, renderer, delta, flip);
}

Hero::~Hero()
{
	frame_capes.clear();
}

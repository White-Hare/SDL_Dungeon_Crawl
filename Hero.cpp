#include "Hero.h"
#include <iostream>


Hero::Hero(SDL_Rect map_rect, const int velocity):Character(map_rect)
{
	this->velocity = velocity;
	this->direction_ = UP;
	this->current_frame_pair = new int[2];
	this->firing_time = 0;
}

void Hero::contoller(const Uint8* keystates, float delta, std::vector<Object*> objects, std::vector<MultipleObjects*> m_objects)
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


	for (auto& m_obj : m_objects) {

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

void Hero::gun_controller(Guns* gun,void (function)(SDL_Rect* rect, Direction direction, float delta), std::vector<Object*> objects, float delta, const Uint8* keystate)
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
				objects[0]->place(rand() % (map_rect.w - 80) + 40, rand() % (map_rect.h - 80) + 40);
			} while (is_collided(obj[0].get_self_rect()));
			gun->erase_rect(i);

		}
	}
}

bool Hero::assign_frame_sequence(std::vector<std::pair<int, int>> frame_capes)
{
	try
	{
		for (auto fc : frame_capes)
			if (fc.first > fc.second)
				throw "Order is wrong";

	}
	catch (const char * err) {
		std::cout << err << '\n';
        return false;
	}
	
    this->frame_capes = frame_capes;
	current_frame_pair[1] = frame_capes.size();

	return true;
}


void Hero::render(SDL_Rect* camera, SDL_Renderer* renderer, float delta)
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

	SDL_Rect* tmp_rect = new SDL_Rect{ -camera->x + self_rect->x, -camera->y + self_rect->y, self_rect->w, self_rect->h };
	SDL_RenderCopyEx(renderer, this->texture_, &this->frames[current_frame + current_frame_pair[0]], tmp_rect, 0 ,nullptr, flip);
	delete tmp_rect;

	if (animation_time > animation_frequency) {
		current_frame++;
		current_frame %= current_frame_pair[1] - current_frame_pair[0];
		animation_time = 0;
	}
}

Hero::~Hero()
{
	frame_capes.clear();
}

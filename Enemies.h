#ifndef ENEMIES_H
#define ENEMIES_H


#include "Character.h"
#include "MultipleObjects.h"
#include "Guns.h"


class Enemies :
	public Character
{
	int velocity;
	
    std::vector<std::pair<SDL_Rect*, Direction*>> locations_;

	std::vector<std::pair<int, int>> frame_capes;
	int *current_frame_pair;

public:
	Enemies(const char* ID, SDL_Rect map_rect, int velocity = 0);


	void behavior(void (function)(SDL_Rect* self_rect, Direction*, int, float, bool, SDL_Rect* target), std::vector<Object*> objects, std::vector<MultipleObjects*> multiple_objects, std::vector<Guns*> bullets, float delta, SDL_Rect* target = nullptr);
	void set_velocity(int velocity);

	void add_enemy(SDL_Rect rect, Direction direction = RIGHT);
	void add_enemy(int x, int y, Direction direction = RIGHT);
	void erase_enemy(int index);
	void pop_enemy();

	SDL_Rect* get_rect(unsigned index);

	unsigned get_size();

	std::vector<int> collision_list(SDL_Rect* rect2);
	std::vector<int> collision_list(Circle* circle);


	void place(unsigned index, int x, int y);
	void move(unsigned index, int dx, int dy, float delta);
    

        
	bool assign_frame_sequence(std::vector<std::pair<int, int>> frames);
	void render(SDL_Rect *camera, SDL_Renderer* renderer, float delta) override;

	~Enemies();
};


#endif

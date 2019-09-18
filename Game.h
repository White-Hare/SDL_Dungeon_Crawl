#ifndef GAME_H
#define GAME_H

#include <vector>
#include "DungeonCrawl.h"


class Game
{
	Window* window;
	SDL_Renderer* renderer;
	SDL_Event* event;

    bool running;
	float last_time = 0, delta = 0;

    std::vector<Object*> objects_;
	std::vector<MultipleObjects*> multiple_objects_;
	std::vector<Character*> characters_;
	std::vector<Enemies*> enemies_;
	std::vector<Guns*> guns_;
	std::vector<MagicCircles*> magic_circles_;

	Hero* hero;
	Camera *camera;
	Console* console;

public:
	Game();

	bool init(const char* name);

	bool is_running() { return running; }

	bool load_objects();
	void timer();
    void controlls();
	void move();
	void render();

	void close();

	~Game();
};

#endif
#include "Game.h"

Game* game = new Game;


int main (int argc, char* args[])
{
	game->init("DungeonOfGods");
    game->load_objects();

	while (game->is_running())
	{
		game->timer();
		game->controlls();
		game->move();
		game->render();
	}

	game->close();


    return 0;
}
#ifndef OBJECT_H 
#define OBJECT_H


#include <SDL.h>


struct Circle
{
	int center_x;
	int center_y;
	unsigned int radius;
};


class Object
{
	SDL_Rect *self_rect, map_rect;
	SDL_Texture* texture_;


public:
	Object(SDL_Rect map_rect);

	bool load_texture(const char* path, SDL_Renderer *renderer,SDL_Color *color_key=nullptr);
	
    void scale(int width);
	void scale(int width, int height);
	void move(int dx, int dy, float delta);
	void place(int x, int y);

	void set_colorModulation(SDL_Color color);
	void set_blendMode(SDL_BlendMode blending);
	void set_Alpha(Uint8 alpha);

	bool is_collided(SDL_Rect rect2);
	bool is_collided(Circle circle);

	virtual void render(SDL_Renderer* renderer, SDL_Rect *clip = nullptr);
	virtual void render(SDL_Renderer* renderer, SDL_RendererFlip flip, double angle = 0, SDL_Point* center = nullptr, SDL_Rect *clip = nullptr);//SDL_FLIP_NONE for default
    
	~Object();
};

#endif

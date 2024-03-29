#ifndef OBJECT_H 
#define OBJECT_H


#include <SDL.h>

const double SIN45 = 0.707106;
const double COS45 = SIN45;

const double PI = 3.14159265359;


struct Circle
{
	int center_x;
	int center_y;
	unsigned int radius;
};

enum NormalVector
{
	POSITIVE=1,
    NEGATIVE=-1,
    NONE=0
};

enum  Direction
{LEFT = 180, RIGHT = 0, UP = 270, DOWN = 90,
UP_LEFT = 225, UP_RIGHT = 135, DOWN_RIGHT = 315, DOWN_LEFT = 45,  
NO_DIRECTION = -1};

class Object
{
protected:
	SDL_Texture* texture_;
	SDL_Rect *self_rect, map_rect;

public:
	const char* ID;


	Object(const char* ID, SDL_Rect map_rect);

	bool load_texture(const char* path, SDL_Renderer *renderer,SDL_Color *color_key=nullptr);
	void assign_texture(SDL_Texture* texture, SDL_Rect* rect);
	
    void scale(int width);
	void scale(int width, int height);
	void move(int velocity, NormalVector x_axis, NormalVector y_axis, float delta, bool place_in_map = true);
	void place(int x, int y, bool place_in_map = true);


	void set_colorModulation(SDL_Color color);
	void set_blendMode(SDL_BlendMode blending);
	void set_Alpha(Uint8 alpha);

	SDL_Rect* get_self_rect();
	bool is_collided(SDL_Rect* rect2);
	bool is_collided(Circle* circle);


	virtual void render( SDL_Rect *camera, SDL_Renderer* renderer, SDL_Rect *clip = nullptr);
	virtual void render( SDL_Rect *camera,SDL_Renderer* renderer, SDL_RendererFlip flip, double angle = 0, SDL_Point* center = nullptr, SDL_Rect *clip = nullptr);//SDL_FLIP_NONE for default
    
	~Object();
};


static void place_in_map(SDL_Rect* rect, SDL_Rect map_rect)
{
	if (rect->x < map_rect.x)
		rect->x = map_rect.x;
	if (rect->x + rect->w > map_rect.x + map_rect.w)
		rect->x = map_rect.x + map_rect.w - rect->w;

	if (rect->y < map_rect.y)
		rect->y = map_rect.y;
	if (rect->y + rect->h > map_rect.y + map_rect.h)
		rect->y = map_rect.y + map_rect.h - rect->h;
}


#endif

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
{LEFT = 180, RIGHT = 0, UP = 270, DOWN = 90, NO_DIRECTION = -1};

class Object
{
protected:
	SDL_Texture* texture_;
	SDL_Rect *self_rect, map_rect;

public:
	


	Object(SDL_Rect map_rect);

	bool load_texture(const char* path, SDL_Renderer *renderer,SDL_Color *color_key=nullptr);
	void assign_texture(SDL_Texture* texture, SDL_Rect* rect);
	
    void scale(int width);
	void scale(int width, int height);
	void move(int velocity, NormalVector x_axis, NormalVector y_axis, float delta);
	void place(int x, int y);

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

#endif

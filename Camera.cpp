#include "Camera.h"



Camera::Camera(SDL_Rect map_rect, unsigned WIDTH, unsigned HEIGHT)
{
	this->map_rect = map_rect;
	this->camera_rect = { 0,0, static_cast<int>(WIDTH), static_cast<int>(HEIGHT)};
}

void Camera::move(unsigned velocity, NormalVector x_axis, NormalVector y_axis, float delta)
{
	if (x_axis && y_axis) {
		camera_rect.x += velocity * x_axis * delta * COS45;
		camera_rect.y += velocity * y_axis * delta * SIN45;
	}

	else if (x_axis) {
		camera_rect.x += velocity * x_axis * delta;
	}
	else if (y_axis)
		camera_rect.y += velocity * y_axis * delta;

	if (camera_rect.x < map_rect.x)
		camera_rect.x = map_rect.x;
	if (camera_rect.x + camera_rect.w > map_rect.x + map_rect.w)
		camera_rect.x = map_rect.x + map_rect.w - camera_rect.w;

	if (camera_rect.y < map_rect.y)
		camera_rect.y = map_rect.y;
	if (camera_rect.y + camera_rect.h > map_rect.y + map_rect.h)
		camera_rect.y = map_rect.y + map_rect.h - camera_rect.h;
}

void Camera::place(int x, int y)
{

	camera_rect.x = x;
	camera_rect.y = y;

	if (camera_rect.x < map_rect.x)
		camera_rect.x = map_rect.x;
	if (camera_rect.x + camera_rect.w > map_rect.x + map_rect.w)
		camera_rect.x = map_rect.x + map_rect.w - camera_rect.w;

	if (camera_rect.y < map_rect.y)
		camera_rect.y = map_rect.y;
	if (camera_rect.y + camera_rect.h > map_rect.y + map_rect.h)
		camera_rect.y = map_rect.y + map_rect.h - camera_rect.h;
}

void Camera::focus(Object* object)
{
	SDL_Rect* rect = object->get_self_rect();

	camera_rect.x = rect->x - camera_rect.w / 2;
	camera_rect.y = rect->y - camera_rect.h / 2;


	if (camera_rect.x < map_rect.x)
		camera_rect.x = map_rect.x;
	if (camera_rect.x + camera_rect.w > map_rect.x + map_rect.w)
		camera_rect.x = map_rect.x + map_rect.w - camera_rect.w;

	if (camera_rect.y < map_rect.y)
		camera_rect.y = map_rect.y;
	if (camera_rect.y + camera_rect.h > map_rect.y + map_rect.h)
		camera_rect.y = map_rect.y + map_rect.h - camera_rect.h;
}

void Camera::resize_camera(unsigned SCREEN_WIDTH, unsigned SCREEN_HEIGHT)
{
	this->camera_rect.w = SCREEN_WIDTH;
	this->camera_rect.h = SCREEN_HEIGHT;
}

bool Camera::is_collided(SDL_Rect* rect)
{
	if ((camera_rect.w || camera_rect.h) || rect == nullptr)
		return false;

	if (camera_rect.x > rect->x + rect->w)
		return false;

	if (camera_rect.x + camera_rect.w < rect->x)
		return false;

	if (camera_rect.y > rect->y + rect->h)
		return false;

	if (camera_rect.y + camera_rect.h < rect->y)
		return false;

	return true;
}


Camera::~Camera()
{
}

#include "Window.h"
#include<iostream>


Window::Window()
{
	this->width = 0;
	this->height = 0;
	this->keyboardFocus = false;
	this->mouseFocus = false;
	this->fullScreen = false;
	this->window = nullptr;
	this->windowID = 0;
	this->shown = false;
	this->minimized = false;
}


bool Window::createWindow(const char* title, int x, int y, int width, int height, Uint32 flags) {
	this->window = SDL_CreateWindow(title, x, y, width, height, SDL_WINDOW_SHOWN | flags);


	if (window == nullptr) {
		std::cout << "Window couldn't been created. SDL_ERROR:: " << SDL_GetError();
	}
	else {
		this->width = width;
		this->height = height;
		this->keyboardFocus = true;
		this->mouseFocus = true;
		this->shown = true;

		this->windowID = SDL_GetWindowID(window);

	}

	return window != nullptr;
}




void Window::handleWindowEvents(SDL_Event* e, SDL_Renderer* renderer)
{
	if (e->window.windowID == windowID) {
		switch (e->window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			this->width = e->window.data1;
			this->height = e->window.data2;
			SDL_RenderPresent(renderer);
			break;

		case SDL_WINDOWEVENT_EXPOSED://SDL_WINDOWEVENT_EXPOSED just means that window was obscured in some way and now is not obscured so we want to repaint the window.
			SDL_RenderPresent(renderer);
			break;

		case SDL_WINDOWEVENT_ENTER://Mouse entered to window
			this->mouseFocus = true;
			break;

		case SDL_WINDOWEVENT_LEAVE:
			this->mouseFocus = false;
			break;

		case SDL_WINDOWEVENT_FOCUS_GAINED: //Window has keyboard focus
			this->keyboardFocus = true;
			break;

		case SDL_WINDOWEVENT_FOCUS_LOST:
			this->keyboardFocus = false;
			break;

		case SDL_WINDOWEVENT_MINIMIZED:
			this->minimized = true;
			std::cout << "hello" << std::endl;
			break;

		case SDL_WINDOWEVENT_MAXIMIZED:
			this->minimized = false;
			break;

		case SDL_WINDOWEVENT_RESTORED:
			this->minimized = false;
			break;

			//Hide on close
		case SDL_WINDOWEVENT_CLOSE:
			SDL_HideWindow(window);
			shown = false;
			break;

		}

		if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_RETURN && SDL_GetModState() & KMOD_LALT) {
			if (!fullScreen) {
				SDL_SetWindowFullscreen(window, SDL_TRUE);
				minimized = false;
				fullScreen = true;
			}
			else {
				SDL_SetWindowFullscreen(window, SDL_FALSE);
				fullScreen = false;
			}
		}
	}
}

void Window::focus()
{
	if (!shown)
		SDL_ShowWindow(window);

	//Move window forward
	SDL_RaiseWindow(window);
}


SDL_Renderer* Window::createRenderer() { return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); }

int Window::getHeight() { return height; }
int Window::getWidth() { return width; }

SDL_Window* Window::getWindow() { return window; }


bool Window::isKeyboardFocused() { return this->keyboardFocus; }
bool Window::isMouseFocused() { return this->mouseFocus; }
bool Window::isMinimized() { return this->minimized; }
bool Window::isShown() { return this->shown; }


void Window::free()
{
	SDL_DestroyWindow(window);
}



Window::~Window()
{
	free();
}

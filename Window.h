#ifndef WINDOW_H
#define WINDOW_H

#include<SDL.h>




class Window
{
	int windowID;
	int width, height;
	SDL_Window* window;
	bool mouseFocus, keyboardFocus, minimized, fullScreen, shown;

public:
	Window();

	bool createWindow(const char* title, int x, int y, int width, int height, Uint32 flags);
	SDL_Renderer* createRenderer();

	//delocate internals
	void free();

	void focus();

	int getWidth();
	int getHeight();


	bool isMouseFocused();
	bool isKeyboardFocused();
	bool isMinimized();
	bool isShown();


	void handleWindowEvents(SDL_Event* e, SDL_Renderer* renderer);

	SDL_Window* getWindow();

	~Window();
};



static Window* createWindow(Window* window, const char* title, int x, int y, int width, int height, Uint32 flags) {
	window = new Window();
	if (!window->createWindow(title, x, y, width, height, flags))
		return nullptr;
	return window;
}


#endif // WINDOW_H

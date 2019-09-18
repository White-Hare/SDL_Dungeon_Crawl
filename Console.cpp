#include "Console.h"
#include <string>
#include <exception>


Console::Console()
{
	text = "";
	font = nullptr;
	texture_ = nullptr;
	updated = false;
	self_rect = new SDL_Rect{ 0,0,0,0 };
	visible = false;
}

bool Console::create_font(const char* path , unsigned size)
{
	this->font = TTF_OpenFont(path, size);
	if (font == nullptr) {
		std::cout << path << "\t Couldn't be found. TTF_ERROR::"<< TTF_GetError() << std::endl;
        return false;
	}
    return false;
}

void Console::create_text(SDL_Renderer* renderer,const char* text)
{
	if (font == nullptr) {
		std::cout << "Font doesn't exist.\n";
		return;
	}


	if (text[0] == 0)
		text = " ";

	SDL_Surface *surface = TTF_RenderText_Shaded(this->font, text, {0,0,0,255}, {200,200,200,100});
    if(surface == nullptr)
    {
		std::cout << "An error occured while rendering text.\n";
        return;
    }

	this->self_rect->w = surface->w;
	this->self_rect->h = surface->h;


	this->texture_ = SDL_CreateTextureFromSurface(renderer, surface);

}

void Console::place_console(int x, int y)
{
	this->self_rect->x = x;
	this->self_rect->y = y;
}

void Console::enable()
{
	this->visible = true;
	SDL_StartTextInput();
}

void Console::disable()
{
	this->visible = false;
	SDL_StopTextInput();
	text = "";
	updated = true;
}

bool Console::is_enabled()
{
	return visible;
}

void Console::shortcuts(const SDL_Keycode key)
{
	
	if (visible) {
		 if (key == SDLK_BACKSPACE && text.size() != 0)
			this->text.pop_back();

		
	    else if (key == SDLK_c && SDL_GetModState() & KMOD_CTRL) 
			SDL_SetClipboardText(this->text.c_str());


		else if (key == SDLK_v && SDL_GetModState() & KMOD_CTRL)
			text += SDL_GetClipboardText();
	
	
		updated = true;
	}

}

void Console::interpreter(std::vector<MultipleObjects*> objects) {
	std::vector<std::string> strings;

	strings.push_back("");

	bool repeat = false;
	for (char t : text)
	{
		if (t == ' ') {
            if(!repeat)
			    strings.push_back("");
			repeat = true;
            continue;
		}
		
	    strings.back() += t;

		repeat = false;
	}



	if (strings[0] == "list")       list(strings, objects);
	else if (strings[0] == "clear") system("cls");
	else if (strings[0] == "add")   add(strings, objects);
    else if (strings[0] == "erase") erase(strings, objects);
	else if (strings[0] == "pop" )  pop(strings, objects);
	else  std::cout << strings[0] << " command doesn't exist\n";
	    
	

}

void Console::add_text(const char* text)
{
	this->text += text;
	updated = true;
}

void Console::render(SDL_Renderer* renderer)
{
	if (visible) {
		if (updated) {
			create_text(renderer, this->text.c_str());
			updated = false;
		}

		SDL_RenderCopy(renderer, this->texture_, nullptr, self_rect);
	}
}




//commands
template <class T>
void Console::add(std::vector<std::string> strings, std::vector<T> objects)
{
	if (strings.size() < 4) {
		std::cout << "Missing variable.\n";
		return;
	}

	bool passed = false;
	for (auto& mo : objects) {
		if (mo->ID == strings[1])
		{
			try {
				mo->append_rect(std::stoi(strings[2]), stoi(strings[3]));
				passed = true;
				break;
			}
			catch (std::exception& e)
			{
				std::cout << "Wrong argument. ERROR::" << e.what() << '\n';
			}
		}
	}
	if (!passed)
		std::cout << strings[1] << " doesn't exist.\n";

}

template <class T>
void Console::list(std::vector<std::string> strings, std::vector<T> objects)
{
	const char *listables[] = { "objects" ,"commands" };

	if (strings.size() == 1) {
		std::cout << "OPTIONS:\n";
		for (const char* l : listables)
			std::cout << l << std::endl;

	}
	else if (strings[1] == listables[0])
		for (auto& o : objects)
			std::cout << o->ID << '\n';
	else if (strings[1] == listables[1])
		std::cout << "add\nlist\nerase\npop_back\nclear\n";

	
		
	std::cout << '\n';
}

template <class T>
void Console::erase(std::vector<std::string> strings, std::vector<T> objects)
{
	if (strings.size() < 3) {
		std::cout << "Missing variable.\n";
		return;
	}

	bool passed = false;
	for (auto& mo : objects) {
		if (mo->ID == strings[1])
		{
			try {
				mo->erase_rect(std::stoi(strings[2]));
				passed = true;
				break;
			}
			catch (std::exception& e)
			{
				std::cout << "Wrong argument. ERROR::" << e.what() << '\n';
			}
		}
	}
	if (!passed)
		std::cout << strings[1] << " doesn't exist.\n";
}

template <class T>
void Console::pop(std::vector<std::string> strings, std::vector<T> objects)
{
	if (strings.size() < 2) {
		std::cout << "Missing variable.\n";
		return;
	}

	bool passed = false;
	for (auto& mo : objects) {
		if (mo->ID == strings[1])
		{
			try {
				mo->pop_rect();
				passed = true;
				break;
			}
			catch (std::exception& e)
			{
				std::cout << "Wrong argument. ERROR::" << e.what() << '\n';
			}
		}
	}
	if (!passed)
		std::cout << strings[1] << " doesn't exist.\n";
}


Console::~Console()
{
	delete &texture_;
	delete self_rect;
	text.clear();
}

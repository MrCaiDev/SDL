#ifndef window_h_
#define window_h_

#include <SDL.h>
#include <error.h>

//Window wrapper class
class Window
{
private:
	SDL_Window* window;
	int WindowID;
	int w, h;
	bool MouseFocus, KeyboardFocus;
	bool fullscreened, shown, minimized;
public:
	SDL_Renderer* rend;
	Window();
	bool Init(const char* title, int x, int y, int w, int h, Uint32 flags);
	void HandleEvent(SDL_Event event);
	void Focus();
	void Clear();
	void Present();
	int GetWidth();
	int GetHeight();
	bool HasMouseFocus();
	bool HasKeyboardFocus();
	bool IsShown();
	bool IsMinimized();
	void free();
};

/*
 * \brief Create an empty window.
 */
Window::Window()
{
	window = NULL;
	WindowID = 0;
	rend = NULL;
	w = 0;
	h = 0;
	MouseFocus = false;
	KeyboardFocus = false;
	fullscreened = false;
	shown = false;
	minimized = false;
 }

/*
 * \brief Create an window.
 * \param title The title of the window, in UTF-8 encoding.
 * \param x The x position of the window, ::SDL_WINDOWPOS_CENTERED, or ::SDL_WINDOWPOS_UNDEFINED.
 * \param y The y position of the window, ::SDL_WINDOWPOS_CENTERED, or ::SDL_WINDOWPOS_UNDEFINED.
 * \param w The width of the window, in screen coordinates.
 * \param h The height of the window, in screen coordinates.
 * \param flags The flags for the window, a mask of any of the following:
 *               ::SDL_WINDOW_FULLSCREEN,    ::SDL_WINDOW_OPENGL,
 *               ::SDL_WINDOW_HIDDEN,        ::SDL_WINDOW_BORDERLESS,
 *               ::SDL_WINDOW_RESIZABLE,     ::SDL_WINDOW_MAXIMIZED,
 *               ::SDL_WINDOW_MINIMIZED,     ::SDL_WINDOW_INPUT_GRABBED,
 *               ::SDL_WINDOW_ALLOW_HIGHDPI, ::SDL_WINDOW_VULKAN
 *               ::SDL_WINDOW_METAL.
 * \return 1 if succeeded, or 0 if failed.
 */
bool Window::Init(const char* title, int x, int y, int w, int h, Uint32 flags)
{
	free();
	//Create a window.
	window = SDL_CreateWindow(title, x, y, w, h, flags);
	if (window == NULL)
	{
		SDL_ReportError("SDL_CreateWindow");
		return 0;
	}
	else
	{
		this->w = w;
		this->h = h;
		MouseFocus = true;
		KeyboardFocus = true;
		//Create a renderer.
		rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (rend == NULL)
		{
			SDL_DestroyWindow(window);
			window = NULL;
			SDL_ReportError("SDL_CreateRenderer");
			return 0;
		}
		else
		{
			SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
			WindowID = SDL_GetWindowID(window);
			shown = true;
			return 1;
		}
	}
}

/*
 * \brief Handle window events.
 * \param event If not NULL, the next event is removed from the queue and stored in that area.
 */
void Window::HandleEvent(SDL_Event event)
{
	if (event.type == SDL_WINDOWEVENT && event.window.windowID == WindowID)
	{
		switch (event.window.event)
		{
			case SDL_WINDOWEVENT_SHOWN:
				shown = true;
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				shown = false;
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				w = event.window.data1;
				h = event.window.data2;
				SDL_RenderPresent(rend);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				SDL_RenderPresent(rend);
				break;
			case SDL_WINDOWEVENT_ENTER:
				MouseFocus = true;
				break;
			case SDL_WINDOWEVENT_LEAVE:
				MouseFocus = false;
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				KeyboardFocus = true;
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				KeyboardFocus = false;
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				minimized = true;
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				minimized = false;
				break;
			case SDL_WINDOWEVENT_RESTORED:
				minimized = false;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				SDL_HideWindow(window);
				break;
		}
	}
	//Press enter to set the window's fullscreen state.
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
	{
		if (fullscreened)
		{
			SDL_SetWindowFullscreen(window, SDL_FALSE);
			fullscreened = false;
		}
		else
		{
			SDL_SetWindowFullscreen(window, SDL_TRUE);
			fullscreened = true;
			minimized = false;
		}
	}
}

/*
 * \brief Grab focus to the window.
 */
void Window::Focus()
{
	if (!shown)
		SDL_ShowWindow(window);
	SDL_RaiseWindow(window);
}

/*
 * \brief Clear the window.
 */
void Window::Clear()
{
	if (!minimized)
	{
		SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
		SDL_RenderClear(rend);
	}
}

/*
 * \brief Present the window.
 */
void Window::Present()
{
	if (!minimized)
	{
		SDL_RenderPresent(rend);
	}
}

/*
 * \brief Get the width of the window.
 * \return The width of the window.
 */
inline int Window::GetWidth()
{
	return w;
}

/*
 * \brief Get the height of the window.
 * \return The height of the window.
 */
inline int Window::GetHeight()
{
	return h;
}

/*
 * \brief Get the mouse focus state of the window.
 * \return The mouse focus state of the window.
 */
inline bool Window::HasMouseFocus()
{
	return MouseFocus;
}

/*
 * \brief Get the keyboard focus state of the window.
 * \return The keyboard focus state of the window.
 */
inline bool Window::HasKeyboardFocus()
{
	return KeyboardFocus;
}

/*
 * \brief Determine if the window is shown.
 * \return 1 if shown, or 0 if not shown.
 */
inline bool Window::IsShown()
{
	return shown;
}

/*
 * \brief Determine if the window has been minimized.
 * \return 1 if minimized, or 0 if not minimized.
 */
inline bool Window::IsMinimized()
{
	return minimized;
}

/*
 * \brief Free the window and its renderer.
 */
void Window::free()
{
	if (window != NULL)
	{
		WindowID = 0;
		SDL_DestroyWindow(window);
		window = NULL;
		SDL_DestroyRenderer(rend);
		rend = NULL;
		w = 0;
		h = 0;
		MouseFocus = false;
		KeyboardFocus = false;
		fullscreened = false;
		shown = false;
		minimized = false;
	}
}


#endif // !window_h_
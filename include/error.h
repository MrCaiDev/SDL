#ifndef error_h_
#define error_h_

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

/*
 * \brief Show the last error message on the console.
 * \param message A string of message which shows the error.
 */
void SDL_ReportError(std::string message)
{
	std::string error = SDL_GetError();
	printf("%s error: %s\n", message.c_str(), error.c_str());
}

/*
 * \brief Show the last error message on the console.
 * \param message A string of message which shows the error.
 */
void TTF_ReportError(std::string message)
{
	std::string error = TTF_GetError();
	printf("%s error: %s\n", message.c_str(), error.c_str());
}

/*
 * \brief Show the last error message on the console.
 * \param message A string of message which shows the error.
 */
void Mix_ReportError(std::string message)
{
	std::string error = Mix_GetError();
	printf("%s error: %s\n", message.c_str(), error.c_str());
}


#endif // !error_h_
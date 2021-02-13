#ifndef timer_h_
#define timer_h_

#include <string>
#include <SDL.h>

//Timer wrapper class
class Timer
{
private:
	bool started;
	bool paused;
public:
	//Two ticks only update their values when the playing state alters.
	int TicksPlaying;
	int TicksNotPlaying;
	Timer();
	~Timer();
	void Start();
	void Stop();
	void Pause();
	void Resume();
	void Reset();
	int GetTime();
	bool IsStarted();
	bool IsPaused();
	std::string WriteTime();
};

/*
 * \brief Create a timer.
 */
Timer::Timer()
{
	TicksPlaying = 0;
	TicksNotPlaying = 0;
	started = 0;
	paused = 1;
}

/*
 * \brief Deallocate a timer.
 */
Timer::~Timer()
{
	Reset();
}

/*
 * \brief Start a timer.
 */
void Timer::Start()
{
	if (!started)
	{
		started = 1;
		paused = 0;
		TicksNotPlaying = SDL_GetTicks(); //The time before is regarded as not-playing when the timer starts.
	}
}

/*
 * \brief Stop a timer. (Unable to resume)
 */
void Timer::Stop()
{
	if (started)
	{
		started = 0;
		if (!paused)
			TicksPlaying = SDL_GetTicks() - TicksNotPlaying; //Update the time of playing when stopped.
		paused = 1;
	}
}

/*
 * \brief Pause the timer. (Able to resume)
 */
void Timer::Pause()
{
	if (started && !paused)
	{
		paused = 1;
		TicksPlaying = SDL_GetTicks() - TicksNotPlaying; //Update the time of playing when paused.
	}
}

/*
 * \brief Resume the timer,
 */
void Timer::Resume()
{
	if (started && paused)
	{
		paused = 0;
		TicksNotPlaying = SDL_GetTicks() - TicksPlaying; //Update the time of not_playing when resumed.
	}
}

/*
 * \brief Reset the timer.
 */
void Timer::Reset()
{
	started = 0;
	paused = 1;
	TicksPlaying = 0;
	TicksNotPlaying = 0;
}

/*
 * \brief Get the current time of a timer.
 * \return the number of milliseconds recorded by the timer.
 */
int Timer::GetTime()
{
	if (started && !paused)
		return SDL_GetTicks() - TicksNotPlaying;
	return TicksPlaying;
}

/*
 * \brief Determine if a timer is started.
 * \return 1 if started, or 0 if stopped.
 */
inline bool Timer::IsStarted()
{
	return started;
}

/*
 * \brief Determine if a timer is paused.
 * \return 1 if paused, or 0 if not.
 */
inline bool Timer::IsPaused()
{
	if (started)
		return paused;
	else
		return 1;
}

/*
 * \brief Write the time in a human readable way.
 * \return A string showing the current time(s) of a timer.
 */
std::string Timer::WriteTime()
{
	std::string string_time;
	int value_time = GetTime() / 1000;
	string_time = std::to_string(value_time);
	return string_time;
}


#endif // !timer_h_
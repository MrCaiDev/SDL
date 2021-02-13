#ifndef fps_h_
#define fps_h_

#include <SDL.h>
#include <timer.h>

//FPS monitor wrapper class
class FPSmonitor
{
private:
	Timer oneframe;
	Timer update;
	bool control;
	int TargetFPS;
	int RealFPS;
public:
	FPSmonitor();
	~FPSmonitor();
	void SetFPS(const int FPS);
	void StartOneFrame();
	void EndOneFrame();
	void Control();
	void ChangeControllingState();
	int GetFPS();
};

/*
 * \brief Create an empty monitor.
 */
FPSmonitor::FPSmonitor()
{
	control = 1;
	TargetFPS = 0;
	RealFPS = 0;
}

/*
 * \brief Deallocate the monitor.
 */
FPSmonitor::~FPSmonitor()
{
	oneframe.~Timer();
	update.~Timer();
	control = 1;
	TargetFPS = 0;
	RealFPS = 0;
}

/*
 * \brief Set the target FPS for the monitor.
 * \param FPS The FPS at which the program should be running.
 */
void FPSmonitor::SetFPS(const int FPS)
{
	TargetFPS = FPS;
}

/*
 * \brief Inform the monitor that a new frame has started.
 */
inline void FPSmonitor::StartOneFrame()
{
	oneframe.Start();
	update.Start();
}

/*
 * \brief Inform the monitor that a frame has ended.
 */
void FPSmonitor::EndOneFrame()
{
	//Update the value of real FPS every 100ms.
	if (update.GetTime() >= 100)
	{
		RealFPS = 1000 / oneframe.GetTime();
		update.Reset();
	}
	//End the recording of one frame.
	oneframe.Reset();
}

/*
 * \brief Control the FPS of a program if required to.
 */
inline void FPSmonitor::Control()
{
	//Compensate for the remaining milliseconds under the circumstance of controlling FPS.
	if (control && oneframe.GetTime() * TargetFPS < 1000)
		SDL_Delay(1000 / TargetFPS - oneframe.GetTime());
}

/*
 * \brief Change whether FPS will be controlled.
 */
inline void FPSmonitor::ChangeControllingState()
{
	control = !control;
}

/*
 * \brief Get current FPS.
 * \return Current FPS.
 */
inline int FPSmonitor::GetFPS()
{
	return RealFPS;
}


#endif // !fps_h_
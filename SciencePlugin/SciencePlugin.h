#pragma once
#pragma comment (lib, "bakkesmod.lib")
#include "bakkesmod\plugin\bakkesmodplugin.h"
#include <fstream>
#include <sstream>
#include "Logger.h"
#include "CommandsManager.h"
#include "Painter.h"

class SciencePlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	Logger logger;
	CommandsManager cmdManager;
	Painter painter;

	RBState oldCarRBState;
	RBState oldBallRBState;

	std::chrono::time_point<std::chrono::steady_clock> last_time;
	std::chrono::time_point<std::chrono::steady_clock> start_time;

public:
	virtual void onLoad();
	virtual void onUnLoad();
	void OnViewportTick();
	void OnSetInput();

	ofstream dump_file;

	bool recordCar;
	bool recordBall;
};
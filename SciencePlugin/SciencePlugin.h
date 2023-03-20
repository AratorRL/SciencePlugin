#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include <fstream>
#include <sstream>
#include <string>

#include "GuiBase.h"
#include "Logger.h"
#include "CommandsManager.h"
#include "Painter.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

class SciencePlugin : public BakkesMod::Plugin::BakkesModPlugin, public SettingsWindowBase
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
	void OnViewportTick();
	void OnSetInput();
	void RenderSettings() override;

	std::ofstream dump_file;

	bool recordCar;
	bool recordBall;
};
#pragma once
#include "Logger.h"
#include "bakkesmod\wrappers\cvarmanagerwrapper.h"
#include "bakkesmod\wrappers\gamewrapper.h"

class CommandsManager
{
public:
	std::shared_ptr<CVarManagerWrapper> cvarManager;
	std::shared_ptr<GameWrapper> gameWrapper;
	Logger logger;

	void addCommands();
};
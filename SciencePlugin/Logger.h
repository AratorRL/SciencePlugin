#pragma once

#include <string>
#include "bakkesmod\plugin\bakkesmodplugin.h"
#include "HelperFunctions.h"

class Logger
{
public:

	void log(std::string s);
	void log(Vector v);
	void log(Rotator r);
	void log(float f);
	void log(bool b);
	void log(int i);
	void log(unsigned long l);

	std::shared_ptr<CVarManagerWrapper> cvarManager;
};
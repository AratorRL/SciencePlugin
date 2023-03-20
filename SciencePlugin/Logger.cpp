#include "pch.h"

#include "Logger.h"

using namespace sp;

void Logger::log(std::string s)
{
	cvarManager->log(s);
}

void Logger::log(Vector v)
{
	cvarManager->log(vector_to_string(v));
}

void Logger::log(Rotator r)
{
	cvarManager->log(rot_to_string(r));
}

void Logger::log(float f)
{
	cvarManager->log(sp::to_string(f));
}

void Logger::log(bool b)
{
	cvarManager->log(to_string(b));
}

void Logger::log(int i)
{
	cvarManager->log(to_string(i));
}

void Logger::log(unsigned long l)
{
	cvarManager->log(to_string(l));
}
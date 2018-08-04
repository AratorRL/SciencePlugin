#pragma once
#include "bakkesmod\wrappers\gamewrapper.h"

#define COLOR_TEXT 255c, 255c, 255c, 255c
#define COLOR_PANEL 64c, 64c, 64c, 192c

class CVarManagerWrapper;

inline constexpr char operator "" c(unsigned long long arg) noexcept
{
	return static_cast< char >(arg);
}

struct Color
{
	char r, g, b, a;
};

class Painter
{
public:
	std::shared_ptr<GameWrapper> gameWrapper;
	std::shared_ptr<CVarManagerWrapper> cvarManager;

	void initDrawables();

private:
	void drawStringAt(CanvasWrapper cw, std::string text, int x, int y, Color col = {255c, 255c, 255c, 255c});
	void drawStringAt(CanvasWrapper cw, std::string text, Vector2 loc, Color col = { 255c, 255c, 255c, 255c });

	void drawPanels(CanvasWrapper cw);
	void drawRBStatePanel(CanvasWrapper cw, std::string title, RBState rbstate, int x, int y, bool recording);
	void drawCarDerivedInfo(CanvasWrapper cw, CarWrapper car, int x, int y);
	void drawYawPlane(CanvasWrapper cw, CarWrapper car, int x, int y, float scale);
	void drawInputPanel(CanvasWrapper cw, CarWrapper car, int x, int y);
};
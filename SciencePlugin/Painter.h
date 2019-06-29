#pragma once
#include "bakkesmod\wrappers\gamewrapper.h"
#include "bakkesmod\wrappers\cvarmanagerwrapper.h"

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

struct Vector2_ {
	int X;
	int Y;
	Vector2_(int x, int y) : X(x), Y(y) {}
	Vector2_ minus(Vector2_ other)
	{
		return{ X - other.X,  Y - other.Y };
	}
	inline Vector2_ operator+(Vector2_ v2) {
		return Vector2_(X + v2.X, Y + v2.Y);
	}
	inline Vector2_ operator*(float s) {
		return Vector2_(X * s, Y * s);
	}
	inline Vector2_ rotate(float theta) {
		float x = cos(theta) * X - sin(theta) * -Y;
		float y = sin(theta) * X + cos(theta) * -Y;
		return Vector2_(x, -y);
	}
};

class Painter
{
public:
	std::shared_ptr<GameWrapper> gameWrapper;
	std::shared_ptr<CVarManagerWrapper> cvarManager;

	void initDrawables();

private:
	void drawStringAt(CanvasWrapper cw, std::string text, int x, int y, Color col = {255c, 255c, 255c, 255c});
	void drawStringAt(CanvasWrapper cw, std::string text, Vector2_ loc, Color col = { 255c, 255c, 255c, 255c });
	void drawLine(CanvasWrapper cw, Vector2_ x, Vector2_ y);

	void drawPanels(CanvasWrapper cw);
	void drawRBStatePanel(CanvasWrapper cw, std::string title, RBState rbstate, int x, int y, bool recording);
	void drawCalculatedCarInfo(CanvasWrapper cw, CarWrapper car, int x, int y);
	void drawYawPlane(CanvasWrapper cw, CarWrapper car, int x, int y, float scale);
	void drawInputPanel(CanvasWrapper cw, CarWrapper car, int x, int y);
};
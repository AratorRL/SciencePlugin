#include "bakkesmod\wrappers\GameEvent\TutorialWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarComponent\VehicleSimWrapper.h"
#include "bakkesmod\wrappers\arraywrapper.h"
#include "bakkesmod\wrappers\wrapperstructs.h"

#include "Painter.h"
#include "HelperFunctions.h"

#define CHAR_WIDTH_PIXELS 7

using namespace sp;


void Painter::initDrawables()
{
	gameWrapper->RegisterDrawable([this](CanvasWrapper cw) {
		if (gameWrapper->IsInOnlineGame() || !(gameWrapper->IsInFreeplay() || gameWrapper->IsInCustomTraining() || gameWrapper->IsInGame())) {
			return;
		}

		if (cvarManager->getCvar("showHUD").getBoolValue())
		{
			this->drawPanels(cw);
		}
	});
}

void Painter::drawPanels(CanvasWrapper cw)
{
	auto players = gameWrapper->GetGameEventAsServer().GetCars();
	if (players.Count() > 0)
	{
		CarWrapper car = players.Get(0);
		RBState rbstate = car.GetRBState();

		if (cvarManager->getCvar("showCarRBState").getBoolValue())
		{
			drawRBStatePanel(cw, "Car RBState", rbstate, 20, 50, cvarManager->getCvar("recordCarInfo").getBoolValue());
		}

		if (cvarManager->getCvar("showCalculatedCarInfo").getBoolValue())
		{
			drawCalculatedCarInfo(cw, car, 1350, 50);
		}

		if (cvarManager->getCvar("showInput").getBoolValue())
		{
			drawInputPanel(cw, car, 20, 550);
		}

		if (cvarManager->getCvar("showYaw").getBoolValue())
		{
			drawYawPlane(cw, car, 1700, 500, 1);
		}
	}

	BallWrapper ball = gameWrapper->GetGameEventAsServer().GetBall();
	if (!ball.IsNull())
	{
		auto ballRBstate = ball.GetRBState();
		if (cvarManager->getCvar("showBallRBState").getBoolValue())
		{
			drawRBStatePanel(cw, "Ball RBState", ballRBstate, 20, 300, cvarManager->getCvar("recordBallInfo").getBoolValue());
		}
	}
}

void Painter::drawStringAt(CanvasWrapper cw, std::string text, int x, int y, Color col)
{
	Vector2 pos = { x , y };
	cw.SetPosition(pos);
	cw.SetColor(col.r, col.g, col.b, col.a);
	cw.DrawString(text);
}

void Painter::drawStringAt(CanvasWrapper cw, std::string text, Vector2_ loc, Color col)
{
	drawStringAt(cw, text, loc.X, loc.Y, col);
}

void Painter::drawLine(CanvasWrapper cw, Vector2_ v1, Vector2_ v2)
{
	Vector2 start = { v1.X, v1.Y };
	Vector2 end = { v2.X, v2.Y };
	cw.DrawLine(start, end);
}

void Painter::drawRBStatePanel(CanvasWrapper cw, std::string title, RBState rbstate, int x, int y, bool recording)
{
	Vector loc = rbstate.Location;
	Vector lin = rbstate.LinearVelocity;
	Quat quat = rbstate.Quaternion;
	Vector ang = rbstate.AngularVelocity;

	int marginLeft = 30;
	int marginTop = 20;
	
	int nameSpacing = 100;
	int vecSpacing = 80;
	int quatSpacing = 120;

	int lineSpacing = 30;

	int width = 620;
	int height = 210;

	Vector2 pos = { x, y };
	cw.SetPosition(pos);
	cw.SetColor(COLOR_PANEL);
	Vector2 box = { width, height };
	cw.FillBox(box);
	cw.SetColor(COLOR_TEXT);
	
	this->drawStringAt(cw, title, x + marginLeft, y + marginTop);

	if (recording)
	{
		this->drawStringAt(cw, "Recording", x + width - 100, y + marginTop, { 255c, 50c, 50c, 255c });
	}

	int currentLine = marginTop + 50;
	
	this->drawStringAt(cw, "Location", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(loc.X, 2), x + marginLeft + nameSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(loc.Y, 2), x + marginLeft + nameSpacing + vecSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(loc.Z, 2), x + marginLeft + nameSpacing + vecSpacing * 2, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Lin. Velocity", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(lin.X, 2), x + marginLeft + nameSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(lin.Y, 2), x + marginLeft + nameSpacing + vecSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(lin.Z, 2), x + marginLeft + nameSpacing + vecSpacing * 2, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Rotation", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(quat.W), x + marginLeft + nameSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(quat.X), x + marginLeft + nameSpacing + quatSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(quat.Y), x + marginLeft + nameSpacing + quatSpacing * 2, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(quat.Z), x + marginLeft + nameSpacing + quatSpacing * 3, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Ang. Velocity", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ang.X, 4), x + marginLeft + nameSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ang.Y, 4), x + marginLeft + nameSpacing + vecSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ang.Z, 4), x + marginLeft + nameSpacing + vecSpacing * 2, y + currentLine);
}

void Painter::drawCalculatedCarInfo(CanvasWrapper cw, CarWrapper car, int x, int y)
{
	RBState rbstate = car.GetRBState();

	Vector loc = rbstate.Location;
	Vector lin = rbstate.LinearVelocity;
	Quat quat = rbstate.Quaternion;
	Vector ang = rbstate.AngularVelocity;
	Rotator rot = car.GetRotation();

	auto horVel = Vector(lin.X, lin.Y, 0);

	Vector up = quatToUp(quat);
	Vector right = quatToRight(quat);
	Vector fwd = quatToFwd(quat);

	auto linLocalFwd = Vector::dot(lin, fwd);
	auto linLocalRight = Vector::dot(lin, right);
	auto linLocalUp = Vector::dot(lin, up);
	Vector linLocal = Vector(linLocalFwd, linLocalRight, linLocalUp);

	Vector angLocal{ Vector::dot(ang, fwd), Vector::dot(ang, right), Vector::dot(ang, up) };

	auto lonSpeed = Vector::dot(horVel, fwd);
	auto latSpeed = Vector::dot(horVel, right);

	auto vsim = car.GetVehicleSim();
	auto wheels = vsim.GetWheels();


	int marginLeft = 30;
	int marginTop = 20;

	int nameSpacing = 125;
	int vecSpacing = 120;
	int quatSpacing = 120;

	int lineSpacing = 30;

	Vector2 pos = { x, y };
	cw.SetPosition(pos);
	cw.SetColor(COLOR_PANEL);
	Vector2 box = { 550, marginTop + 14 * lineSpacing };
	cw.FillBox(box);
	cw.SetColor(COLOR_TEXT);

	this->drawStringAt(cw, "Calculated car info", x + marginLeft, y + marginTop);

	int currentLine = marginTop + 50;

	this->drawStringAt(cw, "Linear speed", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(lin.magnitude(), 4), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Horizontal speed", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(horVel.magnitude(), 4), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Local lin. velocity", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(linLocal.X, 4), x + marginLeft + nameSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(linLocal.Y, 4), x + marginLeft + nameSpacing + vecSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(linLocal.Z, 4), x + marginLeft + nameSpacing + vecSpacing * 2, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Local ang.velocity", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(angLocal.X, 4), x + marginLeft + nameSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(angLocal.Y, 4), x + marginLeft + nameSpacing + vecSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(angLocal.Z, 4), x + marginLeft + nameSpacing + vecSpacing * 2, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Long. speed", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(lonSpeed, 4), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Lat. speed", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(latSpeed, 4), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Rotation fwd", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(fwd.X), x + marginLeft + nameSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(fwd.Y), x + marginLeft + nameSpacing + vecSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(fwd.Z), x + marginLeft + nameSpacing + vecSpacing * 2, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Rotation right", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(right.X), x + marginLeft + nameSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(right.Y), x + marginLeft + nameSpacing + vecSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(right.Z), x + marginLeft + nameSpacing + vecSpacing * 2, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Rotation up", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(up.X), x + marginLeft + nameSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(up.Y), x + marginLeft + nameSpacing + vecSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string_scientific(up.Z), x + marginLeft + nameSpacing + vecSpacing * 2, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Left steer", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(wheels.Get(0).GetSteer2(), 6), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Right steer", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(wheels.Get(1).GetSteer2(), 6), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Rotator", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(rot.Pitch), x + marginLeft + nameSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(rot.Yaw), x + marginLeft + nameSpacing + vecSpacing, y + currentLine);
	this->drawStringAt(cw, sp::to_string(rot.Roll), x + marginLeft + nameSpacing + vecSpacing * 2, y + currentLine);

}

void Painter::drawYawPlane(CanvasWrapper cw, CarWrapper car, int x, int y, float scale)
{
	RBState rbstate = car.GetRBState();

	Vector loc = rbstate.Location;
	Vector lin = rbstate.LinearVelocity;
	Quat quat = rbstate.Quaternion;
	Vector ang = rbstate.AngularVelocity;

	Vector up = quatToUp(quat);
	Vector right = quatToRight(quat);
	Vector fwd = quatToFwd(quat);
	Vector fwdHor(fwd.X, fwd.Y, 0);
	fwdHor.normalize();

	Vector velHor(lin.X, lin.Y, 0);
	if (velHor.magnitude() > 0)
	{
		float fraction = (velHor.magnitude() / 2300) * 100;
		velHor.normalize();
		velHor = velHor * fraction; // scale vector to range 0-100 to fit it in the panel
	}
	Vector2_ velArrow(velHor.Y, -velHor.X);

	Vector2_ offset(x, y);

	int width = 200;
	int height = 200;

	Vector2 pos = { x, y };
	cw.SetPosition(pos);
	cw.SetColor(COLOR_PANEL);
	Vector2 box = { (int)(width * scale), (int)(height * scale) };
	cw.FillBox(box);
	cw.SetColor(COLOR_TEXT);

	Vector2_ center(100, 100);
	Vector2_ axisVer(0, -80);
	Vector2_ axisHor(80, 0);
	Vector2_ orientArrow(0, -90);

	Vector2_ angleLoc(10, 10);
	Vector2_ axisVerLabelLoc(-15, -85);
	Vector2_ axisHorLabelLoc(75, 5);

	// x-axis
	drawLine(cw, offset + center * scale, offset + (center + axisVer) * scale);
	// y-axis
	drawLine(cw, offset + center * scale, offset + (center + axisHor) * scale);

	float cosTheta = Vector::dot(fwdHor, Vector(1, 0, 0));
	float theta = acos(cosTheta);
	if ((Vector::cross(Vector(1, 0, 0), fwdHor)).Z > 0) { theta = -theta; }

	// approximate shape/size of octane
	Vector2_ frontLeft(-42, -73);
	Vector2_ frontRight(42, -73);
	Vector2_ backLeft(-42, 45);
	Vector2_ backRight(42, 45);

	// front and back of car
	drawLine(cw, offset + (center + frontLeft.rotate(theta)) * scale, offset + (center + frontRight.rotate(theta)) * scale);
	drawLine(cw, offset + (center + backLeft.rotate(theta)) * scale, offset + (center + backRight.rotate(theta)) * scale);
	drawLine(cw, offset + (center + frontLeft.rotate(theta)) * scale, offset + (center + backLeft.rotate(theta)) * scale);
	drawLine(cw, offset + (center + frontRight.rotate(theta)) * scale, offset + (center + backRight.rotate(theta)) * scale);

	// fwd arrow
	cw.SetColor(255, 255, 255, 128);
	drawLine(cw, offset + center * scale, offset + (center + orientArrow.rotate(theta)) * scale);

	// velocity arrow
	cw.SetColor(0, 255, 0, 255);
	drawLine(cw, offset + center * scale, offset + (center + velArrow) * scale);

	this->drawStringAt(cw, sp::to_string(theta), offset + angleLoc * scale);
	this->drawStringAt(cw, "x", offset + (center + axisVerLabelLoc) * scale);
	this->drawStringAt(cw, "y", offset + (center + axisHorLabelLoc) * scale);

}

void Painter::drawInputPanel(CanvasWrapper cw, CarWrapper car, int x, int y)
{
	ControllerInput ci = car.GetInput();
	
	int marginLeft = 30;
	int marginTop = 20;

	int nameSpacing = 125;
	int vecSpacing = 120;
	int quatSpacing = 120;

	int lineSpacing = 30;

	Vector2 pos = { x, y };
	cw.SetPosition(pos);
	cw.SetColor(COLOR_PANEL);
	Vector2 box = { 420, 280 };
	cw.FillBox(box);
	cw.SetColor(COLOR_TEXT);

	this->drawStringAt(cw, "Player input", x + marginLeft, y + marginTop);

	int currentLine = marginTop + 50;

	this->drawStringAt(cw, "Throttle", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.Throttle), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Steer", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.Steer), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Pitch", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.Pitch), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Yaw", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.Yaw), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Roll", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.Roll), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Handbrake", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.Handbrake), x + marginLeft + nameSpacing, y + currentLine);

	// second column
	currentLine = marginTop + 50;
	marginLeft += 200;

	this->drawStringAt(cw, "Jump", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.Jump), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Jumped", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.Jumped), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Activate boost", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.ActivateBoost), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Holding boost", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.HoldingBoost), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Dodge forward", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.DodgeForward), x + marginLeft + nameSpacing, y + currentLine);

	currentLine += lineSpacing;

	this->drawStringAt(cw, "Dodge strafe", x + marginLeft, y + currentLine);
	this->drawStringAt(cw, sp::to_string(ci.DodgeStrafe), x + marginLeft + nameSpacing, y + currentLine);
}

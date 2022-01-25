#include "bakkesmod\wrappers\GameEvent\TutorialWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarWrapper.h"
#include "bakkesmod\wrappers\GameObject\BallWrapper.h"
#include "bakkesmod\wrappers\arraywrapper.h"
#include "bakkesmod\wrappers\CVarWrapper.h"
#include "bakkesmod\wrappers\CVarManagerWrapper.h"
#include "utils\parser.h"

#include "SciencePlugin.h"
#include "HelperFunctions.h"

BAKKESMOD_PLUGIN(SciencePlugin, "Science plugin", "1.0", PLUGINTYPE_FREEPLAY)

using namespace sp;

void SciencePlugin::onLoad()
{
	gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", std::bind(&SciencePlugin::OnViewportTick, this));
	gameWrapper->HookEvent("Function TAGame.Car_TA.SetVehicleInput", std::bind(&SciencePlugin::OnSetInput, this));

	logger.cvarManager = this->cvarManager;
	
	cmdManager.cvarManager = this->cvarManager;
	cmdManager.gameWrapper = this->gameWrapper;
	cmdManager.logger = this->logger;
	
	cmdManager.addCommands();

	painter.gameWrapper = this->gameWrapper;
	painter.cvarManager = this->cvarManager;
	
	painter.initDrawables();

	last_time = std::chrono::high_resolution_clock::now();
	start_time = last_time;
	
}

void SciencePlugin::onUnLoad()
{

}


void SciencePlugin::OnViewportTick()
{
	if (gameWrapper->IsInOnlineGame() || !(gameWrapper->IsInFreeplay() || gameWrapper->IsInCustomTraining() || gameWrapper->IsInGame())) {
		return;
	}

	auto now = std::chrono::high_resolution_clock::now();
	auto elapsed = now - last_time;
	auto elapsed_total = now - start_time;
	last_time = now;
	long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
	std::string microseconds_str = std::to_string(microseconds);
	long long timestamp = std::chrono::duration_cast<std::chrono::microseconds>(elapsed_total).count();
	std::string timestamp_str = std::to_string(timestamp);

	if (cvarManager->getCvar("recordBallInfo").getBoolValue())
	{
		bool recordStart = false;
		if (!this->recordBall)
		{
			recordStart = true;
			recordBall = true;
			this->dump_file.open("dump.txt", std::ios::out);
			cvarManager->log("Dump file opened");
		}
		
		ServerWrapper serverGame = gameWrapper->GetGameEventAsServer();
		BallWrapper ball = serverGame.GetBall();
		if (!ball.IsNull())
		{
			auto ballRBstate = ball.GetRBState();
			auto ballLocRB = ballRBstate.Location;
			auto ballLinVelRB = ballRBstate.LinearVelocity;
			auto ballQuatRB = ballRBstate.Quaternion;
			auto ballAngVelRB = ballRBstate.AngularVelocity;

			bool newStuff = !vector_eq(ballLocRB, oldBallRBState.Location) || !vector_eq(ballLinVelRB, oldBallRBState.LinearVelocity) 
				|| !quat_eq(ballQuatRB, oldBallRBState.Quaternion) || !vector_eq(ballAngVelRB, oldBallRBState.AngularVelocity);

			if (newStuff || cvarManager->getCvar("recordEveryTick").getBoolValue() || recordStart)
			{
				if (dump_file.is_open())
				{
					auto output_line = microseconds_str
						+ ", " + vector_to_string(ballLocRB, 2)
						+ ", " + vector_to_string(ballLinVelRB, 2)
						+ ", " + quat_to_string(ballQuatRB)
						+ ", " + vector_to_string(ballAngVelRB, 4)
						;
					dump_file << output_line + "\n";
				}
			}

			oldBallRBState.Location = ballLocRB;
			oldBallRBState.LinearVelocity = ballLinVelRB;
			oldBallRBState.Quaternion = ballQuatRB;
			oldBallRBState.AngularVelocity = ballAngVelRB;
		}

		return;
	}
	else if (this->recordBall)
	{
		this->recordBall = false;
		if (this->dump_file.is_open())
		{
			this->dump_file.close();
			cvarManager->log("dump file closed");
		}
		
	}

	if (cvarManager->getCvar("recordCarInfo").getBoolValue())
	{
		bool recordStart = false;
		if (!this->recordCar)
		{
			recordStart = true;
			recordCar = true;
			this->dump_file.open("dump.txt", std::ios::out);
			cvarManager->log("Dump file opened");
		}

		ServerWrapper game = gameWrapper->GetGameEventAsServer();
		auto car = game.GetGameCar();
		auto rbstate = car.GetRBState();
		//auto changed = rbstate.bNewData;

		auto locRB = rbstate.Location;
		auto linVelRB = rbstate.LinearVelocity;
		auto horVel = Vector(linVelRB.X, linVelRB.Y, 0);
		auto quatRB = rbstate.Quaternion;
		auto angVelRB = rbstate.AngularVelocity;

		Vector up = quatToUp(quatRB);
		Vector right = quatToRight(quatRB);
		Vector fwd = quatToFwd(quatRB);

		auto linLocalFwd = Vector::dot(linVelRB, fwd);
		auto linLocalRight = Vector::dot(linVelRB, right);
		auto linLocalUp = Vector::dot(linVelRB, up);

		Vector linLocal = Vector(linLocalFwd, linLocalRight, linLocalUp);

		auto lonSpeed = Vector::dot(horVel, fwd);
		auto latSpeed = Vector::dot(horVel, right);

		bool newStuff = !vector_eq(locRB, oldCarRBState.Location) || !vector_eq(linVelRB, oldCarRBState.LinearVelocity) 
			|| !quat_eq(quatRB, oldCarRBState.Quaternion) || !vector_eq(angVelRB, oldCarRBState.AngularVelocity);

		if (newStuff || cvarManager->getCvar("recordEveryTick").getBoolValue() || recordStart)
		{
			if (dump_file.is_open())
			{
				auto output_line = microseconds_str
					+ ", " + vector_to_string(locRB, 2)
					+ ", " + vector_to_string(linVelRB, 2)
					+ ", " + quat_to_string(quatRB, 20)
					+ ", " + vector_to_string(angVelRB, 4)
					;
				dump_file << output_line + "\n";
			}
		}

		oldCarRBState.Location = locRB;
		oldCarRBState.LinearVelocity = linVelRB;
		oldCarRBState.Quaternion = quatRB;
		oldCarRBState.AngularVelocity = angVelRB;

	}
	else if (this->recordCar)
	{
		recordCar = false;
		if (this->dump_file.is_open())
		{
			this->dump_file.close();
			cvarManager->log("dump file closed");
		}
	}
	
}

void SciencePlugin::OnSetInput()
{
	if (gameWrapper->IsInOnlineGame() || !(gameWrapper->IsInFreeplay() || gameWrapper->IsInCustomTraining() || gameWrapper->IsInGame())) {
		return;
	}
	
	if (cvarManager->getCvar("overrideInput").getBoolValue())
	{
		ServerWrapper server = gameWrapper->GetGameEventAsServer();
		auto players = gameWrapper->GetGameEventAsServer().GetCars();
		if (players.Count() > 0)
		{
			auto player0 = players.Get(0);

			static ControllerInput input;
			input.Throttle = cvarManager->getCvar("macroThrottle").getFloatValue();
			input.Steer = cvarManager->getCvar("macroSteer").getFloatValue();
			input.Handbrake = cvarManager->getCvar("macroHandbrake").getBoolValue();

			gameWrapper->OverrideParams(&input, sizeof(ControllerInput));

			/*
			if (cvarManager->getCvar("macroAllowOwnInput").getBoolValue())
			{
				ControllerInput input = player0.GetInput();
				
				if (cvarManager->getCvar("overrideThrottle").getBoolValue())
				{
					input.Throttle = cvarManager->getCvar("macroThrottle").getFloatValue();
				}
				if (cvarManager->getCvar("overrideSteer").getBoolValue())
				{
					input.Steer = cvarManager->getCvar("macroSteer").getFloatValue();
				}
				if (cvarManager->getCvar("overrideHandbrake").getBoolValue())
				{
					input.Handbrake = cvarManager->getCvar("macroHandbrake").getBoolValue();
				}

				gameWrapper->OverrideParams(&input, sizeof(ControllerInput));
			}
			else
			{
				static ControllerInput input;
				input.Throttle = cvarManager->getCvar("macroThrottle").getFloatValue();
				input.Steer = cvarManager->getCvar("macroSteer").getFloatValue();
				input.Handbrake = cvarManager->getCvar("macroHandbrake").getBoolValue();

				gameWrapper->OverrideParams(&input, sizeof(ControllerInput));
			}			
			*/
		}
	}
	else if (cvarManager->getCvar("turnRightAt1000").getBoolValue())
	{
		ServerWrapper server = gameWrapper->GetGameEventAsServer();
		auto players = gameWrapper->GetGameEventAsServer().GetCars();
		if (players.Count() > 0)
		{
			auto player0 = players.Get(0);

			static ControllerInput input;
			input.Throttle = 1.0;

			RBState rbstate = player0.GetRBState();
			if (rbstate.LinearVelocity.X > 1000) {
				input.Steer = 1.0;
			}

			gameWrapper->OverrideParams(&input, sizeof(ControllerInput));
		}
	}
}
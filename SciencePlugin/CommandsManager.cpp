#include "CommandsManager.h"
#include "HelperFunctions.h"
#include "utils\parser.h"
#include "bakkesmod\wrappers\GameEvent\TutorialWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarWrapper.h"
#include "bakkesmod\wrappers\arraywrapper.h"
#include "bakkesmod\wrappers\GameObject\CarComponent\VehicleSimWrapper.h"

using namespace sp;

void CommandsManager::addCommands()
{
	cvarManager->registerCvar("recordCarInfo", "0");
	cvarManager->registerCvar("recordBallInfo", "0");
	cvarManager->registerCvar("recordEveryTick", "0");

	cvarManager->registerCvar("showHUD", "1");
	cvarManager->registerCvar("showCarRBState", "1");
	cvarManager->registerCvar("showBallRBState", "1");
	cvarManager->registerCvar("showCarDerivedInfo", "1");
	cvarManager->registerCvar("showInput", "1");
	cvarManager->registerCvar("showYaw", "1");


	cvarManager->registerCvar("overrideInput", "0");
	//cvarManager->registerCvar("macroAllowOwnInput", "0");

	//cvarManager->registerCvar("overrideThrottle", "0");
	//cvarManager->registerCvar("overrideSteer", "0");
	//cvarManager->registerCvar("overrideHandbrake", "0");
	cvarManager->registerCvar("macroThrottle", "0.0");
	cvarManager->registerCvar("macroSteer", "0.0");
	cvarManager->registerCvar("macroHandbrake", "0");

	cvarManager->registerCvar("turnRightAt1000", "0");

	cvarManager->registerNotifier("resetInputs", [this](std::vector<string> commands) {
		cvarManager->getCvar("macroThrottle").setValue(0.0f);
		cvarManager->getCvar("macroSteer").setValue(0.0f);
		cvarManager->getCvar("macroHandbrake").setValue(0);

		std::string logString = "Inputs have been reset ";
		logger.log(logString);
	}, "", 0);

	cvarManager->registerNotifier("setBallRBLocation", [this](std::vector<string> commands) {
		if (commands.size() > 3)
		{
			float x = get_safe_float(commands.at(1));
			float y = get_safe_float(commands.at(2));
			float z = get_safe_float(commands.at(3));
			ServerWrapper game = gameWrapper->GetGameEventAsServer();
			BallWrapper ball = game.GetBall();
			if (ball.IsNull())
				return;

			auto rbstate = ball.GetRBState();
			auto loc = rbstate.Location;
			loc.X = x;
			loc.Y = y;
			loc.Z = z;
			rbstate.Location = loc;
			ball.SetPhysicsState(rbstate);

			std::string logString = "ball RB location set to " + vector_to_string(loc);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("setBallRBLinearVelocity", [this](std::vector<string> commands) {
		if (commands.size() > 3)
		{
			float x = get_safe_float(commands.at(1));
			float y = get_safe_float(commands.at(2));
			float z = get_safe_float(commands.at(3));
			ServerWrapper game = gameWrapper->GetGameEventAsServer();
			BallWrapper ball = game.GetBall();
			if (ball.IsNull())
				return;

			auto rbstate = ball.GetRBState();
			auto vel = rbstate.LinearVelocity;
			vel.X = x;
			vel.Y = y;
			vel.Z = z;
			rbstate.LinearVelocity = vel;
			ball.SetPhysicsState(rbstate);

			std::string logString = "ball RB linear velocity set to " + vector_to_string(vel, 2);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("setBallRBAngularVelocity", [this](std::vector<string> commands) {
		if (commands.size() > 3)
		{
			float x = get_safe_float(commands.at(1));
			float y = get_safe_float(commands.at(2));
			float z = get_safe_float(commands.at(3));
			ServerWrapper game = gameWrapper->GetGameEventAsServer();
			BallWrapper ball = game.GetBall();
			if (ball.IsNull())
				return;

			auto rbstate = ball.GetRBState();
			auto vel = rbstate.AngularVelocity;
			vel.X = x;
			vel.Y = y;
			vel.Z = z;
			rbstate.AngularVelocity = vel;
			ball.SetPhysicsState(rbstate);

			std::string logString = "ball RB angular velocity set to " + vector_to_string(vel, 4);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("setBallRBQuaternion", [this](std::vector<string> commands) {
		if (commands.size() > 4)
		{
			float w = get_safe_float(commands.at(1));
			float x = get_safe_float(commands.at(2));
			float y = get_safe_float(commands.at(3));
			float z = get_safe_float(commands.at(4));
			ServerWrapper game = gameWrapper->GetGameEventAsServer();
			BallWrapper ball = game.GetBall();
			if (ball.IsNull())
				return;

			auto rbstate = ball.GetRBState();
			auto quat = rbstate.Quaternion;
			quat.W = w;
			quat.X = x;
			quat.Y = y;
			quat.Z = z;
			rbstate.Quaternion = quat;
			ball.SetPhysicsState(rbstate);

			std::string logString = "ball RB quaternion set to " + quat_to_string(quat);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("setCarRBLocation", [this](std::vector<string> commands) {
		if (commands.size() > 3)
		{
			float x = get_safe_float(commands.at(1));
			float y = get_safe_float(commands.at(2));
			float z = get_safe_float(commands.at(3));
			ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
			CarWrapper gameCar = tutorial.GetGameCar();

			VehicleWrapper vehicle = (VehicleWrapper)gameCar;
			RBActorWrapper rbActor = (RBActorWrapper)vehicle;
			ActorWrapper actor = (ActorWrapper)rbActor;

			auto rbstate = rbActor.GetRBState();
			auto loc = rbstate.Location;
			loc.X = x;
			loc.Y = y;
			loc.Z = z;
			rbstate.Location = loc;
			rbActor.SetPhysicsState(rbstate);

			std::string logString = "car RB location set to " + vector_to_string(loc);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("setCarRBLinearVelocity", [this](std::vector<string> commands) {
		if (commands.size() > 3)
		{
			float x = get_safe_float(commands.at(1));
			float y = get_safe_float(commands.at(2));
			float z = get_safe_float(commands.at(3));
			ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
			CarWrapper gameCar = tutorial.GetGameCar();

			VehicleWrapper vehicle = (VehicleWrapper)gameCar;
			RBActorWrapper rbActor = (RBActorWrapper)vehicle;
			ActorWrapper actor = (ActorWrapper)rbActor;

			auto rbstate = rbActor.GetRBState();
			auto vel = rbstate.LinearVelocity;
			vel.X = x;
			vel.Y = y;
			vel.Z = z;
			rbstate.LinearVelocity = vel;
			rbActor.SetPhysicsState(rbstate);

			std::string logString = "car RB linear velocity set to " + vector_to_string(vel, 2);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("setCarRBAngularVelocity", [this](std::vector<string> commands) {
		if (commands.size() > 3)
		{
			float x = get_safe_float(commands.at(1));
			float y = get_safe_float(commands.at(2));
			float z = get_safe_float(commands.at(3));
			ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
			CarWrapper gameCar = tutorial.GetGameCar();

			VehicleWrapper vehicle = (VehicleWrapper)gameCar;
			RBActorWrapper rbActor = (RBActorWrapper)vehicle;
			ActorWrapper actor = (ActorWrapper)rbActor;

			auto rbstate = rbActor.GetRBState();
			auto vel = rbstate.AngularVelocity;
			vel.X = x;
			vel.Y = y;
			vel.Z = z;
			rbstate.AngularVelocity = vel;
			rbActor.SetPhysicsState(rbstate);

			std::string logString = "car RB angular velocity set to " + vector_to_string(vel, 4);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("setCarRBQuaternion", [this](std::vector<string> commands) {
		if (commands.size() > 4)
		{
			float w = get_safe_float(commands.at(1));
			float x = get_safe_float(commands.at(2));
			float y = get_safe_float(commands.at(3));
			float z = get_safe_float(commands.at(4));
			ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
			CarWrapper gameCar = tutorial.GetGameCar();

			VehicleWrapper vehicle = (VehicleWrapper)gameCar;
			RBActorWrapper rbActor = (RBActorWrapper)vehicle;
			ActorWrapper actor = (ActorWrapper)rbActor;

			auto rbstate = rbActor.GetRBState();
			auto quat = rbstate.Quaternion;
			quat.W = w;
			quat.X = x;
			quat.Y = y;
			quat.Z = z;
			rbstate.Quaternion = quat;
			rbActor.SetPhysicsState(rbstate);

			std::string logString = "car RB quaternion set to " + quat_to_string(quat);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("resetCar", [this](std::vector<string> commands) {
		if (commands.size() > 0)
		{
			ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
			CarWrapper gameCar = tutorial.GetGameCar();

			auto rbstate = gameCar.GetRBState();
			rbstate.LinearVelocity = Vector(0, 0, 0);
			rbstate.Quaternion.W = 1;
			rbstate.Quaternion.X = 0;
			rbstate.Quaternion.Y = 0;
			rbstate.Quaternion.Z = 0;
			rbstate.AngularVelocity = Vector(0, 0, 0);
			gameCar.SetPhysicsState(rbstate);

			std::string logString = "Car has been reset";
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("slideRight", [this](std::vector<string> commands) {
		if (commands.size() > 1)
		{
			std::string s = commands.at(1);
			float f = get_safe_float(s);
			ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
			CarWrapper gameCar = tutorial.GetGameCar();

			auto rbstate = gameCar.GetRBState();
			auto quatRB = rbstate.Quaternion;
			Vector right = quatToRight(quatRB);
			right.normalize();
			auto slide = Vector(right.X * f, right.Y * f, right.Z * f);

			gameCar.AddVelocity(slide);
			std::string logString = "Velocity increased by " + vector_to_string(slide);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("setMass", [this](std::vector<string> commands) {
		if (commands.size() > 1)
		{
			std::string s = commands.at(1);
			float f = get_safe_float(s);
			ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
			CarWrapper gameCar = tutorial.GetGameCar();

			gameCar.SetMass(f);
			std::string logString = "RB mass set to " + to_string(s);
			logger.log(logString);
		}
	}, "", 0);


	cvarManager->registerNotifier("setOnGround", [this](std::vector<string> commands) {
		if (commands.size() > 1)
		{
			std::string s = commands.at(1);
			bool b = get_safe_bool(s);
			ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
			CarWrapper gameCar = tutorial.GetGameCar();

			gameCar.SetbOnGround((unsigned long)b);
			std::string logString = "bOnGround set to " + to_string(s);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("setUseAckermannSteering", [this](std::vector<string> commands) {
		if (commands.size() > 1)
		{
			std::string s = commands.at(1);
			bool b = get_safe_bool(s);
			ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
			CarWrapper gameCar = tutorial.GetGameCar();

			gameCar.GetVehicleSim().SetbUseAckermannSteering(b);
			std::string logString = "bUseAckermannSteering set to " + to_string(s);
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("getUseAckermannSteering", [this](std::vector<string> commands) {
		ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
		CarWrapper gameCar = tutorial.GetGameCar();

		auto b = gameCar.GetVehicleSim().GetbUseAckermannSteering();
		std::string logString = "bUseAckermannSteering = " + to_string(b);
		logger.log(logString);
	}, "", 0);

	cvarManager->registerNotifier("setDriveTorque", [this](std::vector<string> commands) {
		if (commands.size() > 1)
		{
			auto s = commands.at(1);
			float f = get_safe_float(s);
			ServerWrapper training = gameWrapper->GetGameEventAsServer();
			CarWrapper car = training.GetGameCar();
			auto vehicleSim = car.GetVehicleSim();
			vehicleSim.SetDriveTorque(f);
			auto logString = "drive torque set to: " + std::to_string(vehicleSim.GetDriveTorque());
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("getDriveTorque", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		auto logString = "drive torque: " + std::to_string(vehicleSim.GetDriveTorque());
		logger.log(logString);
	}, "", 0);

	cvarManager->registerNotifier("setBrakeTorque", [this](std::vector<string> commands) {
		if (commands.size() > 1)
		{
			auto s = commands.at(1);
			float f = get_safe_float(s);
			ServerWrapper training = gameWrapper->GetGameEventAsServer();
			CarWrapper car = training.GetGameCar();
			auto vehicleSim = car.GetVehicleSim();
			vehicleSim.SetBrakeTorque(f);
			auto logString = "brake torque set to: " + std::to_string(vehicleSim.GetBrakeTorque());
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("getBrakeTorque", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		auto logString = "brake torque: " + std::to_string(vehicleSim.GetBrakeTorque());
		logger.log(logString);
	}, "", 0);

	cvarManager->registerNotifier("setStopThreshold", [this](std::vector<string> commands) {
		if (commands.size() > 1)
		{
			auto s = commands.at(1);
			float f = get_safe_float(s);
			ServerWrapper training = gameWrapper->GetGameEventAsServer();
			CarWrapper car = training.GetGameCar();
			auto vehicleSim = car.GetVehicleSim();
			vehicleSim.SetStopThreshold(f);
			auto logString = "stop threshold set to: " + std::to_string(vehicleSim.GetStopThreshold());
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("getStopThreshold", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		auto logString = "stop threshold: " + std::to_string(vehicleSim.GetStopThreshold());
		logger.log(logString);
	}, "", 0);

	cvarManager->registerNotifier("setIdleBrakeFactor", [this](std::vector<string> commands) {
		if (commands.size() > 1)
		{
			auto s = commands.at(1);
			float f = get_safe_float(s);
			ServerWrapper training = gameWrapper->GetGameEventAsServer();
			CarWrapper car = training.GetGameCar();
			auto vehicleSim = car.GetVehicleSim();
			vehicleSim.SetIdleBrakeFactor(f);
			auto logString = "idle brake factor set to: " + std::to_string(vehicleSim.GetIdleBrakeFactor());
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("getIdleBrakeFactor", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		auto logString = "idle brake factor: " + std::to_string(vehicleSim.GetIdleBrakeFactor());
		logger.log(logString);
	}, "", 0);

	cvarManager->registerNotifier("setOppositeBrakeFactor", [this](std::vector<string> commands) {
		if (commands.size() > 1)
		{
			auto s = commands.at(1);
			float f = get_safe_float(s);
			ServerWrapper training = gameWrapper->GetGameEventAsServer();
			CarWrapper car = training.GetGameCar();
			auto vehicleSim = car.GetVehicleSim();
			vehicleSim.SetOppositeBrakeFactor(f);
			auto logString = "opposite brake factor set to: " + std::to_string(vehicleSim.GetOppositeBrakeFactor());
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("getOppositeBrakeFactor", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		auto logString = "opposite brake factor: " + std::to_string(vehicleSim.GetOppositeBrakeFactor());
		logger.log(logString);
	}, "", 0);

	cvarManager->registerNotifier("getSuspensionStiffness", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
		if (wheels.Count() >= 4)
		{
			for (int i = 0; i < wheels.Count(); i++)
			{
				auto logString = "suspension stiffness for wheel " + std::to_string(i) + ": " + std::to_string(wheels.Get(i).GetSuspensionStiffness());
				logger.log(logString);
			}
		}
	}, "", 0);

	cvarManager->registerNotifier("getSuspensionDampingCompression", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
		if (wheels.Count() >= 4)
		{
			for (int i = 0; i < wheels.Count(); i++)
			{
				auto logString = "suspension damping compression for wheel " + std::to_string(i) + ": " + std::to_string(wheels.Get(i).GetSuspensionDampingCompression());
				logger.log(logString);
			}
		}
	}, "", 0);

	cvarManager->registerNotifier("getSuspensionDampingRelaxation", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
		if (wheels.Count() >= 4)
		{
			for (int i = 0; i < wheels.Count(); i++)
			{
				auto logString = "suspension damping relaxation for wheel " + std::to_string(i) + ": " + std::to_string(wheels.Get(i).GetSuspensionDampingRelaxation());
				logger.log(logString);
			}
		}
	}, "", 0);

	cvarManager->registerNotifier("getSuspensionTravel", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
		if (wheels.Count() >= 4)
		{
			for (int i = 0; i < wheels.Count(); i++)
			{
				auto logString = "suspension travel for wheel " + std::to_string(i) + ": " + std::to_string(wheels.Get(i).GetSuspensionTravel());
				logger.log(logString);
			}
		}
	}, "", 0);

	cvarManager->registerNotifier("getSuspensionMaxRaise", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
		if (wheels.Count() >= 4)
		{
			for (int i = 0; i < wheels.Count(); i++)
			{
				auto logString = "suspension max raise for wheel " + std::to_string(i) + ": " + std::to_string(wheels.Get(i).GetSuspensionMaxRaise());
				logger.log(logString);
			}
		}
	}, "", 0);

	cvarManager->registerNotifier("setSpinSpeedDecayRate", [this](std::vector<string> commands) {
		if (commands.size() > 1)
		{
			auto s = commands.at(1);
			float f = get_safe_float(s);
			ServerWrapper training = gameWrapper->GetGameEventAsServer();
			CarWrapper car = training.GetGameCar();
			auto vehicleSim = car.GetVehicleSim();
			ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
			if (wheels.Count() >= 4)
			{
				for (int i = 0; i < wheels.Count(); i++)
				{
					wheels.Get(i).SetSpinSpeedDecayRate(f);
				}
				auto logString = "spinspeed decay rate for all wheels set to: " + std::to_string(wheels.Get(0).GetSpinSpeedDecayRate());
				logger.log(logString);
			}
		}
	}, "", 0);

	cvarManager->registerNotifier("getSpinSpeedDecayRate", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
		if (wheels.Count() >= 4)
		{
			for (int i = 0; i < wheels.Count(); i++)
			{
				auto logString = "spinspeed decay rate for wheel " + std::to_string(i) + ": " + std::to_string(wheels.Get(i).GetSpinSpeedDecayRate());
				logger.log(logString);
			}
		}
	}, "", 0);

	cvarManager->registerNotifier("setSpinSpeed", [this](std::vector<string> commands) {
		if (commands.size() > 2)
		{
			auto s = commands.at(1);
			auto s2 = commands.at(2);
			float f = get_safe_float(s2);
			auto i = get_safe_int(s);
			if (i < 0 || i > 3) {
				return;
			}
			ServerWrapper training = gameWrapper->GetGameEventAsServer();
			CarWrapper car = training.GetGameCar();
			auto vehicleSim = car.GetVehicleSim();
			ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
			wheels.Get(i).SetSpinSpeed(f);
			auto logString = "spinspeed for wheel " + to_string(i) + " set to: " + std::to_string(wheels.Get(i).GetSpinSpeed());
			logger.log(logString);
		}
	}, "", 0);

	cvarManager->registerNotifier("getSpinSpeed", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
		if (wheels.Count() >= 4)
		{
			for (int i = 0; i < wheels.Count(); i++)
			{
				auto logString = "spinspeed for wheel " + std::to_string(i) + ": " + std::to_string(wheels.Get(i).GetSpinSpeed());
				logger.log(logString);
			}
		}
	}, "", 0);

	cvarManager->registerNotifier("getSteer", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
		if (wheels.Count() >= 4)
		{
			for (int i = 0; i < wheels.Count(); i++)
			{
				auto logString = "steer for wheel " + std::to_string(i) + ": " + std::to_string(wheels.Get(i).GetSteer2());
				logger.log(logString);
			}
		}
	}, "", 0);

	cvarManager->registerNotifier("getSuspensionDistance", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
		if (wheels.Count() >= 4)
		{
			for (int i = 0; i < wheels.Count(); i++)
			{
				auto logString = "suspension distance for wheel " + std::to_string(i) + ": " + std::to_string(wheels.Get(i).GetSuspensionDistance());
				logger.log(logString);
			}
		}
	}, "", 0);

	cvarManager->registerNotifier("getRefWheelLocation", [this](std::vector<string> commands) {
		ServerWrapper training = gameWrapper->GetGameEventAsServer();
		CarWrapper car = training.GetGameCar();
		auto vehicleSim = car.GetVehicleSim();
		ArrayWrapper<WheelWrapper> wheels = vehicleSim.GetWheels();
		if (wheels.Count() >= 4)
		{
			for (int i = 0; i < wheels.Count(); i++)
			{
				auto logString = "ref wheel location wheel " + std::to_string(i) + ": " + vector_to_string(wheels.Get(i).GetRefWheelLocation());
				logger.log(logString);
			}
		}
	}, "", 0);

}
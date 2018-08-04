#include "HelperFunctions.h"

std::string sp::vector_to_string(Vector v, int prec)
{
	return to_string_decimals(v.X, prec) + ", " + to_string_decimals(v.Y, prec) + ", " + to_string_decimals(v.Z, prec);
}

std::string sp::rot_to_string(Rotator v, int prec)
{
	return to_string_decimals(v.Pitch, prec) + ", " + to_string_decimals(v.Yaw, prec) + ", " + to_string_decimals(v.Roll, prec);
}

std::string sp::quat_to_string(FQuat v, int prec)
{
	return to_string_decimals(v.W, prec) + ", " + to_string_decimals(v.X, prec) + ", " + to_string_decimals(v.Y, prec) + ", " + to_string_decimals(v.Z, prec);
}

std::string sp::to_string(float f, int prec)
{
	return to_string_decimals(f, prec);
}

std::string sp::to_string_scientific(float f, int prec, int treshold)
{
	return to_string_decimals_scientific(f, prec, treshold);
}

bool sp::vector_eq(Vector v1, Vector v2)
{
	return (v1.X == v2.X) && (v1.Y == v2.Y) && (v1.Z == v2.Z);
}

bool sp::quat_eq(FQuat q1, FQuat q2)
{
	return (q1.W == q2.W) && (q1.X == q2.X) && (q1.Y == q2.Y) && (q1.Z == q2.Z);
}

bool sp::rot_eq(Rotator r1, Rotator r2)
{
	return (r1.Pitch == r2.Pitch) && (r1.Yaw == r2.Yaw) && (r1.Roll == r2.Roll);
}

Vector sp::quatToFwd(FQuat quat)
{
	// Vector fwd = Vector(1 - 2 * (quat.X*quat.X + quat.Y*quat.Y), 2 * (quat.Y*quat.Z + quat.X*quat.W), -1 * (2 * (quat.X*quat.Z - quat.Y*quat.W)));

	Vector fwd = rotateVectorWithQuat(Vector(1, 0, 0), quat);
	fwd.normalize();
	return fwd;
}

Vector sp::quatToRight(FQuat quat)
{
	// Vector right = Vector(2 * (quat.Y*quat.Z - quat.X*quat.W), 1 - 2 * (quat.X*quat.X + quat.Z*quat.Z), -1 * (2 * (quat.X*quat.Y + quat.Z*quat.W)));

	Vector right = rotateVectorWithQuat(Vector(0, 1, 0), quat);
	right.normalize();
	return right;
}

Vector sp::quatToUp(FQuat quat)
{
	// Vector up = Vector(2 * (quat.X*quat.Z + quat.Y*quat.W), 2 * (quat.X*quat.Y - quat.Z*quat.W), -1 * (1 - 2 * (quat.Y*quat.Y + quat.Z*quat.Z)));

	Vector up = rotateVectorWithQuat(Vector(0, 0, 1), quat);
	up.normalize();
	return up;
}

Vector sp::rotateVectorWithQuat(Vector v, FQuat q)
{
	FQuat p;
	p.W = 0;
	p.X = v.X;
	p.Y = v.Y;
	p.Z = v.Z;

	FQuat result = (q * p) * q.conjugate();
	return Vector(result.X, result.Y, result.Z);
}
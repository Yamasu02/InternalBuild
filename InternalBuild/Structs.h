#pragma once

class Vector3
{
public:
	float x;
	float y;
	float z;

	Vector3()
	{
		x = y = z = 0;
	}

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3& operator+(Vector3 arg)
	{
		x += arg.x;
		y += arg.y;
		z += arg.z;
		return *this;
	}

	Vector3& operator*(Vector3 arg)
	{
		x *= arg.x;
		y *= arg.y;
		z *= arg.z;
		return *this;
	}
};

struct Vec2
{
	float x, y;
};

struct Vec4
{
	float x, y, z, w;
};

struct ViewMatrix
{
	float matrix [4][4];
	float* operator[ ](int index)
	{
		return matrix[index];
	}
};

class Client
{

};

class Clientmode
{

};



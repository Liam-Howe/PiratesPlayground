#pragma once
#include <cmath>

class Vector2
{
public:
	Vector2(float pX = 0, float pY = 0) 
		: x(pX), y(pY) 
	{ 
	}
	~Vector2() {}

	float Vector2::Length() const
	{
		return sqrt(x * x + y * y);
	}

	Vector2 Vector2::Normalize() const
	{
		Vector2 vector;
		float length = this->Length();

		if (length != 0)
		{
			vector.x = float(x) / length;
			vector.y = float(y) / length;
		}

		return vector;
	}

	Vector2 operator * (const float val) const
	{
		return Vector2(x * val, y * val);
	}
	Vector2 operator - (const Vector2 pVec) const
	{
		return Vector2(x - pVec.x, y - pVec.y);
	}
	Vector2 operator + (const Vector2 pVec) const
	{
		return Vector2(x + pVec.x, y + pVec.y);
	}

	Vector2 operator / (const float& pRhs)
	{
		return Vector2(x / pRhs, y / pRhs);
	}


	Vector2& operator+=(const Vector2 pVec)
	{
		this->x += pVec.x;
		this->y += pVec.y;
		return *this;
	}

	bool operator!=(const Vector2& rhs)
	{
		return (x != rhs.x || y != rhs.y);
	}

	bool operator==(const Vector2& rhs)
	{
		return (x == rhs.x && y == rhs.y);
	}

	Vector2& operator*=(const float& x)
	{
		this->x *= x;
		this->y *= x;
		return *this;
	}

	static float CalculateDistance(const Vector2& p1, const Vector2 p2)
	{
		float diffY = p1.y - p2.y;
		float diffX = p1.x - p2.x;
		return sqrt((diffY * diffY) + (diffX * diffX));
	}


	static Vector2 NormaliseVec(Vector2 vecA)
	{
		float lengthA = sqrt((vecA.x * vecA.x) + (vecA.y * vecA.y));
		if (lengthA != 0)
		{
			vecA.x /= lengthA;
			vecA.y /= lengthA;
		}
		return vecA;
	}


	float x;
	float y;
};
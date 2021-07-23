#pragma once

#include <cstdint>
#include <cmath>
#include <list>
#include <memory>

namespace Melee
{
	class Engine;
	class Entity;

	struct Vector2d
	{
		float x = 0;
		float y = 0;

		float lengthSquared() const
		{
			return x * x + y * y;
		}

		float length() const
		{
			return sqrt(x * x + y * y);
		}

		Vector2d normalised() const
		{
			return Vector2d{ x, y } / length();
		}

		// TODO: Cross Product

		Vector2d& operator +=(const Vector2d& other)
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		Vector2d& operator -=(const Vector2d& other)
		{
			x -= other.x;
			y -= other.y;

			return *this;
		}

		Vector2d& operator *=(float scaler)
		{
			x *= scaler;
			y *= scaler;

			return *this;
		}

		Vector2d& operator /=(float scaler)
		{
			x /= scaler;
			y /= scaler;

			return *this;
		}

		Vector2d operator -() const
		{
			return Vector2d{ -x, -y };
		}

		Vector2d operator +(const Vector2d& other) const
		{
			Vector2d v = *this;
			v.operator+=(other);

			return v;
		}

		Vector2d operator -(const Vector2d& other) const
		{
			Vector2d v = *this;
			v.operator-=(other);

			return v;
		}

		Vector2d operator *(float scaler) const
		{
			Vector2d v = *this;
			v.operator*=(scaler);

			return v;
		}

		Vector2d operator /(float scaler) const
		{
			Vector2d v = *this;
			v.operator/=(scaler);

			return v;
		}
	};

	struct Matrix2x2
	{
		float x0y0 = 0;
		float x1y0 = 0;
		float x0y1 = 0;
		float x1y1 = 0;

		Vector2d operator *(const Vector2d& v) const
		{
			return Vector2d{ v.x * x0y0 + v.y * x1y0, v.x * x0y1 + v.y * x1y1 };
		}
	};

	static inline Matrix2x2 RotationMatrix(float degrees)
	{
		const float rads = degrees * static_cast<float>(2 * M_PI / 360);
		const float cosAng = cosf(rads);
		const float sinAng = sinf(rads);

		return Matrix2x2{ cosAng, -sinAng, sinAng, cosAng };
	}

	using Point = Vector2d;

	using EntityList = std::list<std::shared_ptr<Entity>>;
}

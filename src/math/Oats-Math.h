#pragma once
#include <cmath>

namespace oats
{
	struct float2
	{
		float x, y;
	};

	struct float3
	{
		float x, y, z;
	};

	struct circle
	{
		float2 origin;
		float radius;
	};

	inline float2 operator*(const float2& v, const float s)
	{
		return float2{ v.x * s, v.y * s };
	}

	inline float3 operator*(const float3& v, const float s)
	{
		return float3{ v.x * s, v.y * s, v.z * s };
	}

	inline float2 operator*(const float2& l, const float2& r)
	{
		return float2{ l.x * r.x, l.y * r.y };
	}

	inline float3 operator*(const float3& l, const float3& r)
	{
		return float3{ l.x * r.x, l.y * r.y };
	}

	inline float2 operator/(const float2& v, const float s)
	{
		return float2{ v.x / s, v.y / s };
	}

	inline float3 operator/(const float3& v, const float s)
	{
		return float3{ v.x / s, v.y / s, v.z / 2 };
	}

	inline float2 operator+(const float2& v1, const float2& v2)
	{
		return float2{ v1.x + v2.x, v1.y + v2.y };
	}

	inline float3 operator+(const float3& v1, const float3& v2)
	{
		return float3{ v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}

	inline float2 operator-(const float2& v1, const float2& v2)
	{
		return float2{ v1.x - v2.x, v1.y - v2.y };
	}

	inline const float3 operator-(const float3& v1, const float3& v2)
	{
		return float3{ v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}

	inline float2& operator+=(float2& v1, const float2& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		return v1;
	}

	inline float3& operator+=(float3& v1, const float3& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		v1.z += v2.z;
		return v1;
	}

	inline float3& operator-=(float3& v1, const float3& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		v1.z -= v2.z;
		return v1;
	}

	inline float2& operator-=(float2& v1, const float2& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		return v1;
	}

	inline float2& operator*=(float2& v1, const float s)
	{
		v1.x *= s;
		v1.y *= s;
		return v1;
	}

	inline float dot(const float2& v1, const float2& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y);
	}

	inline float dot(const float3& v1, const float3& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	}

	inline float lengthsq(const float2& v)
	{
		return dot(v, v);
	}

	inline float lengthsq(const float3& v)
	{
		return dot(v, v);
	}

	inline float length(const float2& v)
	{
		return std::sqrtf(lengthsq(v));
	}

	inline float length(const float3& v)
	{
		return std::sqrtf(lengthsq(v));
	}

	inline float2 normalize(const float2& v)
	{
		return v / length(v);
	}

	inline float2 zero2()
	{
		return float2{ 0, 0 };
	}

	inline float3 zero3()
	{
		return float3{ 0, 0, 0 };
	}

	float2 normalizesafe(const float2& v)
	{
		float sqrMag = lengthsq(v);
		if (sqrMag > 0)
			return v / std::sqrtf(sqrMag);
		else
			return zero2();
	}

	float3 normalizesafe(const float3& v)
	{
		float sqrMag = lengthsq(v);
		if (sqrMag > 0)
			return v / std::sqrtf(sqrMag);
		else
			return zero3();
	}


	inline float min(const float a, const float b)
	{
		return std::min(a, b);
	}

	inline float max(const float a, const float b)
	{
		return std::max(a, b);
	}

	inline float clamp(const float n, const float l, const float h)
	{
		return min(h, max(l, n));
	}

	// Note: this is slow but precise lerp. Improve later?
	inline float lerp(const float v0, const float v1, const float t)
	{
		return ((1.0F - t) * v0) + (t * v1);
	}

	inline float2 lerp(const float2& l, const float2& r, const float t)
	{
		return float2{ lerp(l.x, r.x, t), lerp(l.y, r.y, t) };
	}

	inline float2 projectIntoBox(const float2& v, const float xLow, const float xHigh, const float yLow, const float yHigh)
	{
		float2 pos;
		pos.x = clamp(v.x, xLow, xHigh);
		pos.y = clamp(v.y, yLow, yHigh);
		return pos;
	}

	inline float distancesq(const float3& left, const float3& right)
	{
		return lengthsq(left - right);
	}

	inline float distancesq(const float2& left, const float2& right)
	{
		return lengthsq(left - right);
	}

	inline float distance(const float3& left, const float3& right)
	{
		return std::sqrtf(distancesq(left, right));
	}

	inline float distance(const float2& left, const float2& right)
	{
		return std::sqrtf(distancesq(left, right));
	}

	inline float distance(const float left, const float right)
	{
		return std::abs(left - right);
	}


	template <class T>
	inline T pow2(const T p)
	{
		return p * p;
	}

	inline bool overlapTest(const circle& left, const circle& right)
	{
		return distancesq(left.origin, right.origin) <= pow2(left.radius + right.radius);
	}

	inline void verlet(float2& pos, float2& oldPos, const float2& accel, const float dt)
	{
		float2 newPos = (pos * 2) - oldPos + (accel * (dt * dt));
		oldPos = pos;
		pos = newPos;
	}

	inline void verlet(float3& pos, float3& oldPos, const float3& accel, const float dt)
	{
		float3 newPos = (pos * 2) - oldPos + (accel * (dt * dt));
		oldPos = pos;
		pos = newPos;
	}

	inline void jakobsen_satisfy(float3& x1, float3& x2, const float restLength)
	{
		const float3 delta = x2 - x1;
		const float deltaLength = length(delta);

		if (deltaLength <= 0)
			return;

		const float diff = (deltaLength - restLength) / deltaLength;
		x1 += delta * (0.5F * diff);
		x2 -= delta * (0.5F * diff);
	}
}


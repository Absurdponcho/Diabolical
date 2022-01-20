#pragma once
#include <string>
namespace types
{
	typedef signed char Int8;
	typedef unsigned char UInt8;
	typedef signed short Int16;
	typedef unsigned short UInt16;
	typedef signed int Int32;
	typedef unsigned int UInt32;
	typedef signed long long Int64;
	typedef unsigned long long UInt64;
	typedef float Float32;
	typedef double Float64;

	struct Vec2 {
		Float32 x, y;
		Vec2() :x(0), y(0) {};
		Vec2(Float32 x_, Float32 y_)
			:x(x_), y(y_) {};
	};

	struct Vec3 {
		Float32 x, y, z;
		Vec3() :x(0), y(0), z(0) {};
		Vec3(Float32 x_, Float32 y_, Float32 z_)
			:x(x_), y(y_), z(z_) {};
	};

	struct Vec4 {
		Float32 x, y, z, w;
		Vec4() :x(0), y(0), z(0), w(0) {};
		Vec4(Float32 x_, Float32 y_, Float32 z_, Float32 w_)
			:x(x_), y(y_), z(z_), w(w_) {};
	};

	typedef Vec2 vec2;
	typedef Vec3 vec3;
	typedef Vec4 vec4;
}
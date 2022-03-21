#pragma once

#define GENERATE_META()


struct A
{

	int intA = 12;

};

#ifdef __clang__
#define CLASS(a) __attribute__((annotate(a)))
#else
#define CLASS(a)
#endif
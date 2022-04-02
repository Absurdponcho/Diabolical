#pragma once
#include "Meta/Meta.h"

METACLASS MetaTest
{

public: 
	bool bTest = true;
	int intTest = 69;
	float floatTest = 69.69f;


	class Deez {};

	Deez deez; 
};


class MetaTest2
{

public: 
	bool bTest = true; 
	int intTest = 69;
	float floatTest = 69.69f;

};


METACLASS MetaTest3
{

public:
	bool bTest = true;
	int intTest = 69;
	float floatTest = 69.69f;
};

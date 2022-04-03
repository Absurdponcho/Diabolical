#pragma once
#include "Meta/Meta.h"
#include "Header.meta.h" 

METACLASS() MetaTest   
{
	IMPORT_META(MetaTest);   
public:      
	PROPERTY()     
	bool bTest = true;
	 
	PROPERTY()     
	int intTest = 69;        

	PROPERTY()
	float floatTest = 69.69f; 
	 
	 
};  
  
class MetaTest2 {}; 

METACLASS() MetaTest3 : public MetaTest, public MetaTest2
{
	IMPORT_META(MetaTest3);
public:
	bool bTest2 = true;
	int intTest2 = 69;
	float floatTest2 = 69.69f;
};

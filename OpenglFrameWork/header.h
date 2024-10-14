#pragma once
#define	SINGLE(type) public:\
						static type* GetInstance()\
						{\
							static type mgr;\
							return &mgr;\
						}\
						private:\
							type();\
							~type();
#include <string>
#include <vector>

enum MODEL_TYPE
{
	TRIANGLE = 0,
	RECTANGLE = 1
};
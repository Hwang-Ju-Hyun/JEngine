#pragma once

const int window_width = 1980;
const int window_height = 900;

enum MODEL_TYPE
{
	TRIANGLE = 0,
	RECTANGLE = 1,
	CIRCLE = 2
};

typedef struct WorldMouseCursor
{
	float x;
	float y;	
};
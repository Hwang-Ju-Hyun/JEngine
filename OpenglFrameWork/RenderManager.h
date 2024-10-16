#pragma once

#include "single.h"

class RenderManager
{
public:
	SINGLE(RenderManager);
public:
	bool Init();
	bool Update();
	bool Draw();
};


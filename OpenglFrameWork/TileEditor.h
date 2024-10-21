#pragma once
#include "single.h"
#include <glm.hpp>

class MainEditor;

class TileEditor
{
public:
	SINGLE(TileEditor);
private:
	bool m_aWallGridCord[1000][1000] = { false, };
public:
	glm::vec2 GetWorldPosbyScreenGrid(int _width,int _height,int _gridX,int _gridY);
	void Update();
};
#pragma once
#include "single.h"
#include <glm.hpp>

class Transform;
class MainEditor;

class TileEditor
{
public:
	SINGLE(TileEditor);
private:
	glm::vec2 m_vWall = {};
	int m_iScreenGridX = {};
	int m_iScreenGridY = {};
	int m_iWidth = 0;
	int m_iHeight = 0;
	unsigned int m_iNumberOfWalls=0;
	bool m_aWallGridCord[1000][1000] = { false, };
private:
	Transform* trans = nullptr;	
public:
	void CaculateWallPosition();
	void Update();
};
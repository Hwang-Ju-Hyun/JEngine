#pragma once
#include "single.h"
#include <glm.hpp>

class MainEditor;
class TextureResource;

class TileEditor
{
public:
	SINGLE(TileEditor);
private:
	int m_iNumberOfWalls=0;
	bool m_bWallGridCoord[1000][1000] = { false, };
	TextureResource* m_pCurrentTileTexture = nullptr;	
public:
	int GetNumberOfWalls()const;
	void ShowAndSetCurrentTileTexture();
	glm::vec2 GetWorldPosbyScreenGrid(int _width,int _height,int _gridX,int _gridY);
	glm::vec2 GetScreenGridByMousePos(glm::vec2 _mousePos);
	void SetWallGridCoord(int x, int y,bool _flag);

public:
	void Update();
};
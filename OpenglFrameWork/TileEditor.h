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
	int m_iNumberOfWallsWidth  = 0;
	int m_iNumberOfWallsHeight = 0;
	
	int m_iWallWidth = 0;
	int m_iWallHeight = 0;

	
	TextureResource* m_pCurrentTileTexture = nullptr;	
public:	
	void ShowAndSetCurrentTileTexture();
	glm::vec2 GetWorldPosbyScreenGrid(int _width,int _height,int _gridX,int _gridY);
	glm::vec2 GetScreenGridByPoint(glm::vec2 _pointPos);
	void SetWallGridCoord(int x, int y,bool _flag);	
	
public:
	int GetNumberOfWallWidth()const;
	int GetNumberOfWallHeight()const;
	void SetWallWidth(int _width);
	void SetWallHeight(int _height);
	int GetWallWidth()const;
	int GetWallHeight()const;
public:
	static bool m_sWallGridCoord[1000][1000];
public:	
	void Init();
	void Update();
};
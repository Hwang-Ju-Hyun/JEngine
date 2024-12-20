#pragma once
#include "single.h"
#include <glm.hpp>
#include <vector>

class MainEditor;
class TextureResource;
class GameObject;

class TileEditor
{
public:	
	SINGLE(TileEditor);
private:
	int m_iNumberOfWallsCol  = 0;
	int m_iNumberOfWallsRow = 0;
	
	int m_iWallWidth = 0;
	int m_iWallHeight = 0;

	unsigned int m_iMaxXGrid =0;
	unsigned int m_iMaxYGrid =0;

	int m_iScreenGridX = 0;
	int m_iScreenGridY = 0;

	bool m_bExist = false;	
public:
	std::vector<std::vector<bool>> m_vecWallGridCoord;
	std::vector<std::vector<bool>> m_vecStaticWallGridCoord;
private:
	TextureResource* m_pCurrentTileTexture = nullptr;	
	void ShowAndSetCurrentTileTexture();
private:	
	bool m_bCurFragileType = false;
	void ShowAndSetCurrentWallFragile();
public:
	glm::vec2 GetWorldPosbyScreenGrid(int _width,int _height,int _gridX,int _gridY);
	glm::vec2 GetScreenGridByScreenPoint(glm::vec2 _pointPos);
public:	
	void SetWallGridCoord(int _x, int _y,bool _flag);
	void SetWallWidth(int _width);
	void SetWallHeight(int _height);
	void SetScreenGridX(int _gridX);
	void SetScreenGridY(int _gridY);
	void SetExist(bool _exist);
public:
	int GetNumberOfWallWidth()const;
	int GetNumberOfWallHeight()const;
	int GetWallWidth()const;
	int GetWallHeight()const;
	int GetScreenGridX()const;
	int GetScreenGridY()const;
	int GetMaxXGrid()const;
	int GetMaxYGrid()const;
	bool GetExist()const;
	std::vector<std::vector<bool>> GetWallGrid()const;
public:
	GameObject* FindObjectByGrid(glm::vec2 _grid);
public:	
	bool Init();
	void Update();
};
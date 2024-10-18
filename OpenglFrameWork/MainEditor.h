#pragma once
#include "single.h"
#include <glm.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "header.h"

class Transform;
class GameObject;
class BaseLevel;
class TileEditor;

class MainEditor
{
public:
	SINGLE(MainEditor);
private:
	bool m_bShowObjectWindow = false;
	bool m_bCheckBoxTransform = false;
	bool m_bCheckBoxSprite = false;
	bool m_bObjectCreateBtn = false;
	bool m_bShowSaveConf = false;
	bool m_bShowDeleteConf = false;
	bool m_bShowChangeEditModeConf = false;
	bool m_bCurWindowObjectList = false;
private:
	GameObject* m_pSelectedGameObject = nullptr;
public:	
	void TopBar_GameObject();
	void TopBar_Save();
	void TopBar_TileEdit();
	void SelectedObjectWindow();
private:
	WorldMouseCursor m_vWorldMousePos = {};
	bool m_bSelectedObjByMouse = false;
	Transform* m_pTransByMouseSelect = nullptr;
public:
	void SelectedObjectByMouse();
private:
	
private:	
	glm::mat3 m_mScreenToWorldMat = {};
	glm::vec2 m_mScreenToMousePos = {};		
private:
	enum EDIT_MODE
	{
		NORMAL,
		TILE
	};
	int m_iCurrentMode =EDIT_MODE::NORMAL;
public:
	void ChangeCurrentEditMode(enum EDIT_MODE _eMode);
	int GetCurrentEditMode()const;
public:
	void Update();
public:
	friend class TileEditor;
};
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

class MainEditor
{
public:
	SINGLE(MainEditor);
private:
	bool m_bShowObjectWindow = false;
	bool m_bCheckBoxTransform = false;
	bool m_bCheckBoxSprite = false;
	bool m_bBtnObjectCreate = false;
	bool m_bShowSaveConfirmation = false;
	bool m_bShowDeleteConfirmationWindow = false;
private:
	GameObject* m_ptrSelectedGameObject = nullptr;
public:	
	void TopBar_GameObject();
	void TopBar_Save();
	void SelectedObjectWindow();	
private:
	WorldMouseCursor m_vWorldMousePos = {};
	bool m_bSelectedObjByMouse = false;
	Transform* m_pTransByMouseSelect = nullptr;
public:
	void SelectedObjectByMouse();
private:
	bool IsMouseInsideObject(GameObject* _obj);		
	void EditMapMode();
private:	
	glm::mat3 m_mScreenToWorldMat = {};
	glm::vec2 m_mScreenToMousePos = {};
	void CaculateWallPosition(glm::vec2* _wall);
	bool m_aWallGridCord[1000][1000] = { false, };
public:
	void Update();
};
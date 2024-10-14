#pragma once
#include "header.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm.hpp>

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
	unsigned int tempObjectID = 1000;
private:
	GameObject* m_ptrSelectedGameObject = nullptr;
public:	
	void TopBar_GameObject();
	void TopBar_Save();
	void SelectedObjectWindow();
public:
	void SelectedObjectByMouse();
private:
	bool IsMouseInsideObject(glm::vec2 _mousePos, GameObject* _obj);
public:
	void Update();
};
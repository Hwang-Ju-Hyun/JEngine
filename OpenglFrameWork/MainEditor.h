#pragma once
#include "header.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class GameObject;
class BaseLevel;

class MainEditor
{
public:
	SINGLE(MainEditor);
public:
	void TopBar_GameObject();
	void TopBar_Save();
	void SelectedObjectWindow();			
private:
	bool m_bShowObjectWindow = false;
	bool m_bCheckBoxTransform = false;
	bool m_bBtnObjectCreate = false;
	bool m_bShowSaveConfirmation = false;
private:
	unsigned int tempObjectID = 1000;
private:
	GameObject* m_ptrSelectedGameObject = nullptr;
public:
	void Update();
};


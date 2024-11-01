#include "Engine.h"
#include "GLApp.h"
#include "GLHelper.h"
#include "ModelManager.h"
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "GameStateManager.h"
#include "ComponentManager.h"
#include "Stage01_Lvl.h"
#include "MainEditor.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Stage02_Lvl.h"
#include "TimeManager.h"
#include "CollisionManager.h"
#include "TileEditor.h"

Engine::Engine()
{

}

Engine::~Engine()
{

}

bool Engine::Init(GLint _width, GLint _height, const std::string& _title)
{	
	if (!GLHelper::GetInstance()->Init(_width, _height, _title))
		return false;
	if (!GLApp::GetInstance()->Init())
		return false;	
	if (!ModelManager::GetInstance()->Init())
		return false;
	if (!RenderManager::GetInstance()->Init())
		return false;
	if (!TileEditor::GetInstance()->Init())	
		return false;	
	if (!GameStateManager::GetInstance()->ChangeLevel(new Stage01_Lvl("Stage01_Lvl")))
		return false;
	if (!CollisionManager::GetInstance()->Init())
		return false;
	return true;
}

bool Engine::Update()
{	
	GLFWwindow* window = GLHelper::GetInstance()->GetWindow();	

	glfwSwapBuffers(window);
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow(); // Show demo window! :)

	//TimeManager Udpate
	if (!TimeManager::GetInstance()->Update())
		return false;

	//GoManager Update
	if (!GameObjectManager::GetInstance()->Update())
		return false;	
	
	//LevelManager Update
	if(!GameStateManager::GetInstance()->Update())
		return false;
	
	//ComponentManager Udpate
	if (!ComponentManager::GetInstance()->Update())
		return false;

	//CollisionManager Update
	if (!CollisionManager::GetInstance()->Update())
		return false;

#ifndef _EDITOR
#define _EDITOR
	MainEditor::GetInstance()->Update();
#endif

	//RenderManger Update
	if (!RenderManager::GetInstance()->Update())
		return false;

	return true;
}

bool Engine::Draw()
{		
	if(!RenderManager::GetInstance()->Draw())
		return false;		
	return true;
}

bool Engine::Exit()
{		
	GLHelper::GetInstance()->Exit();
	GLApp::GetInstance()->Exit();
	GameStateManager::GetInstance()->Exit();	

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return true;
}
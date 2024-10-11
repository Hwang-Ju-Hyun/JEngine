#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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

Engine::Engine()
{
	std::cout << __FUNCTION__ << std::endl;
}

Engine::~Engine()
{
	std::cout << __FUNCTION__ << std::endl;
}

bool Engine::Init(GLint _width, GLint _height, const std::string& _title)
{	
	if (!GLHelper::GetInstance()->Init(_width, _height, _title))
		return false;
	//if (!GLApp::GetInstance()->Init())
	//	return false;
	//if (!ModelManager::GetInstance()->Init())
	//	return false;
	//if (!RenderManager::GetInstance()->Init())
	//	return false;
	//if (!GameStateManager::GetInstance()->ChangeLevel(new Stage01_Lvl))
	//	return false;
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

	//GoManager Update
	if (!GameObjectManager::GetInstance()->Update())
		return false;	
	
	//LevelManager Update
	if(!GameStateManager::GetInstance()->Update())
		return false;
	
	//ComponentManager Udpate
	if (!ComponentManager::GetInstance()->Update())
		return false;

	//RenderManger Update
	if (!RenderManager::GetInstance()->Update())
		return false;

	MainEditor::GetInstance()->Update();

	return true;
}

bool Engine::Draw()
{		
	if(!RenderManager::GetInstance()->Draw())
		return false;
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return true;
}

bool Engine::Exit()
{
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplGlfw_Shutdown();
	//ImGui::DestroyContext();
	
	GLHelper::GetInstance()->Exit();
	GLApp::GetInstance()->Exit();
	GameStateManager::GetInstance()->Exit();	
	return true;
}

#include "MainEditor.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include <iostream>
#include "BaseComponent.h"
#include "Transform.h"
#include "Serializer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//에디터에서 new level을 구현할 시 selected 꼭 nullptr로 만들자
MainEditor::MainEditor()
{

}


MainEditor::~MainEditor()
{

}


void MainEditor::TopBar()
{
	//auto all_obj = GameObjectManager::GetInstance()->GetAllObject();
	//std::cout<<all_obj.size()<<std::endl;
	ImGui::BeginMainMenuBar(); 

	ImGui::Text("This is the main menu %i",56);
	if (ImGui::Button("testbutton"))
	{
		static int temp_id = 100;
		//When i press the button, this happens
		std::cout << "pressed" << std::endl;
		GameObject* temp_obj = new GameObject("tempObject", temp_id++);
		Serializer::GetInstance()->SaveGameObject("json/GameObject/GameObject.json");
	}

	if (ImGui::BeginMenu("new gameobject"))
	{
		ImGui::Text("New GameObject Menu");

		char buffer[100] = { "\0" };

		ImGui::InputText("GO Name",buffer,100);

		if (ImGui::Button("Create"))
		{
			//GameObject* temp_obj=new GameObject("tempimgui",3);
			//std::cout << "GameObject Create!" << std::endl; //콘솔에 입력할 시 프로그램이 멈춤 왜지...
			//std::cout << GameObjectManager::GetInstance()->GetAllObject().size()<<std::endl;
		}		
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Load Level"))
	{
		ImGui::MenuItem("Load Level");
		ImGui::EndMenu();

	}


	ImGui::EndMainMenuBar();
}

void MainEditor::Update()
{
	TopBar();
	//ShowAllObject();
}

void MainEditor::ShowAllObject()
{		
	for (auto& comp : selected->GetAllComponentOfObj())
	{
		comp.second->Edit();
	}
}

void MainEditor::SelectedObjectWindow()
{
	ImGui::Begin("object list");
	for (auto a : GameObjectManager::GetInstance()->GetAllObject())
	{
		selected = a;
	}



	if (ImGui::TreeNode("AddComponent"))
	{
		//std::vector<std::string> comps = { Transform::GetName() };

		//for (auto compType : comps)
		//{
		//	if()
		//}

	}

	


	ImGui::End();
}

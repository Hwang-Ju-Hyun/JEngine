#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#include "GLHelper.h"
//#include "Engine.h"
#include <string>
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"


int main(void)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::string s = "aweawe";
	//if (!Engine::GetInstance()->Init(1980, 1020, "Hello World!"))
	//	return 0;
	//Engine::GetInstance()->Exit();





	//while (!glfwWindowShouldClose(GLHelper::GetInstance()->GetWindow()))
	//{	
	//	break;
	//	if (Engine::GetInstance()->Update())
	//		break;
	//	if (Engine::GetInstance()->Draw())
	//		break;
	//}
	//Engine::GetInstance()->Exit();

	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	return 0;
}
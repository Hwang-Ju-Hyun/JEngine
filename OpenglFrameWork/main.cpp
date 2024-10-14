#include "Engine.h"
#include "GLHelper.h"
#include <string>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


int main(void)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	if (!Engine::GetInstance()->Init(1980, 1020, "Hello World!"))
		return 0;
	while (!glfwWindowShouldClose(GLHelper::GetInstance()->GetWindow()))
	{			
		if (!Engine::GetInstance()->Update())
			break;
		if (!Engine::GetInstance()->Draw())
			break;
	}
	Engine::GetInstance()->Exit();

	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();
	return 0;
}
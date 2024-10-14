#include "GLHelper.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//#define _MOUSE_CURSOR
//#define _KEY
//#define _KEY_PRESSED

GLHelper::GLHelper()
{
    
}

GLHelper::~GLHelper()
{
    
}

GLFWwindow* GLHelper::GetWindow() const
{
    return m_ptrWindow;
}

GLint GLHelper::GetWindowWidth() const
{
    return m_giWidth;
}

GLint GLHelper::GetWindowHeight() const
{
    return m_giHeight;
}

std::string GLHelper::GetWindowTitleName() const
{
    return m_strTitleName;
}


void GLHelper::KeyCallBack(GLFWwindow* _window, int _key, int _scancod, int _action, int _mod)
{

    if (GLFW_PRESS == _action) 
    {
#ifdef _DEBUG
#ifdef _KEY
        std::cout << "Key pressed" << std::endl;    
#endif
#endif
    }
    else if (GLFW_REPEAT == _action) 
    {
#ifdef _DEBUG     
#ifdef _KEY
        std::cout << "Key repeatedly pressed" << std::endl;    
#endif
#endif
    }
    else if (GLFW_RELEASE == _action)
    {
#ifdef _DEBUG    
#ifdef _KEY
        std::cout << "Key released" << std::endl;    
#endif
#endif
        if (GLFW_KEY_ESCAPE == _key && GLFW_PRESS == _action)
            glfwSetWindowShouldClose(_window, GLFW_TRUE);
    }    
}

void GLHelper::MousePositionCallBack(GLFWwindow* _window, double _xpos, double _ypos)
{
#ifdef _DEBUG 
    #ifdef _MOUSE_CURSOR
    std::cout << "Mouse cursor position: (" << _xpos << ", " << _ypos << ")" << std::endl;
    #endif
#endif
}

void GLHelper::MousebuttonCallBack(GLFWwindow* _window, int _button, int _action, int _mod)
{
    switch (_button) 
    {
    case GLFW_MOUSE_BUTTON_LEFT:
#ifdef _DEBUG
#ifdef _KEY
        std::cout << "Left mouse button ";
#endif
#endif
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
#ifdef _DEBUG
#ifdef _KEY
        std::cout << "Right mouse button ";
#endif
#endif
        break;
    }

    switch (_action) 
    {
    case GLFW_PRESS:
#ifdef _DEBUG        
#ifdef _KEY_PRESSED
        std::cout << "pressed!!!" << std::endl;
#endif
#endif
        break;

    case GLFW_RELEASE:
#ifdef _DEBUG
#ifdef _KEY_PRESSED
        std::cout << "released!!!" << std::endl;
#endif
#endif
        break;
    }
}

void GLHelper::setup_event_callbacks()
{
    glfwSetKeyCallback(GLHelper::GetInstance()->GetWindow(),KeyCallBack);
    glfwSetCursorPosCallback(GLHelper::GetInstance()->GetWindow(), MousePositionCallBack);
    glfwSetMouseButtonCallback(GLHelper::GetInstance()->GetWindow(), MousebuttonCallBack);
}

bool GLHelper::Init(GLint _width, GLint _height, const std::string& _title)
{
    m_giWidth = _width;
    m_giHeight = _height;
    m_strTitleName = _title;
    
    if (!glfwInit())
    {
        std::cerr << "Error : glfwInit is false - GLHelper::Init" << std::endl;
        return false;
    }   
      
    m_ptrWindow = glfwCreateWindow(_width, _height, _title.c_str(), NULL,NULL);
    
    if (!m_ptrWindow)
    {
        std::cerr<< "Error : window is nullptr - GLHelper::Init" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(m_ptrWindow);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        glfwTerminate();
        std::cerr << "Error! : glewInit is false - GLHelper::Init" << std::endl;
        std::cerr << "Error Message : " << glewGetErrorString(err) << std::endl;                
        return false;
    }    
    
    glfwSetKeyCallback(m_ptrWindow, KeyCallBack);
    glfwSetMouseButtonCallback(m_ptrWindow, MousebuttonCallBack);
    glfwSetCursorPosCallback(m_ptrWindow, MousePositionCallBack);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls	
    
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_ptrWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    return true;
}

bool GLHelper::Exit()
{
    glfwTerminate();
    return true;
}

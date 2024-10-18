#include "GLHelper.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define _MOUSE_CURSOR
#define _KEY
//#define _KEY_PRESSED

glm::vec2 GLHelper::m_vMouseCursorPosition = { 0.f,0.f };
GLboolean GLHelper::m_bLeftMouseTriggered = false;
GLboolean GLHelper::m_bLeftMouseReleased = false;
GLboolean GLHelper::m_bRightMouseTriggered = false;
GLboolean GLHelper::m_bLeftControlKeyPressed = false;
GLboolean GLHelper::m_bLeftControlKeyReleased = false;

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
    if (_action == GLFW_PRESS && _key == GLFW_KEY_LEFT_CONTROL)
    {     
        m_bLeftControlKeyPressed = true;
    }
    if (m_bLeftControlKeyPressed && _action == GLFW_RELEASE)
    {        
        m_bLeftControlKeyPressed = false;
    }        
   
    /*if (GLFW_PRESS == _action) 
    {
#ifdef _DEBUG
#ifdef _KEY_PRESSED        
        std::cout << "Key pressed" << std::endl;    
#endif
#endif
    }
    else if (GLFW_REPEAT == _action) 
    {
#ifdef _DEBUG     
#ifdef _KEY_PRESSED
        std::cout << "Key repeatedly pressed" << std::endl;    
#endif
#endif
    }
    else if (GLFW_RELEASE == _action)
    {
#ifdef _DEBUG    
#ifdef _KEY_PRESSED

        std::cout << "Key released" << std::endl;    
#endif
#endif
        if (GLFW_KEY_ESCAPE == _key && GLFW_PRESS == _action)
            glfwSetWindowShouldClose(_window, GLFW_TRUE);
    }    */
}

void GLHelper::MousePositionCallBack(GLFWwindow* _window, double _xpos, double _ypos)
{           
#ifdef _DEBUG 
    #ifdef _MOUSE_CURSOR
        m_vMouseCursorPosition.x = _xpos;
        m_vMouseCursorPosition.y = _ypos;
        //std::cout << "Mouse cursor position: (" << _xpos << ", " << _ypos << ")" << std::endl;
    #endif
#endif
}

void GLHelper::MousebuttonCallBack(GLFWwindow* _window, int _button, int _action, int _mod)
{
    //if (_button == GLFW_MOUSE_BUTTON_LEFT && _action == GLFW_PRESS)
    //{
    //    m_bLeftMouseTriggered = true;
    //    //std::cout << "Left mouse button  press"<<std::endl;
    //}
    //if (m_bLeftMouseTriggered && _action == GLFW_RELEASE)
    //{
    //    m_bLeftMouseTriggered = false;
    //    //std::cout << "Left mouse button released "<<std::endl;
    //}
    if (_button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (_action == GLFW_PRESS)
        {
            if (!m_bLeftMouseTriggered) 
            {
                m_bLeftMouseTriggered = true;               
            }
        }
        else if (_action == GLFW_RELEASE)
        {
            if (m_bLeftMouseTriggered)
            {
                m_bLeftMouseTriggered = false;
                m_bLeftMouseReleased = true;                
            }
            else
            {                
                m_bLeftMouseReleased = false;
            }
        }
    }    
}

void GLHelper::setup_event_callbacks()
{
    glfwSetKeyCallback(GLHelper::GetInstance()->GetWindow(),KeyCallBack);
    glfwSetCursorPosCallback(GLHelper::GetInstance()->GetWindow(), MousePositionCallBack);
    glfwSetMouseButtonCallback(GLHelper::GetInstance()->GetWindow(), MousebuttonCallBack);
}

glm::vec2 GLHelper::GetMouseCursorPosition() const
{
    return m_vMouseCursorPosition;
}

GLboolean GLHelper::GetLeftMouseTriggered() const
{
    return m_bLeftMouseTriggered;
}

GLboolean GLHelper::GetLeftMouseReleased() const
{
    return m_bLeftMouseReleased;
}



GLboolean GLHelper::GetRightMouseTriggered() const
{
    return m_bRightMouseTriggered;
}

void GLHelper::ResetLeftMouseTriggered() const
{
    m_bLeftMouseTriggered = false;
}

GLboolean GLHelper::GetLeftControlPressed() const
{
    return m_bLeftControlKeyPressed;
}

glm::mat3 GLHelper::GetScreenToWorldMatFromMouse()
{
    glm::mat3 Inversed_Trans =
    {
        1,0,0,
        0,1,0,
        -window_width / 2.f,-window_height / 2.f,1.f
    };
    glm::mat3 X_axis_Revert =
    {
        1,  0,  0,
        0, -1,  0,
        0,  0,  1
    };

    glm::mat3 ScreenMouseMat =
    {
        1,0,0,
        0,1,0,
        m_vMouseCursorPosition.x,m_vMouseCursorPosition.y,1
    };
    m_mScreenToWorld = X_axis_Revert* Inversed_Trans *ScreenMouseMat;
    m_mScreenToWorld *= X_axis_Revert;
    return m_mScreenToWorld;
}

glm::mat3 GLHelper::GetScreenToWorldMat(glm::vec2 _pos)
{    
    glm::mat3 Inversed_Trans =
    {
        1,0,0,
        0,1,0,
        -window_width / 2.f,-window_height / 2.f,1.f
    };
    glm::mat3 X_axis_Revert =
    {
        1,  0,  0,
        0, -1,  0,
        0,  0,  1
    };

    glm::mat3 ScreenPosMat =
    {
        1,0,0,
        0,1,0,
        _pos.x,_pos.y,1
    };
    auto mat = X_axis_Revert * Inversed_Trans * ScreenPosMat;
    mat *= X_axis_Revert;
    return mat;
}

float GLHelper::GetRadianFromTwoVectors(glm::vec2 _vec1, glm::vec2 _vec2)
{
    //I will no use cos function <- It's expensive function
    //By using the normalize   
    return std::cosf((_vec1.x * _vec2.x) + (_vec2.y * _vec2.y) / std::sqrt(std::pow(_vec1.x, 2) + std::pow(_vec2.y, 2)));
}

void GLHelper::NormalizeVector(glm::vec2* _vec)
{
    float length = std::sqrt(std::pow(_vec->x, 2) + std::pow(_vec->y, 2));
    _vec->x /= length;
    _vec->y /= length;
}

bool GLHelper::IsPointInsideRectangle(glm::vec2 _pos, float _RecLeft, float _RecRight, float _RecTop, float _RecBottom, bool _IsWorldCord)
{
    glm::mat3 mat = GetScreenToWorldMat(_pos);        
    glm::vec2 position;
    if (!_IsWorldCord)    
         position = { mat[2][0],mat[2][1] };    
    else    
        position = _pos;    
    if (position.x >= _RecLeft && position.x <= _RecRight
        && position.y <= _RecTop && position.y >= _RecBottom)
    {
        return true;
    }
    return false;
}

bool GLHelper::IsPointInsideTriangle(glm::vec2 _pos, glm::vec2 _vertex1, glm::vec2 _vertex2, glm::vec2 _vertex3, bool _IsWorldCord)
{
    glm::mat3 mat = GetScreenToWorldMat(_pos);
    glm::vec2 position;
    if (!_IsWorldCord)
        position = { mat[2][0],mat[2][1] };
    else
        position = _pos;

    glm::vec2 ortho_vector1 = { _vertex2.y - _vertex1.y,-_vertex2.x + _vertex1.x };
    glm::vec2 ortho_vector2 = { _vertex3.y - _vertex2.y,-_vertex3.x + _vertex2.x };
    glm::vec2 ortho_vector3 = { _vertex1.y - _vertex3.y,-_vertex1.x + _vertex3.x };

    glm::vec2 point_vector1 = { position.x - _vertex1.x,position.y - _vertex1.y };
    glm::vec2 point_vector2 = { position.x - _vertex2.x,position.y - _vertex2.y };
    glm::vec2 point_vector3 = { position.x - _vertex3.x,position.y - _vertex3.y };
        

    float angle1 = std::acosf(GetRadianFromTwoVectors(ortho_vector1, point_vector1));
    float angle2 = std::acosf(GetRadianFromTwoVectors(ortho_vector2, point_vector2));
    float angle3 = std::acosf(GetRadianFromTwoVectors(ortho_vector3, point_vector3));

    if (angle1 >= 0 && angle2 >= 0 && angle3 >= 0)
    {
        return true;
    }
    return false;
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

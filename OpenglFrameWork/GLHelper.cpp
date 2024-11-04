#include "GLHelper.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GameObject.h"
#include "Transform.h"

#define _MOUSE_CURSOR
#define _KEY
//#define _KEY_PRESSED

glm::vec2 GLHelper::m_vMouseCursorPosition = { 0.f,0.f };
GLboolean GLHelper::m_bLeftMouseTriggered       = false;
GLboolean GLHelper::m_bLeftMouseReleased        = false;
GLboolean GLHelper::m_bRightMouseTriggered      = false;
GLboolean GLHelper::m_bRightMouseReleased       = false;

GLboolean GLHelper::m_bLeftControlKeyPressed    = false;
GLboolean GLHelper::m_bLeftControlKeyReleased   = false;
GLboolean GLHelper::m_bLeftAltKeyPressed        = false;
GLboolean GLHelper::m_bLeftAltKeyReleased       = false;
GLboolean GLHelper::m_bUpArrowKeyPressed        = false;
GLboolean GLHelper::m_bUpArrowKeyReleased       = false;
GLboolean GLHelper::m_bDownArrowKeyPressed      = false;
GLboolean GLHelper::m_bDownArrowKeyReleased     = false;
GLboolean GLHelper::m_bLeftArrowKeyPressed      = false;
GLboolean GLHelper::m_bLeftArrowKeyReleased     = false;
GLboolean GLHelper::m_bRightArrowKeyPressed     = false;
GLboolean GLHelper::m_bRightArrowKeyReleased    = false;
GLboolean GLHelper::m_bSpaceKeyPressed          = false;
GLboolean GLHelper::m_bSpaceKeyReleased         = false;

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
    //This code looks like yandere simulator?!
    if (_action == GLFW_PRESS)
    {     
        switch (_key)
        {
        case GLFW_KEY_UP:
            m_bUpArrowKeyPressed = true;
            break;
        case GLFW_KEY_DOWN:
            m_bDownArrowKeyPressed = true;
            break;
        case GLFW_KEY_LEFT:
            m_bLeftArrowKeyPressed = true;
            break;
        case GLFW_KEY_RIGHT:
            m_bRightArrowKeyPressed = true;
            break;
        case GLFW_KEY_SPACE:
            m_bSpaceKeyPressed = true;
            break;
        case GLFW_KEY_LEFT_CONTROL:
            m_bLeftControlKeyPressed = true;
        case GLFW_KEY_LEFT_ALT:
            m_bLeftAltKeyPressed = true;
        default:
            break;
        }
    }    
    else if(_action==GLFW_RELEASE)
    {
        if (m_bUpArrowKeyPressed)
        {
            m_bUpArrowKeyPressed = false;
            m_bUpArrowKeyReleased = true;
        }            
        if (m_bDownArrowKeyPressed)
        {
            m_bDownArrowKeyPressed = false;
            m_bDownArrowKeyReleased = true;
        }            
        if (m_bLeftArrowKeyPressed)
        {
            m_bLeftArrowKeyPressed = false;
            m_bLeftArrowKeyReleased = true;
        }
        if (m_bRightArrowKeyPressed)
        {
            m_bRightArrowKeyPressed = false;
            m_bRightArrowKeyReleased = true;
        }
        if (m_bSpaceKeyPressed)
        {
            m_bSpaceKeyPressed = false;
            m_bSpaceKeyReleased = true;
        }            
        if (m_bLeftControlKeyPressed)
        {
            m_bLeftControlKeyPressed = false;
            m_bLeftControlKeyReleased = true;
        }
        if (m_bLeftAltKeyPressed)
        {
            m_bLeftAltKeyPressed = false;
            m_bLeftAltKeyReleased = true;
        }
    }
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
    else if (_button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (_action == GLFW_PRESS)
        {
            if (!m_bRightMouseTriggered)
            {
                m_bRightMouseTriggered = true;
            }
        }
        else if (_action == GLFW_RELEASE)
        {
            if (m_bRightMouseTriggered)
            {
                m_bRightMouseTriggered = false;
                m_bRightMouseReleased = true;
            }
            else
            {
                m_bRightMouseReleased = false;
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

GLboolean GLHelper::GetRightMouseReleased() const
{
    return m_bRightMouseReleased;
}

void GLHelper::ResetLeftMouseTriggered() const
{
    m_bLeftMouseTriggered = false;
}

GLboolean GLHelper::GetAltPressed() const
{
    return m_bLeftAltKeyPressed;
}

GLboolean GLHelper::GetAltReleased() const
{
    return m_bLeftAltKeyReleased;
}

GLboolean GLHelper::GetLeftControlPressed() const
{
    return m_bLeftControlKeyPressed;
}

GLboolean GLHelper::GetLeftControlReleased() const
{
    return m_bLeftControlKeyReleased;
}

GLboolean GLHelper::GetUpArrowKeyPressed() const
{
    return m_bUpArrowKeyPressed;
}

GLboolean GLHelper::GetUpArrowKeyReleased() const
{
    return m_bUpArrowKeyReleased;
}

GLboolean GLHelper::GetDownArrowKeyPressed() const
{
    return m_bDownArrowKeyPressed;
}

GLboolean GLHelper::GetDownArrowKeyReleased() const
{
    return m_bDownArrowKeyReleased;
}

GLboolean GLHelper::GetLeftArrowKeyPressed() const
{
    return m_bLeftArrowKeyPressed;
}

GLboolean GLHelper::GetLeftArrowKeyReleased() const
{
    return m_bLeftArrowKeyReleased;
}

GLboolean GLHelper::GetRightArrowKeyPressed() const
{
    return m_bRightArrowKeyPressed;
}

GLboolean GLHelper::GetRightArrowKeyReleased() const
{
    return m_bRightArrowKeyReleased;
}

GLboolean GLHelper::GetSpaceKeyPressed() const
{
    return m_bSpaceKeyPressed;
}

GLboolean GLHelper::GetSpaceKeyReleased() const
{
    return m_bSpaceKeyReleased;
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

float GLHelper::GetDistanceFromTwoVertex(glm::vec2 _vertex1, glm::vec2 _vertex2)
{        
    return std::sqrt(std::pow(_vertex2.x - _vertex1.x, 2) + std::pow(_vertex2.y - _vertex1.y, 2));    
}

glm::vec2 GLHelper::GetDirectionFromTwoVector(glm::vec2 _vec1, glm::vec2 _vec2, bool _getNormalize)
{
    glm::vec2 dir = _vec2 - _vec1;
    if(_getNormalize)
        NormalizeVector(&dir);
    return dir;
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
    glm::vec2 position = _IsWorldCord ? _pos : glm::vec2(mat[2][0], mat[2][1]);


    float x1 = _vertex1.x, y1 = _vertex1.y;
    float x2 = _vertex2.x, y2 = _vertex2.y;
    float x3 = _vertex3.x, y3 = _vertex3.y;

    glm::vec2 ortho_v1 = { y2 - y1, -x2 + x1 };
    glm::vec2 ortho_v2 = { y3 - y2, -x3 + x2 };
    glm::vec2 ortho_v3 = { y1 - y3, -x1 + x3 };


    glm::vec2 PointAndOrtho_v1 = { position.x - x1, position.y - y1 };
    glm::vec2 PointAndOrtho_v2 = { position.x - x2, position.y - y2 };
    glm::vec2 PointAndOrtho_v3 = { position.x - x3, position.y - y3 };


    float dot1 = glm::dot(ortho_v1, PointAndOrtho_v1);
    float dot2 = glm::dot(ortho_v2, PointAndOrtho_v2);
    float dot3 = glm::dot(ortho_v3, PointAndOrtho_v3);
    
    if (dot1 <= 0 && dot2 <= 0 && dot3 <= 0)
        return true;
    return false;
}

bool GLHelper::IsPointInsideCircle(glm::vec2 _pointPos, glm::vec2 _circlePos, float _radius, bool _IsWorldCord)
{
    glm::mat3 mat = GetScreenToWorldMat(_pointPos);
    glm::vec2 position = _IsWorldCord ? _pointPos : glm::vec2(mat[2][0], mat[2][1]);

    if (_radius < GetDistanceFromTwoVertex(position,_circlePos))
        return false;

    return true;
}

glm::vec2 GLHelper::GetClosestPointRectangle(glm::vec2 _pointPos, GameObject* _rec)
{
    Transform* rec_trs = static_cast<Transform*>(_rec->FindComponent(Transform::TransformTypeName));
    glm::vec2 rec_pos = rec_trs->GetPosition();
    glm::vec2 rec_scale = rec_trs->GetScale();

    float left   = rec_pos.x - rec_scale.x / 2.f;
    float right  = rec_pos.x + rec_scale.x / 2.f;
    float top    = rec_pos.y + rec_scale.y / 2.f;
    float bottom = rec_pos.y - rec_scale.y / 2.f;

    float min_x_axis = left;
    float max_x_axis = right;
    float min_y_axis = bottom;
    float max_y_axis = top;

    if (_pointPos.x <= min_x_axis)    
        _pointPos.x = min_x_axis;
    else if(_pointPos.x >=max_x_axis)
        _pointPos.x = max_x_axis;
    
    if(_pointPos.y <=min_y_axis)
        _pointPos.y = min_y_axis;
    else if (_pointPos.y >= max_y_axis)
        _pointPos.y = max_y_axis;                

    glm::vec2 ClosestPoint;
    ClosestPoint = { _pointPos.x,_pointPos.y };

    return ClosestPoint;
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

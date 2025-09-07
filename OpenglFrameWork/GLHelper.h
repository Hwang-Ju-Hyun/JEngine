#pragma once
#include "single.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include "header.h"
#include <string>
#include <vector>
#include <unordered_map>
class GameObject;

class GLHelper
{
public:
	SINGLE(GLHelper);
private:
	std::string m_strTitleName="";
	GLint m_giWidth=0;
	GLint m_giHeight = 0;
	GLFWwindow* m_ptrWindow=nullptr;
public:	
	bool Init(GLint _width, GLint _height, const std::string& _title);
	bool Update();
	bool Exit();
public:
	GLFWwindow* GetWindow()const;
	GLint GetWindowWidth()const;
	GLint GetWindowHeight()const;
	std::string GetWindowTitleName()const;
public:
	static void KeyCallBack(GLFWwindow* _window,int _key, int _scancod, int _action, int _mod);
	static void MousePositionCallBack(GLFWwindow* _window, double _xpos, double _ypos);
	static void MousebuttonCallBack(GLFWwindow* _window, int _button, int _action, int _mod);
public:
	static void setup_event_callbacks();
private:
	//Mouse Info
	glm::mat3 m_mScreenToWorld = {};
	glm::mat3 m_mWorldToScreen = {};
	static glm::vec2 m_vMouseCursorPosition;
	static GLboolean m_bLeftMouseTriggered; 	
	static GLboolean m_bLeftMouseReleased;

	static GLboolean m_bRightMouseTriggered;
	static GLboolean m_bRightMouseReleased;
public:
	glm::vec2 GetMouseCursorPosition()const;
	GLboolean GetLeftMouseTriggered()const;
	GLboolean GetLeftMouseReleased()const;	
	GLboolean GetRightMouseTriggered()const;
	GLboolean GetRightMouseReleased()const;
	void ResetLeftMouseTriggered()const;
public:
	enum KEY_STATE
	{
		NOTHING = 0,
		PUSH = 1,
		REPEAT = 2,
		RELEASED = 3
	};

	struct KeyInfo
	{
		bool bHeld = false;      // 현재 눌려 있는 상태 (콜백에서 설정됨)
		bool bPrevHeld = false;  // 이전 프레임 눌림 여부
		KEY_STATE eState = KEY_STATE::NOTHING; // 최종 Key 상태 (Update()에서 계산됨)
	};	
	
	static enum KEY
	{
		LEFT_CONTROL=0,
		LEFT_ALT=1,
		W=2,
		A=3,
		D=4,
		S=5,
		UP=5,
		DOWN=6,
		LEFT=7,
		RIGHT=8,
		NUM0=9,
		SPACE=10,
		END=11
	};

	static std::unordered_map<int, KeyInfo> m_mKeyState;

	std::vector<std::pair<bool, KEY_STATE>> m_vecKeyInfo;
public:
	KEY_STATE GetKeyState(KEY _key)const;
private:
	//Keyboard Info
	//std::vector<bool> m_vKeyArrPrev;
	static std::vector<bool> m_vKeyArr;

	static bool m_bPrevPush;
	static GLboolean m_bLeftControlKeyPressed;
	static GLboolean m_bLeftControlKeyReleased;
	static GLboolean m_bLeftAltKeyPressed;
	static GLboolean m_bLeftAltKeyReleased;

	static GLboolean m_bWKeyPressed;
	static GLboolean m_bWKeyReleased;
	static GLboolean m_bAKeyPressed;
	static GLboolean m_bAKeyReleased;
	static GLboolean m_bDKeyPressed;
	static GLboolean m_bDKeyReleased;
	static GLboolean m_bSKeyPressed;
	static GLboolean m_bSKeyReleased;
	static GLboolean m_bSpaceKeyPressed;
	static GLboolean m_bSpaceKeyReleased;

	static GLboolean m_bUpArrowKeyPressed;
	static GLboolean m_bUpArrowKeyReleased;
	static GLboolean m_bDownArrowKeyPressed;
	static GLboolean m_bDownArrowKeyReleased;
	static GLboolean m_bLeftArrowKeyPressed;
	static GLboolean m_bLeftArrowKeyReleased;
	static GLboolean m_bRightArrowKeyPressed;
	static GLboolean m_bRightArrowKeyReleased;
	static GLboolean m_bNum0KeyPressed;
	static GLboolean m_bNum0KeyReleased;

public:
	void SetPrevPush(bool _key);
	bool GetPrevPush()const;

	GLboolean GetWKeyPressed()const;
	GLboolean GetWKeyReleased()const;
	GLboolean GetAKeyPressed()const;
	GLboolean GetAKeyReleased()const;
	GLboolean GetDKeyPressed()const;
	GLboolean GetDKeyReleased()const;
	GLboolean GetSKeyPressed()const;
	GLboolean GetSKeyReleased()const;

	GLboolean GetNum0KeyPressed()const;	
	GLboolean GetNum0KeyReleased()const;

	GLboolean GetAltPressed()const;
	GLboolean GetAltReleased()const;
	GLboolean GetLeftControlPressed()const;
	GLboolean GetLeftControlReleased()const;
	GLboolean GetUpArrowKeyPressed()const;
	GLboolean GetUpArrowKeyReleased()const;
	GLboolean GetDownArrowKeyPressed()const;
	GLboolean GetDownArrowKeyReleased()const;
	GLboolean GetLeftArrowKeyPressed()const;
	GLboolean GetLeftArrowKeyReleased()const;
	GLboolean GetRightArrowKeyPressed()const;
	GLboolean GetRightArrowKeyReleased()const;
	GLboolean GetSpaceKeyPressed()const;
	GLboolean GetSpaceKeyReleased()const;
public:
	inline KEY_STATE GetKetCode(int _key) { return m_mKeyState[_key].eState; }
	glm::mat3 GetScreenToWorldMatFromMouse();
	glm::mat3 GetScreenToWorldMat(glm::vec2 _pos);	
public:
	float GetDistanceFromTwoVertex(glm::vec2 _vertex1, glm::vec2 _vertex2);
	glm::vec2 GetDirectionFromTwoVector(glm::vec2 _vec1, glm::vec2 _vec2,bool _getNormalize=false);
	float GetRadianFromTwoVectors(glm::vec2 _vec1, glm::vec2 _vec2);	
	void NormalizeVector(glm::vec2* _vec);
public:
	bool IsPointInsideRectangle(glm::vec2 _pos, float _RecLeft, float _RecRight, float _RecTop, float _RecBottom, bool _IsWorldCord = true);
	bool IsPointInsideTriangle(glm::vec2 _pos, glm::vec2 _top,glm::vec2 _Lbottom,glm::vec2 _Rbottom, bool _IsWorldCord = true);
	bool IsPointInsideCircle(glm::vec2 _pointPos, glm::vec2 _circlePos, float _radius, bool _IsWorldCord);			
public:
	glm::vec2 GetClosestPointRectangle(glm::vec2 _pointPos, GameObject* _rec);
};


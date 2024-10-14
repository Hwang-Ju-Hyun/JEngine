#pragma once
#include "BaseComponent.h"
#include "json.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

class GameObject;

class Transform :
    public BaseComponent
{
public:
    Transform(GameObject* _owner);
    virtual ~Transform()override;
private:
	glm::vec2 m_vPosition = { 0.f,0.f };
	glm::vec2 m_vScale = { 0.f,0.f };
	float m_vRotate=0.f;
	glm::mat3 m_mModelToNDC = {};
public:
	//settor
	void SetPosition(glm::vec2 _pos);
	void AddPositionX(float _x);
	void AddPositionY(float _y);
	void AddPosition(glm::vec2 _pos);

	void SetScale(glm::vec2 _scale);
	void AddScale(glm::vec2 _scale);
public:
	//gettor
	glm::vec2 GetPositoin()const;
	glm::vec2 GetScale()const;
	glm::mat3 GetModelToNDC();
public:
    virtual void Update()override;
public:
	static BaseRTTI* CreateTransformComponent();
	static constexpr const char* TransformTypeName = "Transform";
public:
	virtual void LoadFromJson(const json& str)override;
	virtual json SaveToJson(const json& str)override;
public:
	bool EditFromImgui()override;
};


#pragma once
#include "BaseComponent.h"
#include <glm.hpp>

class GameObject;
class BaseLevel;
class Bomb;

class Player :
    public BaseComponent
{
public:
    Player(GameObject* _owner);
    virtual ~Player()override;
private:
    glm::vec2 m_vDirection = { 0.f,0.f };
public:
    void SetDirection(glm::vec2 _dir);
    glm::vec2 GetDirection()const;
private:
    int m_iHP = 0;
    BaseLevel* m_pCurrentLevel = nullptr;
    std::string m_sCurrentLevelName = "";
public:
    void SetCurrentLevel(BaseLevel* _level);
    BaseLevel* GetCurrentLevel()const;
public:
    void MoveMent();
    virtual void Update()override;
public:
    static constexpr const char* PlayerTypeName = "Player";
public:
    virtual void LoadFromJson(const json& _str);
    virtual json SaveToJson(const json& _str);
public:
    static BaseRTTI* CreatePlayerComponent();
public:
    void SpawBawn();
};


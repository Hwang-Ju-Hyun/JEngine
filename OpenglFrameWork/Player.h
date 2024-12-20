#pragma once
#include "BaseComponent.h"
#include <glm.hpp>

class GameObject;
class BaseLevel;
class Bomb;
class Transform;

class Player :
    public BaseComponent
{
public:
    Player(GameObject* _owner);
    virtual ~Player()override;
private:
    Transform* m_pPlayerTrs = nullptr;
private:
    glm::vec2 m_vDirection = { 0.f,0.f };    
public:
    void SetDirection(glm::vec2 _dir);
    glm::vec2 GetDirection()const;
private:
    Bomb* bomb_comp = nullptr;
    Transform* bomb_trs = nullptr;  
    Collision* m_pCol = nullptr;
private:
    int m_iHP = 0;
    int m_iMaxBombCnt = 0;
    int m_iCurBombCnt = 0;    
private:
    int m_iHP_temp = 0;
    int m_iMaxBombCnt_temp = 0;
    int m_iCurBombCnt_temp = 0;
public:
    void SetHp(int _hp);    
    int GetHp()const;
    void SetMaxBombCnt(int _cnt);
    void AddMaxBombCnt(int _cnt);
    int GetMaxBombCnt()const;    
public:
    void SetCurrentLevel(BaseLevel* _level);
    BaseLevel* GetCurrentLevel()const;
public:    
    void MoveMent();
    void Attack();
public:
    virtual void Update()override;
public:
    virtual void EventCollision(Collision* _pOther)override;
public:
    static constexpr const char* PlayerTypeName = "Player";
public:
    virtual void LoadFromJson(const json& _str);
    virtual json SaveToJson(const json& _str);
public:
    static BaseRTTI* CreatePlayerComponent();
private:
    BaseLevel* m_pCurrentLevel = nullptr;
    std::string m_sCurrentLevelName = "";
};


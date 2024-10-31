#pragma once
#include "BaseComponent.h"

class GameObject;

class Bomb :
    public BaseComponent
{
public:
    Bomb(GameObject* _owner);
    virtual ~Bomb()override;
private:
    static int id;
public:
    void SetBombMaster(GameObject* _master);
    GameObject* GetBombMaster()const;
private:
    GameObject* m_pBombMaster = nullptr;
    GameObject* m_pBomb=nullptr;
public:
    void SpawnBomb(GameObject* _bomb);
public:
    virtual void LoadFromJson(const json& _str)override;
    virtual json SaveToJson(const json& _str)override;
};


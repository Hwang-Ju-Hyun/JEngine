#pragma once
#include "BaseComponent.h"
#include <glm.hpp>

class GameObject;

class Player :
    public BaseComponent
{
public:
    Player(GameObject* _owner);
    virtual ~Player()override;
public:
    int m_iHP = 0;
public:
    virtual void Update()override;
    void MoveMent();
};


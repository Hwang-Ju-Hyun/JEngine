#pragma once
#include "BaseComponent.h"

class BaseEnemyState;

class AI :
    public BaseComponent
{
public:
    AI(GameObject* _owner);
    virtual ~AI()override;
private:
    std::string m_sCurState ="";
public:    
    virtual void Update();
public:
    void SetState(const std::string& _name);
    void ChangeState(BaseEnemyState* _state);
public:
    static constexpr const char* AITypeName = "AI";
public:
    static BaseRTTI* CreateAIComponent();
public:
    virtual void LoadFromJson(const json& _str)override;
    virtual json SaveToJson(const json& _str)override ;
};
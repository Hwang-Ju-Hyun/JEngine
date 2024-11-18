#pragma once
#include "BaseComponent.h"

class GameObject;
class BaseLevel;

class Bomb :
    public BaseComponent
{
public:
    Bomb(GameObject* _owner);
    virtual ~Bomb()override;
private:
    bool m_bIsExplode = false;
    float m_fRemaingTime = 0.f;
    float m_fExplodingTime = 0.f;
    int m_iBombRange = 1;
public:
    void SetBombRange(int _range);
    int  GetBombRange()const;
    void SetIsExplode(bool _explode);
    bool GetIsExplode()const;
    void SetRemainTime(float _time);
    void AddRemainTime(float _time);
    const float GetRemainTime()const;
    void SetExplodingTime(float _time);
    const float GetExplodingTime()const;
public:
    void Update();
public:
    BaseLevel* m_pCurrentLevel = nullptr;
private:
    std::vector<Bomb*> m_vecBombFragment;
    Collision* m_pCol = nullptr;
    bool m_bBombFrag = false;
public:
    virtual void EventCollision(Collision* _pOther)override;
public:
    virtual void LoadFromJson(const json& _str)override;
    virtual json SaveToJson(const json& _str)override;
public:
    static BaseRTTI* CreateBombComponent();
public:
    static constexpr const char* BombTypeName = "Bomb";
};
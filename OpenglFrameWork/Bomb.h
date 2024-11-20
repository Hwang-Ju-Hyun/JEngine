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
    bool m_bIsFragmenet = false;    
    bool m_bDoExplode = false;
public:
    void SetBombRange(int _range);
    int  GetBombRange()const;
    void SetIsExploding(bool _explode);
    bool GetIsExploding()const;
    void SetIsFragment(bool _frag);
    bool GetIsFragment()const;
    void SetRemainTime(float _time);
    void AddRemainTime(float _time);
    const float GetRemainTime()const;
    void SetExplodingTime(float _time);
    const float GetExplodingTime()const;
    void SetCanExplode(bool _explode);
    bool GetCanExplode()const;
public:
    void Update();
public:
    BaseLevel* m_pCurrentLevel = nullptr;
public:
    void CreateBombFragment(Bomb* _bomb);
private:
    //std::vector<GameObject*> m_vecBombFragment;
    Collision* m_pCol = nullptr;
    bool m_bBombFrag = false;
    float m_fBombFragExplodeTime = 0.f;        
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
#pragma once
#include "BaseComponent.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

class TextureResource;

class Sprite :
    public BaseComponent
{
public:
    Sprite(GameObject* _owner);
    virtual ~Sprite()override;
private:    
    glm::vec4 m_vColor = { 1.f,1.f,1.f,1.f };
    TextureResource* m_pTexture=nullptr;
public:
    void SetRed(float _red){ m_vColor[0] = _red; }
    void SetGreen(float _green){ m_vColor[1] = _green; }
    void SetBlue(float _blue){ m_vColor[2] = _blue; }
    void SetAlpha(float _alpha) { m_vColor[3] = _alpha; }

    float GetRed()const{ return    m_vColor[0]; }
    float GetBlue()const{ return   m_vColor[1];  }
    float GetGreen()const{ return  m_vColor[2]; }
    float GetAlpha()const { return m_vColor[3]; }
    void SetColor(glm::vec4 _vColor){ m_vColor = _vColor; }
    glm::vec4 GetColor()const{ return m_vColor; }
public:
    virtual void Update() override;
public:
    void SetTexture(TextureResource* _texture);
    TextureResource* GetTexture()const;
public:
    static BaseRTTI* CreateSpriteComponent();
    static constexpr const char* SpriteTypeName = "Sprite";
public:
    virtual void LoadFromJson(const json& str)override;
    virtual json SaveToJson(const json& str)override;
public:
    TextureResource* GetTextureFromImGui()const;
    bool EditFromImgui()override;
};
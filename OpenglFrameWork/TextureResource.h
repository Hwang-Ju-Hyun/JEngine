#pragma once
#include "Resource.h"
class TextureResource :
    public Resource
{
public:
    TextureResource(const std::string& _name);
    virtual ~TextureResource()override;
private:
    int m_iWidth=0;
    int m_iHeight=0;
    int m_iNrChannels=0;
    unsigned int m_iTextureID = 0;
public:
    int GetTextureWidth()const;
    int GetTextureHeight()const;
    int GetTextureChannels()const;
    unsigned int GetTextureID()const;
public:
    virtual void Load(const std::string& _path)override;
    virtual void UnLoad()override;
public:
    virtual unsigned char* GetData()const override;
};


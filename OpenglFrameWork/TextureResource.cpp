#include "TextureResource.h"
#include <iostream>
#include "GL/glew.h"
#include "stb_image.h"
TextureResource::TextureResource(const std::string& _name)
	:Resource(_name)
{
}

TextureResource::~TextureResource()
{
	UnLoad();
}

int TextureResource::GetTextureWidth() const
{
	return m_iWidth;
}

int TextureResource::GetTextureHeight() const
{
	return m_iHeight;
}

int TextureResource::GetTextureChannels() const
{
	return m_iNrChannels;
}

unsigned int TextureResource::GetTextureID() const
{
	return m_iTextureID;
}

void TextureResource::Load(const std::string& _path)
{
	m_cData = stbi_load(_path.c_str(), &m_iWidth, &m_iHeight, &m_iNrChannels, 0);	
	if (!m_cData)
	{
		std::cout << "Error : Can't load texture " << _path << std::endl;
		return;
	}	

	glGenTextures(1, &m_iTextureID);	
	glBindTexture(GL_TEXTURE_2D, m_iTextureID);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	if (m_cData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_cData);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}	
	stbi_image_free(m_cData);
	SetPath(_path);
	
	
}

void TextureResource::UnLoad()
{
	glDeleteTextures(1, &m_iTextureID);
	m_iTextureID = 0;
	m_iWidth = 0;
	m_iHeight = 0;
	m_iNrChannels = 0;
}

unsigned char* TextureResource::GetData() const
{
	return m_cData;
}

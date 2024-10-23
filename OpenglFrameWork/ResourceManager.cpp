#include "ResourceManager.h"
#include "Resource.h"
#include "TextureResource.h"
#include <iostream>


ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::RemoveAllRes()
{
	for (auto iter = m_mapResource.begin(); iter != m_mapResource.end();)
	{
		if (iter->second != nullptr)
		{			
			delete iter->second;
			iter = m_mapResource.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
}

ResourceManager::SELECT_EXTENSION ResourceManager::GetExtension(const std::string& _extension) const
{
	if (_extension=="png")
	{
		return SELECT_EXTENSION::png;
	}
	else if (_extension == "jpg")
	{
		return SELECT_EXTENSION::jpg;
	}
	else if (_extension == "wav")
	{
		return SELECT_EXTENSION::wav;
	}
	else if (_extension == "mp3")
	{
		return SELECT_EXTENSION::mp3;
	}
	else if (_extension == "ttf")
	{
		return SELECT_EXTENSION::ttf;
	}
	std::cerr << "extension is not valid" << std::endl;
}

std::map<std::string, Resource*> ResourceManager::GetAllResources() const
{
	return m_mapResource;
}

Resource* ResourceManager::GetAndLoad(const std::string& _resName, const std::string& _path)
{
	auto iter = m_mapResource.find(_resName);
	if (iter == m_mapResource.end())
	{
		std::string ext = _path.substr(_path.find_last_of(".") + 1);
		SELECT_EXTENSION extension = GetExtension(ext);

		Resource* resource = nullptr;
		switch (extension)
		{
		case ResourceManager::png:
			resource = new TextureResource(_resName);
			resource = static_cast<TextureResource*>(resource);
			m_mapResource.insert({ _resName,resource });
			break;
		case ResourceManager::jpg:
			resource = new TextureResource(_resName);
			resource = static_cast<TextureResource*>(resource);
			m_mapResource.insert({ _resName,resource });
			break;
		case ResourceManager::wav:
			break;
		case ResourceManager::mp3:
			break;
		case ResourceManager::ttf:
			break;
		default:
			break;
		}
		resource->Load(_path);
		return resource;
	}
	else
	{
		return iter->second;
	}
}

Resource* ResourceManager::FindResource(const std::string& _resName)
{	
	auto iter = m_mapResource.find(_resName);
	if (iter == m_mapResource.end())
	{
		std::cerr << "Error : Can't find Resource" << "(" << _resName << ") - ResourceManager::FindResource" << std::endl;
		return nullptr;
	}	
	return iter->second;
}
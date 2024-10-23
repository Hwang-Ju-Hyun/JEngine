#pragma once
#include <string>
class Resource
{
public:
	Resource() = delete;
	Resource(const std::string& _name);
	virtual ~Resource();
private:
	std::string m_sResName;	
	std::string m_sPath;
public:
	unsigned char* m_cData=nullptr;
public:
	virtual void Load(const std::string& _path) = 0;
	virtual void UnLoad() = 0;
public:
	virtual unsigned char* GetData()const;
public:
	void SetName(const std::string& _strName);
	const std::string& GetName()const;
	void SetPath(const std::string& _strPath);
	const std::string& GetPath()const;
};


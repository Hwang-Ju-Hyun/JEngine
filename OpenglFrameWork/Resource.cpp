#include "Resource.h"

Resource::Resource(const std::string&  _name)	
	:m_sResName(_name)
{
}

Resource::~Resource()
{
}

unsigned char* Resource::GetData() const
{
	return m_cData;
}

void Resource::SetName(const std::string& _strName)
{
	m_sResName = _strName;
}

const std::string& Resource::GetName() const
{
	return m_sResName;
}

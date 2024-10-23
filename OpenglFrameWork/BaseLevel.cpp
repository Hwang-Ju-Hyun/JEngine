#include "BaseLevel.h"

BaseLevel::BaseLevel(const std::string _name)
	:m_strName(_name)
{
}

BaseLevel::~BaseLevel()
{
}

bool BaseLevel::Init()
{
	return true;
}

bool BaseLevel::Update()
{
	return true;
}

bool BaseLevel::Exit()
{
	return true;
}

void BaseLevel::SetName(const std::string& _name)
{
	m_strName = _name;
}

const std::string& BaseLevel::GetName() const
{
	return m_strName;
}


#include "Registry.h"
#include "Transform.h"
#include "Sprite.h"

Registry::Registry()
{
    m_mapRttiMap.insert({ "Transform",&Transform::CreateTransformComponent });
    m_mapRttiMap.insert({ "Sprite",&Sprite::CreateSpriteComponent });
}

Registry::~Registry()
{

}

BaseRTTI* Registry::FindOrCreate(const std::string& _type)
{
    std::map<std::string, BaseRTTI* (*)()>::iterator iter =m_mapRttiMap.find(_type);
    if (iter != m_mapRttiMap.end())
    {
        if (iter->second == nullptr)
            return nullptr;
        return iter->second();
    }
    if (iter->second == nullptr)
        return nullptr;
    return nullptr;
}

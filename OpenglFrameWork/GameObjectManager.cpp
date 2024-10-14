#include "GameObjectManager.h"
#include "GameObject.h"
#include "ModelManager.h"
#include <iostream>

GameObjectManager::GameObjectManager()
{

}

GameObjectManager::~GameObjectManager()
{

}

GameObject* GameObjectManager::AddObject(GameObject* _obj)
{    
    if (_obj == nullptr)
    {
        std::cerr << "Error : Object is Invalid - GameObjectManager::AddObject" << std::endl;
        return nullptr;
    }

    m_vecGameObject.push_back(_obj);      

    int size = m_vecGameObject.size();


    return m_vecGameObject[size-1];
}

GameObject* GameObjectManager::FindObject(const std::string& _objName) const
{
    for (int i = 0; i < m_vecGameObject.size(); i++)
    {
        if (m_vecGameObject[i]->GetName() == _objName)        
            return m_vecGameObject[i];        
    }
    std::cerr << "Error : Can't find GameObject - GameObjectManager::FindObject" << std::endl;
    return nullptr;
}

GameObject* GameObjectManager::GetLastObject() const
{
    unsigned int size = m_vecGameObject.size();
    if (size == 0)
    {
        std::cerr << "Error : container of GameObject size is Zero  - GameObjectManager::GetLastObject" << std::endl;
        return nullptr;
    }
    return m_vecGameObject[size - 1];   
}

std::vector<GameObject*> GameObjectManager::GetAllObject() const
{
    return m_vecGameObject;
}

void GameObjectManager::RemoveAllObjects()
{
    for (auto iter=m_vecGameObject.begin();iter!=m_vecGameObject.end();)
    {
        if (*iter != nullptr)
        {
            delete* iter;            
            iter = m_vecGameObject.erase(iter);            
        }
        else
        {
            iter++;
        }
    }
    m_vecGameObject.clear();   
    //temp는 지역변수이므로 삭제된다
    std::vector<GameObject*> temp = m_vecGameObject; 
    temp.swap(m_vecGameObject);    
}

void GameObjectManager::RemoveObject(unsigned int _id)
{
    for (auto iter = m_vecGameObject.begin(); iter != m_vecGameObject.end();)
    {
        if ((*iter)->GetID()==_id)
        {
            delete* iter;
            iter = m_vecGameObject.erase(iter);
            return;
        }
        else
        {
            iter++;
        }
    }
}

bool GameObjectManager::Init()
{
    for (int i = 0; i < m_vecGameObject.size(); i++)    
        m_vecGameObject[i]->Init();    
    return true;
}

bool GameObjectManager::Update()
{
    float temp_dt = 0.f;
    for (int i = 0; i < m_vecGameObject.size(); i++)
    {
        m_vecGameObject[i]->Update(temp_dt);
    }
    return true;
}

bool GameObjectManager::Draw()
{
    for (int i = 0; i < m_vecGameObject.size(); i++)
    {
        m_vecGameObject[i]->Draw();
    }
    return true;
}

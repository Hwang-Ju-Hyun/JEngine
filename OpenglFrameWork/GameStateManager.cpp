#include "GameStateManager.h"
#include "BaseLevel.h"
#include <iostream>

GameStateManager::GameStateManager()
{

}

GameStateManager::~GameStateManager()
{

}

bool GameStateManager::Init()
{   
    if (m_pCurrentLevel == nullptr)
    {
        std::cerr << "Error : CurrentLevel is nullptr - GameStateManager::Init" << std::endl;
        return false;
    }
    m_pCurrentLevel->Init();
    return true;
}

bool GameStateManager::Update()
{
    if (m_pCurrentLevel == nullptr)
    {
        std::cerr << "Error : CurrentLevel is nullptr - GameStateManager::Update" << std::endl;
        return false;
    }
    m_pCurrentLevel->Update();
    return true;
}

bool GameStateManager::Exit()
{
    if (m_pCurrentLevel == nullptr)
    {
        //std::cerr << "Error : CurrentLevel is nullptr - GameStateManager::Exit" << std::endl;
        return true;
    }
    m_pCurrentLevel->Exit();
    return true;
}

bool GameStateManager::ChangeLevel(BaseLevel* _newLevel)
{    
    if (_newLevel == nullptr)
    {
        std::cerr << "Error : newLevel is nullptr - GameStateManager::ChangeLevel" << std::endl;
        Exit();
        delete m_pCurrentLevel;
        m_pCurrentLevel = nullptr;
        return false;
    }
    if (m_pPreviousLevel)
        delete m_pPreviousLevel;
    Exit();
    m_pPreviousLevel = m_pCurrentLevel;
    m_pCurrentLevel = _newLevel;
    m_pCurrentLevel->Init();
    return true;
}

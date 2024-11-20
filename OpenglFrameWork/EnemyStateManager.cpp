#include "EnemyStateManager.h"
#include "BaseEnemyState.h"
#include <iostream>

EnemyStateManager::EnemyStateManager()
{

}

EnemyStateManager::~EnemyStateManager()
{
    if (m_pPrevState)
        delete m_pPrevState;
    if (m_pCurState)    
        delete(m_pCurState);    
}

void EnemyStateManager::Init()
{
    if (m_pCurState == nullptr)
    {
        std::cerr << "Error : CurrentState is nullptr - StateManager::Init" << std::endl;
        return;
    }
    m_pCurState->Init();
}

void EnemyStateManager::Update()
{
    if (m_pCurState == nullptr)
    {
        std::cerr << "Error : CurrentState is nullptr - StateManager::Update" << std::endl;
        return;
    }
    m_pCurState->Update();
}

void EnemyStateManager::Exit()
{
    if (m_pCurState == nullptr)
    {
        std::cerr << "Error : CurrentState is nullptr - StateManager::Exit" << std::endl;
        return;
    }
    m_pCurState->Exit();
}

void EnemyStateManager::ChangeState(BaseEnemyState* _state)
{
    if (_state == nullptr)
    {
        std::cerr << "Error : ChangeState is nullptr - StateManager::ChangeState" << std::endl;
        Exit();
        delete m_pCurState;
        m_pCurState = nullptr;
        return;
    }
    
    if (m_pPrevState)
        delete m_pPrevState;

    Exit();

    
    m_pPrevState = m_pCurState;    
    m_pCurState = _state;
    m_pCurState->Init();
}

bool EnemyStateManager::ShouldExit()
{
	return m_pCurState == nullptr;
}

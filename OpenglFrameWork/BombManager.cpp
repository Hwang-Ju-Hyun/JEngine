#include "BombManager.h"
#include <iostream>

BombManager::BombManager()
{

}

BombManager::~BombManager()
{

}

void BombManager::AddBombFragment(GameObject* _bombFrag)
{
    if (_bombFrag == nullptr)
    {
        std::cerr << "Error : BombFragment object is nullptr - BombManager::AddBombFragment" << std::endl;
        return;
    }
    m_vecBombFragment.push_back(_bombFrag);
}

std::vector<GameObject*> BombManager::GetAllBombFragmenets()
{
    return m_vecBombFragment;
}

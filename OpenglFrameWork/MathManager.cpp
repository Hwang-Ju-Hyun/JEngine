#include "MathManager.h"

MathManager::MathManager()
{

}

MathManager::~MathManager()
{

}

float MathManager::GetDistBetweenVectors(glm::vec3 _vec1, glm::vec3 _vec2)
{
    return std::sqrt(std::pow((_vec2.x - _vec1.x), 2) + std::pow((_vec2.y - _vec1.y),2));
}

glm::vec3 MathManager::GetDistVectorBetweenVectors(glm::vec3 _vec1, glm::vec3 _vec2)
{
    glm::vec3 vec;
    vec.x = _vec2.x - _vec1.x;
    vec.y = _vec2.y - _vec1.y;
    return vec;
}

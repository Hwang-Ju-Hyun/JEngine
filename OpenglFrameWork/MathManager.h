#pragma once
#include "single.h"
#include <glm.hpp>

class MathManager
{
public:
	SINGLE(MathManager);
public:
	float GetDistBetweenVectors(glm::vec3 _vec1, glm::vec3 _vec2);
	glm::vec3 GetDistVectorBetweenVectors(glm::vec3 _vec1, glm::vec3 _vec2);
};


#pragma once
#include "single.h"
#include <vector>
class GLShader;

class ShaderManager
{
public:
	SINGLE(ShaderManager);
private:
	std::vector<GLShader> m_vecShader;
public:
	void AddShader(GLShader _shader);
	std::vector<GLShader> GetAllShader()const;
};


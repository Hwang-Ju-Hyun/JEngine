#pragma once
#include "header.h"
#include <fstream>

class GameObject;

class Serializer
{
public:
	SINGLE(Serializer);
public:
	GameObject* LoadGameObject(const std::string& _path);//TODO: ���߿� �Լ��̸� �� clear�ϰ� ������
	void SaveGameObject(const std::string& _path);
};


#pragma once
#include "header.h"
#include <fstream>

class GameObject;

class Serializer
{
public:
	SINGLE(Serializer);
public:
	GameObject* LoadGameObject(const std::string& _path,unsigned int _id);//TODO: ���߿� �Լ��̸� �� clear�ϰ� ������
	void SaveGameObject(const std::string& _path);
	int GetObjectSize(const std::string& _path);
};


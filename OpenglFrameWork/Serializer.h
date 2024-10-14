#pragma once
#include "header.h"
#include <fstream>

class GameObject;

class Serializer
{
public:
	SINGLE(Serializer);
public:
	GameObject* LoadGameObject(const std::string& _path,unsigned int _id);//TODO: 나중에 함수이름 더 clear하게 봐꾸자
	void SaveGameObject(const std::string& _path);
	int GetObjectSize(const std::string& _path);
};


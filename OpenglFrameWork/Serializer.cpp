#include "Serializer.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "BaseComponent.h"
#include "Registry.h"
#include <iostream>
#include <string>
#include "json.hpp"
#include "header.h"

using json = nlohmann::ordered_json;

Serializer::Serializer()
{

}

Serializer::~Serializer()
{

}

GameObject* Serializer::LoadGameObject(const std::string& _path, unsigned int _id)
{
	std::fstream file;
	file.open(_path, std::fstream::in);
	if (!file.is_open())
	{
		std::cerr << "Error : Can't open the file - Serializer::LoadGameObject" << std::endl;
		return nullptr;
	}

	json js_all_data;
	file >> js_all_data;
	for (auto& item : js_all_data)
	{
		auto obj = item.find("tempObject");
		if (obj != item.end()&&obj->begin().value()==_id)
		{
			//Create
			GameObject* go_obj = new GameObject("tempObject",_id);
			
			auto components = item.find("Components");
			
			if (components == item.end())
				continue;

			auto comp = *components;
			for (auto element : comp)
			{
				auto type = element.find("Type");
				if (type == element.end())
					continue;

				std::string type_name = (*type);

				BaseRTTI* p = Registry::GetInstance()->FindOrCreate(type_name);
				if (p != nullptr)
				{					
					p->LoadFromJson(element);
				}
					
			}
			auto shader_ref = item.find("Shader_Ref");
			go_obj->SetShaderRef(shader_ref->begin().value());
			auto model_type = item.find("Model_Type");
			go_obj->SetModelType(model_type->begin().value());

			file.close();
			return go_obj;
		}		
	}
	std::cerr << "Error : Can't find Object - Serializer::LoadGameobject" << std::endl;
	return nullptr;
}

void Serializer::SaveGameObject(const std::string& _path)
{
	auto all_objs=GameObjectManager::GetInstance()->GetAllObject();
	json js_all_data;
	for (const auto& go_obj : all_objs)
	{
		json js_components;
		if (go_obj->GetName() == "tempObject")
		{
			json js_obj;
			js_obj["tempObject"] = go_obj->GetID();;
			for (auto comp : go_obj->GetAllComponentOfObj())
			{
				BaseComponent* c = comp.second;
				js_components.push_back(c->SaveToJson(_path));
			}
			js_obj["Components"] = js_components;
			js_obj["Shader_Ref"] = go_obj->GetShaderRef();
			js_obj["Model_Type"] = go_obj->GetModelType();
			js_all_data.push_back(js_obj);			
		}
	}

	//Open file
	std::fstream file;
	file.open(_path, std::fstream::out);//Open as write mode. Create it if it does not exist!

	if (!file.is_open())
	{
		std::cerr << "Error : Can't open the file - Serializer::SaveGameObject" << std::endl;
		return;
	}

	//file << AllData; //All in 1 line
	file << std::setw(2) << js_all_data;

	file.close();
}

int Serializer::GetObjectSize(const std::string& _path)
{
	std::fstream file;
	file.open(_path, std::fstream::in);
	if (!file.is_open())
	{
		std::cerr << "Error : Can't open the file - Serializer::GetObjectSize" << std::endl;
		return -1;
	}
	json js_all_data;
	file >> js_all_data;
	int obj_count = 0;
	for (auto& item : js_all_data)
		obj_count++;
	if (obj_count == 0)
	{
		std::cerr << "Nothing object in "<<_path << std::endl;
		return -1;
	}
	return obj_count;
}

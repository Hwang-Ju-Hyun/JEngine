#include "Serializer.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "BaseComponent.h"
#include "Registry.h"
#include "TileEditor.h"
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::ordered_json;

Serializer::Serializer()
{

}

Serializer::~Serializer()
{

}

GameObject* Serializer::LoadGameObject(const std::string& _path)
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
		auto obj = item.find("GameObject");
		if (obj != item.end())
		{
			//Create
			GameObject* go_obj = new GameObject("GameObject", obj->begin().value());
			
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
		if (go_obj->GetName() == "GameObject")
		{
			json js_obj;
			js_obj["GameObject"] = go_obj->GetID();
			int a = go_obj->GetID();
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

GameObject* Serializer::LoadPlayer(const std::string& _path)
{
	std::fstream file;
	file.open(_path, std::fstream::in);
	if (!file.is_open())
	{
		std::cerr << "Error : Can't open the file - Serializer::LoadPlayer" << std::endl;
		return nullptr;
	}

	json js_all_data;
	file >> js_all_data;
	for (auto& item : js_all_data)
	{
		auto obj = item.find("Player");
		if (obj != item.end() )
		{
			//Create
			GameObject* go_obj = new GameObject("Player", obj->begin().value());

			auto components = item.find("Components");

			if (components == item.end())
				continue;

			auto comp = *components;
			for (auto element : comp)
			{
				auto type = element.find("Type");
				if (type == element.end())
					continue;
				int size =comp.size();
				size;
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
	std::cerr << "Error : Can't find Object - Serializer::LoadPlayer" << std::endl;
	return nullptr;
}

void Serializer::SavePlayer(const std::string& _path)
{
	auto all_objs = GameObjectManager::GetInstance()->GetAllObject();
	json js_all_data;
	for (const auto& go_obj : all_objs)
	{
		json js_components;
		if (go_obj->GetName() == "Player")
		{
			json js_obj;
			js_obj["Player"] = go_obj->GetID();					
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
		std::cerr << "Error : Can't open the file - Serializer::SavePlayer" << std::endl;
		return;
	}

	//file << AllData; //All in 1 line
	file << std::setw(2) << js_all_data;

	file.close();
}

void Serializer::LoadStage(const std::string& _path)
{
	std::fstream file_stream;
	file_stream.open(_path, std::fstream::in);
	if (!file_stream.is_open())
	{
		std::cerr << "Error: Can't open the file - Serializer::LoadStage" << std::endl;
		return;
	}
	std::string line;
	std::string stage_name = FindStageName(_path);
	while (!file_stream.eof())
	{
		std::getline(file_stream, line);
		LoadJson("json/" + stage_name + "/" + line);
	}
	file_stream.close();
}

GameObject* Serializer::CreateObjectFromJson(json _item)
{		
	auto Name_json = _item.find("Name");
	auto id_json = _item.find("ID");
	if (Name_json != _item.end())
	{
		std::string name = (*Name_json);
		int id = (*id_json);
		//Create
		GameObject* go_obj = new GameObject(name, id);

		auto components = _item.find("Components");

		if (components == _item.end())
		{
			std::cerr << "Error : " << go_obj->GetName() << "don't have components - Serializer::CreateObejectFromJson" << std::endl;
			return nullptr;
		}

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
		auto shader_ref = _item.find("Shader_Ref");
		go_obj->SetShaderRef(shader_ref->begin().value());
		auto model_type = _item.find("Model_Type");
		go_obj->SetModelType(model_type->begin().value());	

		return go_obj;
	}
	return nullptr;
}

GameObject* Serializer::LoadJson(const std::string& _path, bool _IsPrefabs)
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
	file.close();
		
	json Nothing;
	GameObject* temp=nullptr;
	for (auto& item : js_all_data)
	{
		if (_IsPrefabs)		
			return CreateObjectFromJson(item);		
		else		
			CreateObjectFromJson(item);		
	}	
	return temp;
}

bool comp_name_obj(GameObject* _obj1, GameObject* _obj2)
{
	if (_obj1->GetName() == _obj2->GetName())
		return false;
	if (_obj1->GetName() < _obj2->GetName())
		return true;
	return false;
}

bool comp_name_str(const std::string& _str1, const std::string& _str2)
{
	if (_str1 == _str2)
		return false;
	if (_str1 < _str2)
		return true;
	return false;
}



void Serializer::SaveJson(const std::string& _path)
{
	auto all_objs = GameObjectManager::GetInstance()->GetAllObject();
	json js_all_data;

	std::sort(all_objs.begin(), all_objs.end(),comp_name_obj);

	for(int i= m_iIndexForSave; i<all_objs.size(); i++)
	{	
		if (all_objs[i]->GetName() != m_strRecentlyName)
		{
			m_iIndexForSave = i;
			break;
		}
		json js_components;
		json js_obj;
		std::string name = m_strRecentlyName;
		unsigned int id = all_objs[i]->GetID();
		js_obj["Name"] = name;
		js_obj["ID"] = id;
		for (auto comp : all_objs[i]->GetAllComponentOfObj())
		{
			BaseComponent* c = comp.second;
			js_components.push_back(c->SaveToJson(_path));
		}
		js_obj["Components"] = js_components;
		js_obj["Shader_Ref"] = all_objs[i]->GetShaderRef();
		js_obj["Model_Type"] = all_objs[i]->GetModelType();		
		js_all_data.push_back(js_obj);
		
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

void Serializer::SaveStage(const std::string& _path)
{	
	std::fstream file_stream;
	file_stream.open(_path, std::fstream::in);
	if (!file_stream.is_open())
	{
		std::cerr << "Error: Can't open the file - Serializer::LoadStage" << std::endl;
		return;
	}
	std::string line;
	std::string stage_name = FindStageName(_path);
	while (!file_stream.eof())
	{
		std::getline(file_stream, line);
		m_vecJsonFile.push_back(line);
	}
	
	std::sort(m_vecJsonFile.begin(), m_vecJsonFile.end(), comp_name_str);
	file_stream.seekg(0);

	int index = 0;

	while (index<m_vecJsonFile.size())
	{		
		std::string object_name = SubstractObjectNameByFile(m_vecJsonFile[index]);
		m_strRecentlyName = object_name;
		SaveJson("json/" + stage_name + "/" + m_vecJsonFile[index]);
		index++;
	}
	m_strRecentlyName = "";
	file_stream.close();
}

std::string Serializer::FindStageName(const std::string& _path)
{
	std::string token;
	std::stringstream iss(_path);
	std::vector<std::string> vec_token;
	while (!iss.eof())
	{
		std::getline(iss, token, '/');
		vec_token.push_back(token);
	}
	int length = vec_token.size();	
	if (length - 2 < 0)
	{
		std::cerr << "Error : Can't Find StageName - Serializer::FindStageName" << std::endl;
		return "error";
	}
	return vec_token[length - 2];
}

std::string Serializer::SubstractObjectNameByFile(const std::string _path)
{
	std::string token;
	std::stringstream iss(_path);
	std::vector<std::string> vec_token;
	while (!iss.eof())
	{
		std::getline(iss, token, '.');
		vec_token.push_back(token);
	}
	return vec_token[0];
}

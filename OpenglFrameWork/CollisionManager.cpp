#include "CollisionManager.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Player.h"
#include "GLModel.h"
#include "Transform.h"
#include "Player.h"
#include "Serializer.h"
#include "MathManager.h"
#include <algorithm>
#include <iostream>

CollisionManager::CollisionManager()
{
	
}

CollisionManager::~CollisionManager()
{

}

bool CollisionManager::IsOverLapRectAndRect(float _left1, float _right1, float _top1, float _bot1, float _left2, float _right2, float _top2, float _bot2)
{
	if (_right1 > _left2 && _left1 < _right2 && _bot1<_top2 && _top1>_bot2)	
		return true;
	return false;
}

bool CollisionManager::IsOverLapConvexsOfProj(float _amax, float _bmax, float _amin, float _bmin)
{
	if ((_amin<_bmax && _amax>_bmin) || (_amax > _bmin && _amin < _bmax)
		|| (_amin == _bmin && _amax == _bmax))
	{
		return true;
	}
	return false;
}

void CollisionManager::HandlePosOnCollision(GameObject* _obj1, GameObject* _obj2)
{
	Transform* wall_trs = (Transform*)_obj1->FindComponent("Transform");
	Transform* obj_trs = (Transform*)_obj2->FindComponent("Transform");

	glm::vec2 wall_pos = wall_trs->GetPosition();
	glm::vec2 obj_pos = obj_trs->GetPosition();

	glm::vec2 wall_scale = wall_trs->GetScale();
	glm::vec2 obj_scale = obj_trs->GetScale();
		
	float upper_distance = std::fabs((wall_pos.y - (wall_scale.y / 2.f)) - (obj_pos.y + obj_scale.y/2.f));
	float down_distance = std::fabs((wall_pos.y  + (wall_scale.y / 2.f)) - (obj_pos.y - obj_scale.y / 2.f));
	float right_distance = std::fabs((wall_pos.x + (wall_scale.x / 2.f)) - (obj_pos.x - obj_scale.x / 2.f));
	float left_distance = std::fabs((wall_pos.x  - (wall_scale.x / 2.f)) - (obj_pos.x + obj_scale.x / 2.f));

	float arr_distance[4] = { upper_distance,down_distance,right_distance,left_distance };
	float min_distance = arr_distance[0];
	int direction = 0;
	for (int i = 0; i < 4; i++)
	{
		if (min_distance > arr_distance[i])
		{
			min_distance = arr_distance[i];
			direction = i;
		}
	}

	switch (direction)
	{
	case 0:
		obj_trs->AddPosition({ 0.f,-min_distance });
		break;
	case 1:
		obj_trs->AddPosition({ 0.f,min_distance });
		break;
	case 2:
		obj_trs->AddPosition({ min_distance,0.f});
		break;
	case 3:
		obj_trs->AddPosition({ -min_distance,0.f });
		break;
	default:
		break;
	}

}


bool CollisionManager::IsCollisionRectAndRect(GameObject* _obj1, GameObject* _obj2)
{	
	Transform* obj_trs1 = static_cast<Transform*>(_obj1->FindComponent("Transform"));
	Transform* obj_trs2 = static_cast<Transform*>(_obj2->FindComponent("Transform"));
	
	glm::vec2 obj1_Pos = static_cast<Transform*>(obj_trs1)->GetPosition();
	glm::vec2 obj2_Pos = static_cast<Transform*>(obj_trs2)->GetPosition();	

	glm::vec2 obj1_Scale = static_cast<Transform*>(obj_trs1)->GetScale();
	glm::vec2 obj2_Scale = static_cast<Transform*>(obj_trs2)->GetScale();	

	float obj1Left  = obj1_Pos.x - obj1_Scale.x / 2.f;
	float obj1Right = obj1_Pos.x + obj1_Scale.x / 2.f;
	float obj1Top   = obj1_Pos.y + obj1_Scale.y / 2.f;
	float obj1Bot   = obj1_Pos.y - obj1_Scale.y / 2.f;

	float obj2Left  = obj2_Pos.x - obj2_Scale.x / 2.f;
	float obj2Right = obj2_Pos.x + obj2_Scale.x / 2.f;
	float obj2TopY  = obj2_Pos.y + obj2_Scale.y / 2.f;
	float obj2BotY  = obj2_Pos.y - obj2_Scale.y / 2.f;

	if (IsOverLapRectAndRect(obj1Left, obj1Right, obj1Top, obj1Bot, obj2Left, obj2Right, obj2TopY, obj2BotY))
		return true;
    return false;
}

bool CollisionManager::IsCollisionConvexAndConvex(GameObject* _obj1, GameObject* _obj2)
{
	Transform* obj_trs1 = (Transform*)_obj1->FindComponent("Transform");
	Transform* obj_trs2 = (Transform*)_obj2->FindComponent("Transform");

	glm::vec2 obj1_pos = static_cast<Transform*>(obj_trs1)->GetPosition();
	glm::vec2 obj2_pos = static_cast<Transform*>(obj_trs2)->GetPosition();
	
	glm::vec2 obj1_scale= static_cast<Transform*>(obj_trs1)->GetScale();
	glm::vec2 obj2_scale = static_cast<Transform*>(obj_trs1)->GetScale();


	std::vector<glm::vec3> vertices1 = obj_trs1->GetOwner()->GetModel()->GetEachVertexPosition();
	std::vector<glm::vec3> vertices2 = obj_trs2->GetOwner()->GetModel()->GetEachVertexPosition();
	
	std::vector<glm::vec3> edges1= obj_trs1->GetOwner()->GetModel()->GetEdges();
	std::vector<glm::vec3> edges2 = obj_trs2->GetOwner()->GetModel()->GetEdges();

	std::vector<glm::vec3> ortho_vector;
	
	for (const auto& edge : edges1)	
		ortho_vector.push_back({ -edge.y, edge.x,0.f });

	for (const auto& edge : edges2)	
		ortho_vector.push_back({ -edge.y, edge.x,0.f });
			
	std::vector<float> invade_dist_vector;

	for(int i=0;i<ortho_vector.size();i++)
	{	
		float amin = 1e9;
		float amax = -1e9;
		float bmin = 1e9;
		float bmax = -1e9;

		for(int j=0;j<vertices1.size();j++)
		{
			//내적의 결과는 투영이다 왜냐하면 현재 vertex는 단위벡터로 정규화 되어있기 때문
			float dot = ((obj1_pos.x + obj1_scale.x * vertices1[j].x) * ortho_vector[i].x) + ((obj1_pos.y + obj1_scale.y * vertices1[j].y) * ortho_vector[i].y);
			if (amin > dot)
			{
				amin = dot;
			}			
			if (amax < dot)
			{				
				amax = dot;
			}
		}
		for(int j=0;j<vertices2.size();j++)
		{
			float dot = ((obj2_pos.x + obj2_scale.x * vertices2[j].x) * ortho_vector[i].x) + ((obj2_pos.y + obj2_scale.y * vertices2[j].y) * ortho_vector[i].y);
			if (bmin > dot)
			{
				bmin = dot;
			}
			if (bmax < dot)
			{
				bmax = dot;
			}
		}
		if(IsOverLapConvexsOfProj(amax,bmax,amin,bmin))
		{
			continue;
		}			
		else
			return false;		
	}
	return true;
}


void CollisionManager::HandlePosOnCollision2(GameObject* _triangle, GameObject* _rectangle)
{

}

bool CollisionManager::IsCollisionCirlceAndCircle(GameObject* _obj1, GameObject* _obj2)
{
	Transform* obj1_trs = static_cast<Transform*>(_obj1->FindComponent(Transform::TransformTypeName));
	Transform* obj2_trs = static_cast<Transform*>(_obj2->FindComponent(Transform::TransformTypeName));

	auto obj1_pos = obj1_trs->GetPosition();
	auto obj2_pos = obj2_trs->GetPosition();

	auto obj1_scale = obj1_trs->GetScale();
	auto obj2_scale = obj2_trs->GetScale();	
	
	int obj1_model_radius = obj1_scale.x;
	int obj2_model_radius = obj2_scale.y;
	
	auto math = MathManager::GetInstance();

	float dist = math->GetDistBetweenVectors(glm::vec3{ obj1_pos,0.f }, glm::vec3{ obj2_pos,0.f });
	if (dist >= obj1_model_radius + obj2_model_radius)	
		return false;
	return true;
}

bool CollisionManager::Init()
{	
	auto all_objs = GameObjectManager::GetInstance()->GetAllObject();
	for (auto obj : all_objs)
	{
		auto player_comp = obj->FindComponent(Player::PlayerTypeName);
		if (player_comp != nullptr)
			m_pPlayer = player_comp->GetOwner();
	}
	return true;
}

bool CollisionManager::Update()
{		
	auto all_objs = GameObjectManager::GetInstance()->GetAllObject();
	Transform* player_trs = static_cast<Transform*>(m_pPlayer->FindComponent(Transform::TransformTypeName));
	//std::cout << player_trs->GetPosition().x << "," << player_trs->GetPosition().y << std::endl;
	for (auto obj : all_objs)
	{		
		if (obj->GetName() == "WALL")
		{
			if (IsCollisionConvexAndConvex(obj, m_pPlayer))
			{
				HandlePosOnCollision2(m_pPlayer,obj);
				std::cout << "Collision" << std::endl;
			}
			else
			{
				std::cout << "Not Collision" << std::endl;
			}
		}
	}
	return true;
}

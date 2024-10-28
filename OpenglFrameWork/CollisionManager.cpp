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

bool CollisionManager::IsCollisionRectAndRect(GameObject* _obj1, GameObject* _obj2)
{	
	Transform* obj_trs1 = (Transform*)_obj1->FindComponent("Transform");
	Transform* obj_trs2 = (Transform*)_obj2->FindComponent("Transform");	
	
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
	
	for (const auto& axis : ortho_vector)
	{
		float amin = 1e9;
		float amax = -1e9;
		float bmin = 1e9;
		float bmax = -1e9;

		for (const auto& vertex : vertices1)
		{
			float dot = ((obj1_pos.x+obj1_scale.x*vertex.x) * axis.x) + ((obj1_pos.y+obj1_scale.y*vertex.y) * axis.y);
			amin=std::fmin(amin, dot);
			amax=std::fmax(amax, dot);
		}
		for (const auto& vertex : vertices2)
		{
			float dot = ((obj2_pos.x+obj2_scale.x * vertex.x) * axis.x) + ((obj2_pos.y+obj2_scale.y * vertex.y) * axis.y);
			bmin=std::fmin(bmin, dot);
			bmax=std::fmax(bmax, dot);
		}

		if ((amin<bmax && amax>bmin) || (amax > bmin && amin < bmax) ||
			(amin == bmin && amax == bmax))
		{
			continue;
		}			
		else
			return false;
	}
	return true;
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
	for (auto obj : all_objs)
	{		
	}
	return true;
}

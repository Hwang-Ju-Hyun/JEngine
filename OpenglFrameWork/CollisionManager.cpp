#include "CollisionManager.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "GLHelper.h"
#include "Player.h"
#include "GLModel.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Player.h"
#include "Bomb.h"
#include "Serializer.h"
#include "MathManager.h"
#include "Collision.h"
#include "EventManager.h"
#include "wall.h"
#include <algorithm>
#include <iostream>

CollisionManager::CollisionManager()
{
	
}

CollisionManager::~CollisionManager()
{

}

const std::vector<CollisionManager::HitBox*> CollisionManager::GetHitBoxes() const
{
	return m_vecHitBoxes;
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

void CollisionManager::HandlePosOnCollision_Rect_Rect(GameObject* _obj1, GameObject* _obj2)
{
	Transform* wall_trs = (Transform*)_obj1->FindComponent("Transform");
	Transform* obj_trs = (Transform*)_obj2->FindComponent("Transform");

	glm::vec2 wall_pos = wall_trs->GetPosition();
	glm::vec2 obj_pos = obj_trs->GetPosition();

	glm::vec2 wall_scale = wall_trs->GetScale();
	glm::vec2 obj_scale = obj_trs->GetScale();


	float upper_distance = std::fabs((wall_pos.y - (wall_scale.y / 2.f)) - (obj_pos.y + obj_scale.y / 2.f));
	float down_distance = std::fabs((wall_pos.y + (wall_scale.y / 2.f)) - (obj_pos.y - obj_scale.y / 2.f));
	float right_distance = std::fabs((wall_pos.x + (wall_scale.x / 2.f)) - (obj_pos.x - obj_scale.x / 2.f));
	float left_distance = std::fabs((wall_pos.x - (wall_scale.x / 2.f)) - (obj_pos.x + obj_scale.x / 2.f));

	float arr_distance[4] = { upper_distance, down_distance, right_distance, left_distance };
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
		obj_trs->AddPosition({ 0.f, -min_distance });
		break;
	case 1:		
		obj_trs->AddPosition({ 0.f, min_distance });
		break;
	case 2: 		
		obj_trs->AddPosition({ min_distance, 0.f });
		break;
	case 3: 		
		obj_trs->AddPosition({ -min_distance, 0.f });
		break;
	default:
		break;
	}
}
void CollisionManager::HandlePosOnCollision_Rect_Circle(GameObject* _obj1, GameObject* _obj2)
{
	GameObject* circle = nullptr;
	GameObject* rectangle = nullptr;
	auto helper = GLHelper::GetInstance();

	if (_obj2->GetModel()->GetName() == "Circle" && _obj1->GetModel()->GetName() == "Rectanlge")
	{
		circle = _obj2;
		rectangle = _obj1;
	}
	else
	{
		circle = _obj1;
		rectangle = _obj2;
	}

	Transform* circle_trs = static_cast<Transform*>(circle->FindComponent(Transform::TransformTypeName));
	Transform* rectangle_trs = static_cast<Transform*>(rectangle->FindComponent(Transform::TransformTypeName));

	glm::vec2 circle_pos = circle_trs->GetPosition();
	glm::vec2 rectangle_pos = rectangle_trs->GetPosition();

	glm::vec2 circle_scale = circle_trs->GetScale();
	glm::vec2 rectangle_scale = rectangle_trs->GetScale();

	//Todo : 이상함
	glm::vec2 closest_point_rect = helper->GetClosestPointRectangle(circle_pos, _obj2);
	float distance_two_obj = helper->GetDistanceFromTwoVertex(circle_pos, closest_point_rect);
	glm::vec2 direction = helper->GetDirectionFromTwoVector(circle_pos, rectangle_pos,true);
	
	glm::vec2 addpos = direction * distance_two_obj;
	
	rectangle_trs->AddPosition(addpos);
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

void CollisionManager::HandlePosByCollisionCheck_Convex_Convex(GameObject* _obj1, GameObject* _obj2)
{
	GameObject* obj1 = nullptr;
	GameObject* obj2 = nullptr;	

	if (_obj1->GetName() == "Wall")
	{
		obj1 = _obj2;
		obj2 = _obj1;
	}
	else if (_obj2->GetName() == "Wall")
	{
		obj1 = _obj1;
		obj2 = _obj2;
	}

	Transform* obj_trs1 = static_cast<Transform*>(obj1->FindComponent("Transform"));
	Transform* obj_trs2 = static_cast<Transform*>(obj2->FindComponent("Transform"));

	glm::vec2 obj1_pos = obj_trs1->GetPosition();
	glm::vec2 obj2_pos = obj_trs2->GetPosition();

	glm::vec2 obj1_scale = obj_trs1->GetScale();
	glm::vec2 obj2_scale = obj_trs2->GetScale();

	std::vector<glm::vec3> vertices1 = obj_trs1->GetOwner()->GetModel()->GetEachVertexPosition();
	std::vector<glm::vec3> vertices2 = obj_trs2->GetOwner()->GetModel()->GetEachVertexPosition();

	std::vector<glm::vec3> edges1 = obj_trs1->GetOwner()->GetModel()->GetEdges();
	std::vector<glm::vec3> edges2 = obj_trs2->GetOwner()->GetModel()->GetEdges();

	std::vector<glm::vec3> ortho_vector;
	
	for (const auto& edge : edges1) 
	{
		ortho_vector.push_back(glm::vec3(-edge.y, edge.x, 0.f));
	}
	for (const auto& edge : edges2) 
	{
		ortho_vector.push_back(glm::vec3(-edge.y, edge.x, 0.f));
	}

	float min_overlap = 1e9;
	glm::vec2 minimum_trans_vector_axis = { 0.f, 0.f };
	
	for (int i = 0; i < ortho_vector.size(); i++) 
	{
		float amin = 1e9, amax = -1e9;
		float bmin = 1e9, bmax = -1e9;


		for (int j = 0; j < vertices1.size(); j++) 
		{
			float dot = ((obj1_pos.x + obj1_scale.x * vertices1[j].x) * ortho_vector[i].x) +
				((obj1_pos.y + obj1_scale.y * vertices1[j].y) * ortho_vector[i].y);
			amin = std::min(amin, dot);
			amax = std::max(amax, dot);
		}


		for (int j = 0; j < vertices2.size(); j++) 
		{
			float dot = ((obj2_pos.x + obj2_scale.x * vertices2[j].x) * ortho_vector[i].x) +
				((obj2_pos.y + obj2_scale.y * vertices2[j].y) * ortho_vector[i].y);
			bmin = std::min(bmin, dot);
			bmax = std::max(bmax, dot);
		}

		
		float overlap_distance = std::min(amax, bmax) - std::max(amin, bmin);

		if (overlap_distance <= 0)
		{		
			return;
		}		
		if (overlap_distance < min_overlap) 
		{
			min_overlap = overlap_distance;
			minimum_trans_vector_axis = ortho_vector[i];
		}
	}
	
	glm::vec2 minimum_trans_vector = { minimum_trans_vector_axis.x * min_overlap , minimum_trans_vector_axis.y * min_overlap };

	glm::vec2 direction = obj2_pos - obj1_pos;

	//내적이 0보다 작으면	
	if (glm::dot(direction, minimum_trans_vector) < 0) 
	{
		minimum_trans_vector*=-1;
	}	
	obj_trs1->AddPosition(-minimum_trans_vector);
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

bool CollisionManager::IsCollisionCircleAndRect(GameObject* _obj1, GameObject* _obj2)
{
	//_obj1 원,_obj2 사각형
	GameObject* circle=nullptr;
	GameObject* rectangle=nullptr;
	auto helper = GLHelper::GetInstance();

	if (_obj2->GetModel()->GetName() == "Circle"&&_obj1->GetModel()->GetName()=="Rectanlge")
	{		
		circle = _obj2;
		rectangle = _obj1;
	}
	else
	{
		circle = _obj1;
		rectangle = _obj2;
	}


	Transform* circle_trs = static_cast<Transform*>(circle->FindComponent(Transform::TransformTypeName));
	Transform* rectangle_trs = static_cast<Transform*>(rectangle->FindComponent(Transform::TransformTypeName));

	glm::vec2 circle_pos = circle_trs->GetPosition();
	glm::vec2 rectangle_pos = rectangle_trs->GetPosition();

	glm::vec2 circle_scale = circle_trs->GetScale();
	glm::vec2 rectangle_scale = rectangle_trs->GetScale();
	

	glm::vec2 closest_point_rect = helper->GetClosestPointRectangle(circle_pos, _obj2);
	float distance_two_obj = helper->GetDistanceFromTwoVertex(circle_pos, closest_point_rect);
	
	float radius = circle_scale.y/2.f;
	if (radius > distance_two_obj)
		return true;
	return false;
}

bool CollisionManager::Init()
{	
	auto all_objs = GameObjectManager::GetInstance()->GetAllObject();
	for (auto obj : all_objs)
	{
		auto player_comp = obj->FindComponent(Player::PlayerTypeName);
		if (player_comp != nullptr)
			m_pPlayer = player_comp->GetOwner();
		auto bomb_comp = obj->FindComponent(Bomb::BombTypeName);		
	}

	for (int i = 0; i < all_objs.size(); i++)
	{
		if (all_objs[i]->GetName() == Wall::WallTypeName)
		{
			Collision* wall_col=static_cast<Collision*>(all_objs[i]->FindComponent(Collision::CollisionTypeName));
			m_vecWall_Col.push_back(wall_col);
		}
		else if (all_objs[i]->GetName() == Player::PlayerTypeName)
		{
			Collision* player_col = static_cast<Collision*>(all_objs[i]->FindComponent(Collision::CollisionTypeName));
			m_vecPlayer_Col.push_back(player_col);
		}
		else if (all_objs[i]->GetName() == Bomb::BombTypeName)
		{
			Collision* bomb_col = static_cast<Collision*>(all_objs[i]->FindComponent(Bomb::BombTypeName));
			m_vecBomb_Col.push_back(bomb_col);
		}
	}	

	return true;
}

bool CollisionManager::Update()
{			
	auto all_objs = GameObjectManager::GetInstance()->GetAllObject();
	Transform* player_trs = static_cast<Transform*>(m_pPlayer->FindComponent(Transform::TransformTypeName));

	auto all_walls_col_comp = GetWallCollisionVec();
	auto all_players_col_comp = GetPlayerCollisionVec();
	auto all_bombs_col_comp = GetBombCollisionVec();
	
	int walls_col_size = all_walls_col_comp.size();
	int player_col_size = all_players_col_comp.size();
	int bombs_col_size = all_bombs_col_comp.size();

	//player vs wall 
	for (int i = 0; i < player_col_size; i++)
	{
		for (int j = 0; j < walls_col_size; j++)
		{
			GameObject* player_obj = all_players_col_comp[i]->GetOwner();
			GameObject* wall_obj = all_walls_col_comp[j]->GetOwner();
			if (IsCollisionRectAndRect(player_obj,wall_obj))
			{
				auto player_comp = dynamic_cast<Player*>(player_obj->FindComponent(Player::PlayerTypeName));				
				if (player_comp!= nullptr)
				{
					player_comp->EventCollision(static_cast<Collision*>(wall_obj->FindComponent(Collision::CollisionTypeName)));					
				}								
			}
		}
	}

	//bomb vs wall
	for (int i = 0; i < bombs_col_size; i++)
	{
		for (int j = 0; j < walls_col_size; j++)
		{
			GameObject* bombs_obj = all_bombs_col_comp[i]->GetOwner();
			GameObject* wall_obj  = all_walls_col_comp[j]->GetOwner();
			if (bombs_obj != nullptr)
			{
				if (IsCollisionRectAndRect(bombs_obj, wall_obj))
				{
					auto bombs_comp = dynamic_cast<Bomb*>(bombs_obj->FindComponent(Bomb::BombTypeName));
					if (bombs_comp != nullptr)
					{
						bombs_comp->EventCollision(static_cast<Collision*>(wall_obj->FindComponent(Collision::CollisionTypeName)));
						break;
					}					
				}
			}			
		}
	}

	//player vs bomb
	for (int i = 0; i < player_col_size; i++)
	{
		for (int j = 0; j < bombs_col_size; j++)
		{
			GameObject* player_obj = all_players_col_comp[i]->GetOwner();
			GameObject* bombs_obj = all_bombs_col_comp[j]->GetOwner();
			if (IsCollisionRectAndRect(player_obj, bombs_obj))
			{
				auto player_comp = dynamic_cast<Player*>(player_obj->FindComponent(Player::PlayerTypeName));
				if (player_comp != nullptr)
				{
					player_comp->EventCollision(static_cast<Collision*>(bombs_obj->FindComponent(Collision::CollisionTypeName)));
				}
			}
		}
	}

	//bomb vs bomb
	/*for (int i = 0; i < bombs_col_size; i++)
	{
		for (int j = 0; j < bombs_col_size; j++)
		{
			GameObject* bombs_obj1 = all_bombs_col_comp[i]->GetOwner();
			GameObject* bombs_obj2 = all_bombs_col_comp[j]->GetOwner();
			if (IsCollisionRectAndRect(bombs_obj1, bombs_obj2))
			{
				auto bombs_comp = dynamic_cast<Bomb*>(bombs_obj1->FindComponent(Bomb::BombTypeName));
				if (bombs_comp != nullptr)
				{
					bombs_comp->EventCollision(static_cast<Collision*>(bombs_obj2->FindComponent(Collision::CollisionTypeName)));
				}
			}
		}
	}*/


	//player vs player
	
	return true;
}

std::vector<GameObject*> CollisionManager::GetCollsionVector() const
{
	return m_vecCollision;
}

void CollisionManager::AddCollisionObject(GameObject* _obj)
{
	m_vecCollision.push_back(_obj);
}

std::vector<Collision*> CollisionManager::GetWallCollisionVec() const
{
	return m_vecWall_Col;
}

std::vector<Collision*> CollisionManager::GetPlayerCollisionVec() const
{
	return m_vecPlayer_Col;
}

std::vector<Collision*> CollisionManager::GetBombCollisionVec() const
{
	return m_vecBomb_Col;
}

void CollisionManager::AddBombToBombColVec(Collision* _bombCol)
{
	m_vecBomb_Col.push_back(_bombCol);
}

void CollisionManager::RemoveBombCol(Collision* _bombCol)
{
	for (auto iter = m_vecBomb_Col.begin(); iter != m_vecBomb_Col.end(); iter++)
	{
		if (*iter == _bombCol)
		{
			m_vecBomb_Col.erase(iter);
			break;
		}
	}
}

void CollisionManager::RemoveWallCol(Collision* _wallCol)
{
	for (auto iter = m_vecWall_Col.begin(); iter != m_vecWall_Col.end(); iter++)
	{
		if (*iter == _wallCol)
		{
			m_vecWall_Col.erase(iter);
			break;
		}
	}
}

CollisionManager::HitBox::HitBox(GameObject* _obj1, GameObject* _obj2, bool _result)
{
	obj1 = _obj1;
	obj2 = _obj2;
	result = _result;
}

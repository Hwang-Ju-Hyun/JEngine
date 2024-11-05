#pragma once
#include "single.h"
#include <list>
#include <string>
#include <map>

class GameObject;

class Event
{
public:
	Event();
	virtual ~Event();
private:
	std::string m_sName;
public:
	void SetEventName(const std::string& _str);
	const std::string& GetEventName()const;
};

class OnCollision_Rect_Rect_Event :public Event
{
public:
	OnCollision_Rect_Rect_Event(GameObject* _obj1,GameObject* _obj2);
	virtual ~OnCollision_Rect_Rect_Event()override;
public:	
	GameObject* m_pLeftObj;
	GameObject* m_pRightObj;
public:
	static constexpr const char* EventName = "RectRectCollision";
};

class Entity
{
public:
	virtual void OnEvent(Event* _ev) = 0;
};


class HandelPosOnCollision_Rect_Rect :public Entity
{
public:	
	virtual void OnEvent(Event* _ev)override;
};





class EventManager
{
public:
	SINGLE(EventManager);
private:
	std::list<Event*> m_liEvents;
	std::map<std::string, std::list<Entity*>> m_mapRegisteredEntities;
public:
	void AddEvent(Event* _ev);
	void AddEntity(const std::string& _evName, Entity* _et);
	std::list<Entity*>* FindEntityList(const std::string& _evName);
	void RemoveEntity(const std::string& _evName, Entity* _et);
	void RemoveAllEvent();
public:
	void DispatchEvent(Event* _ev);
public:
	void Update();
};


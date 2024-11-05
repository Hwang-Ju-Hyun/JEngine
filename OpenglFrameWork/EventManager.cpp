#include "EventManager.h"
#include "CollisionManager.h"

EventManager::EventManager()
{		
}

EventManager::~EventManager()
{

}

void EventManager::AddEvent(Event* _ev)
{
	m_liEvents.push_back(_ev);
}

void EventManager::AddEntity(const std::string& _evName, Entity* _et)
{
	auto iter=m_mapRegisteredEntities.find(_evName);
	if (iter != m_mapRegisteredEntities.end())
	{
		std::list<Entity*>& subscribers = iter->second;
		subscribers.push_back(_et);
	}
	else
	{
		//새로운 이벤트
		std::list<Entity*> newPerson;
		newPerson.push_back(_et);
		m_mapRegisteredEntities.insert({ _evName,newPerson });
	}
}

std::list<Entity*>* EventManager::FindEntityList(const std::string& _evName)
{
	auto iter = m_mapRegisteredEntities.find(_evName);
	if (iter == m_mapRegisteredEntities.end())
		return nullptr;
	return &(iter->second);
}

void EventManager::RemoveEntity(const std::string& _evName, Entity* _et)
{
	std::list<Entity*>* subscribers = FindEntityList(_evName);
	if (subscribers)
	{
		auto iter = std::find(subscribers->begin(), subscribers->end(), _et);
		if (iter != subscribers->end())
		{
			subscribers->erase(iter);
		}
	}
}

void EventManager::RemoveAllEvent()
{
	auto iter = m_liEvents.begin();
	for (iter; iter != m_liEvents.end(); iter++)
	{
		delete* iter;
		*iter = nullptr;
	}
	m_liEvents.clear();
}

void EventManager::DispatchEvent(Event* _ev)
{
	if (_ev == nullptr)
		return;

	std::list<Entity*>* listEt = FindEntityList(_ev->GetEventName());

	if (listEt == nullptr)
		return;
	if (m_liEvents.size() <= 0)
	{
		return;
	}
	else
	{
		for (auto iter = listEt->begin(); iter != listEt->end(); iter++)
		{
			(*iter)->OnEvent(_ev);
		}
	}
}

void EventManager::Update()
{
	for (auto iter = m_liEvents.begin(); iter != m_liEvents.end(); iter++)
	{
		DispatchEvent(*iter);
		delete (*iter);
	}
	m_liEvents.clear();
}

Event::Event()
{
}

Event::~Event()
{
}

void Event::SetEventName(const std::string& _str)
{
	m_sName = _str;
}

const std::string& Event::GetEventName() const
{
	return m_sName;
}

OnCollision_Rect_Rect_Event::OnCollision_Rect_Rect_Event(GameObject* _obj1, GameObject* _obj2)
	:m_pLeftObj(_obj1),
	 m_pRightObj(_obj2)
{
	SetEventName(OnCollision_Rect_Rect_Event::EventName);
}

OnCollision_Rect_Rect_Event::~OnCollision_Rect_Rect_Event()
{
}

void HandelPosOnCollision_Rect_Rect::OnEvent(Event* _ev)
{
	OnCollision_Rect_Rect_Event* event = static_cast<OnCollision_Rect_Rect_Event*>(_ev);
	CollisionManager::GetInstance()->HandlePosOnCollision_Rect_Rect(event->m_pLeftObj, event->m_pRightObj);
}

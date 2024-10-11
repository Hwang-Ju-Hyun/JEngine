#pragma once
#include "header.h"


class GameObject;
class MainEditor
{
public:
	SINGLE(MainEditor);
public:
	void TopBar();
	void Update();
	void ShowAllObject();
	void SelectedObjectWindow();
	GameObject* selected = nullptr;
	enum EditorMode
	{
		PLAY,
		PAUSE,
		EXIT
	};
	EditorMode currentMdoe;
};


#include "Sprite.h"

Sprite::Sprite(GameObject* _owner)
	:BaseComponent(_owner)
{
	SetName("Sprite");
}

Sprite::~Sprite()
{
}

void Sprite::Update()
{
	
}
#pragma once
class BaseLevel
{
public:
	BaseLevel();
	virtual ~BaseLevel();
public:
	virtual bool Init()=0;
	virtual bool Update()=0;
	virtual bool Exit()=0;
	
};
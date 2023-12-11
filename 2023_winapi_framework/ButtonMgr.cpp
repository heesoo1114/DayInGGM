#include "pch.h"
#include "ButtonMgr.h"

void ButtonMgr::Init()
{
	selectedBtn = 0;
	isLeft = true;
}

void ButtonMgr::Render(HDC hdc)
{

}

void ButtonMgr::Left()
{
	selectedBtn = 1;
	isLeft = true;
}

void ButtonMgr::Right()
{
	selectedBtn = 2;
	isLeft = false;
}

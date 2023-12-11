#pragma once
class ButtonMgr
{
	SINGLE(ButtonMgr);
public:
	void Init();
	void Render(HDC hdc);
	void Left();
	void Right();
public :
	int selectedBtn = 0;
	bool isLeft = true;
};


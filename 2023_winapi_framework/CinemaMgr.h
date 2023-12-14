#pragma once
#include "define.h"

class CinemaMgr
{
	SINGLE(CinemaMgr);
public:
	void Init(HWND hWnd);
	void VideoStart(HWND hWnd, POINT point);
	void VideoChange(HWND hWnd, POINT point, int index);
	void VideoStop();
	void VideoResume();
public:
	HWND GetVideoHandle() { return m_hVideo; }
private:
	HWND m_hVideo;
public:
	bool isChoiceVideo = true;
	bool isPaused = false;
	int currentIndex = 3;
	LPCWSTR path;
	LPCWSTR pathList[19] =
	{
		L"Res\\Video\\1.wmv", 
		L"Res\\Video\\1.wmv",
		L"Res\\Video\\1.wmv",
		L"Res\\Video\\1.wmv", // 여기부터 시작인데
		L"Res\\Video\\2.wmv",
		L"Res\\Video\\3.wmv", 
		L"Res\\Video\\4.wmv",
		L"Res\\Video\\5.wmv",
		L"Res\\Video\\6.wmv",
		L"Res\\Video\\7.wmv",
		L"Res\\Video\\8.wmv",
		L"Res\\Video\\9.wmv",
		L"Res\\Video\\10.wmv",
		L"Res\\Video\\11.wmv",
		L"Res\\Video\\12.wmv",
		L"Res\\Video\\13.wmv",
		L"Res\\Video\\14.wmv",
		L"Res\\Video\\15.wmv",
		L"Res\\Video\\16.wmv",
	};
};


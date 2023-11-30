#include "pch.h"
#include "CinemaMgr.h"

void CinemaMgr::Init(HWND hWnd)
{
	// 비디오 윈도우 핸들에 세팅 및 
	path = pathList[0];
	m_hVideo = MCIWndCreate(hWnd, nullptr, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, path);
}

void CinemaMgr::VideoStart(HWND hWnd, POINT point)
{
	MoveWindow(m_hVideo, 100, 50, point.x - 200, point.y- 200, false);
	MCIWndPlay(m_hVideo);
}

void CinemaMgr::VideoChange(HWND hWnd, POINT point, int index)
{
	currentIndex += index; 
	path = pathList[currentIndex];

	MCIWndClose(m_hVideo);  // 기존에 재생 중이던 영상 닫기
	m_hVideo = MCIWndCreate(hWnd, NULL, WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR, path);
	MoveWindow(m_hVideo, 0, 0, point.x, point.y, false);
	
	VideoStart(hWnd, point);
}

void CinemaMgr::VideoStop()
{
	MCIWndPause(m_hVideo);
}

void CinemaMgr::VideoResume()
{
	MCIWndResume(m_hVideo);
}

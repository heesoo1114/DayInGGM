#include "pch.h"
#include "Core.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "CollisionMgr.h"
#include "EventMgr.h"
#include "CinemaMgr.h"

HWND m_hVideo;
bool Core::Init(HWND _hWnd, POINT _ptResolution)
{
	// === 변수 초기화 === 
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;
	m_hbackDC = 0;
	m_hbackbit = 0;


	// 더블버퍼링
	m_hDC = GetDC(m_hWnd);	
	// 1. 생성
	m_hbackbit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_hbackDC = CreateCompatibleDC(m_hDC);

	// 2. 연결
	SelectObject(m_hbackDC, m_hbackbit);

	CreateGDI();
	// ==== Manager Init ====
	PathMgr::GetInst()->Init();
	TimeMgr::GetInst()->Init();
	KeyMgr::GetInst()->Init();
	ResMgr::GetInst()->Init();
	SceneMgr::GetInst()->Init();
	PathMgr::GetInst()->GetResPath();
	CinemaMgr::GetInst()->Init(m_hWnd);

	// 여기서 영상을 틀어보자.
	CinemaMgr::GetInst()->VideoStart(m_hWnd, m_ptResolution);

	return true;
}

void Core::GameLoop()
{
	/*static int count = 0;
	++count;
	static int prev = GetTickCount64();
	int cur = GetTickCount64();
	if (cur - prev > 1000)
	{
		prev = cur;
		count = 0;
	}*/
	Update();
	// Render();
}

bool isPaused = false;
void Core::Update()
{
	// === Manager Update === 
	TimeMgr::GetInst()->Update();
	KeyMgr::GetInst()->Update();
	SceneMgr::GetInst()->Update();
	CollisionMgr::GetInst()->Update();
	// CinemaMgr::GetInst()->GetHandle();

	if (KEY_DOWN(KEY_TYPE::SPACE))
	{
		if (CinemaMgr::GetInst()->isPaused)
		{
			// 현재 재생 중이면 일시 정지
			CinemaMgr::GetInst()->VideoResume();
		}
		else
		{
			// 현재 일시 정지 상태이면 다시 재생
			CinemaMgr::GetInst()->VideoStop();
		}

		CinemaMgr::GetInst()->isPaused = !CinemaMgr::GetInst()->isPaused;  // 상태 변경
	}

	if (KEY_DOWN(KEY_TYPE::RIGHT))
	{
		if (!CinemaMgr::GetInst()->isPaused)
		{
			CinemaMgr::GetInst()->VideoChange(m_hWnd, m_ptResolution, 1);
		}
	}
	if (KEY_DOWN(KEY_TYPE::LEFT))
	{
		if (!CinemaMgr::GetInst()->isPaused)
		{
			CinemaMgr::GetInst()->VideoChange(m_hWnd, m_ptResolution, -1);
		}
	}

	// if (KEY_DOWN(KEY_TYPE::H))
	// {
	// 	float duration = MCIWndGetEnd(m_hVideo);
	// 	float position = MCIWndGetPosition(m_hVideo); 
	// 
	// 	if (position <= duration)
	// 	{
	// 		// CinemaMgr::GetInst()->VideoChange(m_hWnd, m_ptResolution, 0);
	// 		// CinemaMgr::GetInst()->VideoStart(m_hWnd, m_ptResolution);
	// 		CinemaMgr::GetInst()->VideoStop();
	// 	}
	// }

}

void Core::Render()
{
	// 칠한다.
	//Rectangle(m_hbackDC, -1,-1,m_ptResolution.x +1,m_ptResolution.y + 1);
	PatBlt(m_hbackDC, 0, 0, m_ptResolution.x, m_ptResolution.y, WHITENESS);

	SceneMgr::GetInst()->Render(m_hbackDC);
	/*Vec2 vPos = m_obj.GetPos();
	Vec2 vScale = m_obj.GetScale();
	RECT_RENDER(vPos.x, vPos.y, vScale.x, vScale.y, m_hbackDC);*/

	//// cursor test
	//POINT mousepos = KeyMgr::GetInst()->GetMousePos();
	//static wchar_t mousebuf[100] = {};
	//swprintf_s(mousebuf, L"Mouse: x %d, y: %d", mousepos.x, mousepos.y);
	//TextOut(m_hbackDC, 10, 10, mousebuf, wcslen(mousebuf));

	// 3. 옮긴다.
	BitBlt(m_hDC, 0,0, m_ptResolution.x, m_ptResolution.y, 
		m_hbackDC, 0,0, SRCCOPY);
	EventMgr::GetInst()->Update();
}

void Core::CreateGDI()
{
	// HOLLOW
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::RED] = (HBRUSH)CreateSolidBrush(RGB(255, 167, 167));
	m_arrBrush[(UINT)BRUSH_TYPE::GREEN] = (HBRUSH)CreateSolidBrush(RGB(134, 229, 134));
	m_arrBrush[(UINT)BRUSH_TYPE::BLUE] = (HBRUSH)CreateSolidBrush(RGB(103, 153, 255));
	m_arrBrush[(UINT)BRUSH_TYPE::YELLOW] = (HBRUSH)CreateSolidBrush(RGB(255, 187, 0));

	//RED GREEN BLUE PEN
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_arrPen[(UINT)PEN_TYPE::YELLOW] = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::HOLLOW] = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
}

void Core::Release()
{
	ReleaseDC(m_hWnd, m_hDC);
	DeleteDC(m_hbackDC); // createdc 한거 지우는거
	DeleteObject(m_hbackbit); // createbit 한거 지우는거
	for (int i = 0; i < (UINT)PEN_TYPE::END; ++i)
	{
		DeleteObject(m_arrPen[i]);
	}
	for (int i = 1; i < (UINT)BRUSH_TYPE::END; ++i)
	{
		DeleteObject(m_arrBrush[i]);
	}

	ResMgr::GetInst()->Release();
}

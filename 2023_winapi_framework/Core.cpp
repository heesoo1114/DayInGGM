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
#include "ButtonMgr.h"
#include "SelectGDI.h"

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
	ButtonMgr::GetInst()->Init();

	// 여기서 영상을 틀어보자.
	CinemaMgr::GetInst()->VideoStart(m_hWnd, m_ptResolution);

	return true;
}

void Core::GameLoop()
{
	Update();
	Render();
}

void Core::Update()
{
	// === Manager Update === 
	TimeMgr::GetInst()->Update();
	KeyMgr::GetInst()->Update();
	SceneMgr::GetInst()->Update();
	CollisionMgr::GetInst()->Update();

	// 오른쪽 버튼으로
	if (KEY_DOWN(KEY_TYPE::RIGHT))
	{
		ButtonMgr::GetInst()->Right();
	}
	// 왼쪽 버튼으로
	else if (KEY_DOWN(KEY_TYPE::LEFT))
	{
		ButtonMgr::GetInst()->Left();
	}

	// 버튼 선택에 따른 다음 영상으로
	if (KEY_DOWN(KEY_TYPE::SPACE))
	{
		int nextIndex = ButtonMgr::GetInst()->selectedBtn;
		int currentIndex = CinemaMgr::GetInst()->currentIndex;

		int passIndex = 1;
		
		// 선택이 진행되는 영상이면
		if (currentIndex % 3 == 0)
		{
			passIndex = nextIndex;
		}
		// A 영상이라면
		else if (currentIndex % 3 == 1)
		{
			passIndex = 2;
		}
		// B 영상이라면
		else if (currentIndex % 3 == 2)
		{
			passIndex = 1;
		}

		CinemaMgr::GetInst()->VideoChange(m_hWnd, m_ptResolution, passIndex);
	}

#pragma region 폐기
	// if (KEY_DOWN(KEY_TYPE::SPACE))
	// {
	// 	if (CinemaMgr::GetInst()->isPaused)
	// 	{
	// 		// 현재 재생 중이면 일시 정지
	// 		CinemaMgr::GetInst()->VideoResume();
	// 	}
	// 	else
	// 	{
	// 		// 현재 일시 정지 상태이면 다시 재생
	// 		CinemaMgr::GetInst()->VideoStop();
	// 	}
	// 
	// 	CinemaMgr::GetInst()->isPaused = !CinemaMgr::GetInst()->isPaused;  // 상태 변경
	// }

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
#pragma endregion
}

void Core::Render()
{
	TextOut(m_hDC, m_ptResolution.x / 2 - 62, m_ptResolution.y - 100, L"당신의 선택은?", 8);
	TextOut(m_hDC, m_ptResolution.x / 4, m_ptResolution.y - 35, L"A", 1);
	TextOut(m_hDC, m_ptResolution.x / 4 + m_ptResolution.x / 2, m_ptResolution.y - 35, L"B", 1);
	
	bool isLeft = ButtonMgr::GetInst()->isLeft;
	if (isLeft)
	{
		HBRUSH hBlueBrush = Core::GetInst()->GetBrush(BRUSH_TYPE::RED);
		HBRUSH originalBrush = (HBRUSH)SelectObject(m_hDC, hBlueBrush);

		if (originalBrush != NULL)                                             
		{
			Rectangle(m_hDC, 100, 775, 600, 850);
			SelectObject(m_hDC, Core::GetInst()->GetBrush(BRUSH_TYPE::WHITE));
		}      
		Rectangle(m_hDC, 800, 775, 1300, 850);
	}
	else
	{
		HBRUSH hBlueBrush = Core::GetInst()->GetBrush(BRUSH_TYPE::RED);
		HBRUSH originalBrush = (HBRUSH)SelectObject(m_hDC, hBlueBrush);

		if (originalBrush != NULL)
		{
			Rectangle(m_hDC, 800, 775, 1300, 850);
			SelectObject(m_hDC, Core::GetInst()->GetBrush(BRUSH_TYPE::WHITE));
		}
		Rectangle(m_hDC, 100, 775, 600, 850);
	}
}

void Core::CreateGDI()
{
	// HOLLOW
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::RED] = (HBRUSH)CreateSolidBrush(RGB(255, 102, 178));
	m_arrBrush[(UINT)BRUSH_TYPE::GREEN] = (HBRUSH)CreateSolidBrush(RGB(134, 229, 134));
	m_arrBrush[(UINT)BRUSH_TYPE::BLUE] = (HBRUSH)CreateSolidBrush(RGB(103, 153, 255));
	m_arrBrush[(UINT)BRUSH_TYPE::YELLOW] = (HBRUSH)CreateSolidBrush(RGB(255, 187, 0));
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	m_arrBrush[(UINT)BRUSH_TYPE::WHITE] = (HBRUSH)CreateSolidBrush(RGB(224, 224, 224));

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


// exMfcHowToGraphicsGdi.h: exMfcHowToGraphicsGdi 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CexMfcHowToGraphicsGdiApp:
// 이 클래스의 구현에 대해서는 exMfcHowToGraphicsGdi.cpp을(를) 참조하세요.
//

class CexMfcHowToGraphicsGdiApp : public CWinAppEx
{
private:
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;

public:
	CexMfcHowToGraphicsGdiApp() noexcept;


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CexMfcHowToGraphicsGdiApp theApp;


// UtilityTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUtilityToolApp: 
// �йش����ʵ�֣������ UtilityTool.cpp
//

class CUtilityToolApp : public CWinApp
{
public:
	CUtilityToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUtilityToolApp theApp;
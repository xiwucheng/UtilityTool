#include "stdafx.h"
#include "DragEdit.h"


CDragEdit::CDragEdit()
{
	m_nType = 0;
}


CDragEdit::~CDragEdit()
{
}
BEGIN_MESSAGE_MAP(CDragEdit, CEdit)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


void CDragEdit::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	TCHAR szPath[2048] = { 0 };
	int len = DragQueryFile(hDropInfo, 0, szPath, sizeof(szPath));
	if (len)
	{
		WIN32_FIND_DATA wfd;
		HANDLE hFind = FindFirstFile(szPath, &wfd);
		if (hFind != INVALID_HANDLE_VALUE && ~(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (m_nType == 0)
			{
				SetWindowText(szPath);
			}
			else
			{
				CFile fp;
				WCHAR wsKey[30] = { 0 };
				char szKey[30] = { 0 };
				if (fp.Open(szPath, CFile::modeRead | CFile::typeBinary))
				{
					if (fp.GetLength() == 49)
					{
						fp.Seek(20, SEEK_SET);
						fp.Read(szKey, 29);
						for (int i = 0; i < 29; i++)
						{
							wsKey[i] = szKey[i];
						}
						SetWindowText(wsKey);
					}
					fp.Close();
				}
			}
		}
		FindClose(hFind);
	}
	CEdit::OnDropFiles(hDropInfo);
}


void CDragEdit::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	DragAcceptFiles();
	typedef BOOL(WINAPI* ChangeWindowMessageFilterFn)(UINT, DWORD);
	HMODULE hUserMod = LoadLibrary(TEXT("user32.dll"));
	if (hUserMod)
	{
		ChangeWindowMessageFilterFn pfnChangeWindowMessageFilter = (ChangeWindowMessageFilterFn)GetProcAddress(hUserMod, "ChangeWindowMessageFilter");
		if (pfnChangeWindowMessageFilter)
		{
			pfnChangeWindowMessageFilter(WM_DROPFILES, 1); // 1-MSGFLT_ADD, 2-MSGFLT_REMOVE
			pfnChangeWindowMessageFilter(0x0049, 1); // 1-MSGFLT_ADD, 2-MSGFLT_REMOVE
		}

		FreeLibrary(hUserMod);
	}
	//SetTimer(1, 1000, 0);
	CEdit::PreSubclassWindow();
}

void CDragEdit::SetType(int nType)
{
	m_nType = nType;
}
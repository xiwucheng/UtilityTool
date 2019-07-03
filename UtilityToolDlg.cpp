
// UtilityToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UtilityTool.h"
#include "UtilityToolDlg.h"
#include "afxdialogex.h"
#include "SmbiosDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUtilityToolDlg 对话框



CUtilityToolDlg::CUtilityToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UTILITYTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUtilityToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUtilityToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_UPGRADE, &CUtilityToolDlg::OnBnClickedUpgrade)
END_MESSAGE_MAP()


// CUtilityToolDlg 消息处理程序

BOOL CUtilityToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	dmi.Init();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUtilityToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUtilityToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

wstring CUtilityToolDlg::ExecuteCmd(LPSTR lpCmd, BOOL bWantResult, LPDWORD lpExitCode, BOOL bShow)
{
	if (lpExitCode == NULL)
	{
		return wstring(L"");
	}

	BOOL retval;
	int result = -1;
	STARTUPINFOA si = { 0 };
	SECURITY_ATTRIBUTES sa = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	DWORD retcode = -1;
	HANDLE hReadPipe, hWritePipe;

	*lpExitCode = -1;
	sa.bInheritHandle = bWantResult;
	sa.nLength = sizeof SECURITY_ATTRIBUTES;
	sa.lpSecurityDescriptor = NULL;
	retval = CreatePipe(&hReadPipe, &hWritePipe, &sa, 0);
	if (!retval)
	{
		return wstring(L"");
	}
	si.cb = sizeof STARTUPINFO;
	si.wShowWindow = bShow ? SW_SHOW : SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdOutput = si.hStdError = hWritePipe;
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
	retval = CreateProcessA(NULL, lpCmd, &sa, &sa, bWantResult, 0, NULL, 0, &si, &pi);
	if (!retval)
	{
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		return wstring(L"");
	}

	DWORD dwLen, dwRead;
	WaitForSingleObject(pi.hThread, INFINITE);
	GetExitCodeProcess(pi.hProcess, lpExitCode);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	if (!bWantResult)
	{
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		return wstring(L"");
	}
	dwLen = GetFileSize(hReadPipe, NULL);
	char* buff = new char[dwLen + 1];
	memset(buff, 0, dwLen + 1);
	retval = ReadFile(hReadPipe, buff, dwLen, &dwRead, NULL);
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);

	wstring ws;
	int len = MultiByteToWideChar(CP_ACP, 0, buff, dwLen, NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, buff, dwLen, buffer, len);
	buffer[len] = '\0';             //添加字符串结尾  
									//删除缓冲区并返回值  
	ws.append(buffer);
	delete[] buff;
	delete[] buffer;
	return ws;
}

void CUtilityToolDlg::On_UEFI_Update()
{
	GUID CHT3 =  { 0x0ff0a55a, 0x0003, 0x0204,{ 0x06, 0x02, 0x10, 0x0e, 0x20, 0x00, 0x21, 0x00 } };
	GUID* pFw = 0;
	char buff[16] = { 0 };
	if (wcsstr(dmi.get_processor_version().c_str(), L"Z8300") ||
		wcsstr(dmi.get_processor_version().c_str(), L"Z8350") 
		)
	{
		pFw = &CHT3;
	}

	if (pFw == NULL)
	{
		MessageBox(TEXT("Platform not supported!"), TEXT("Error"), MB_ICONERROR);
		return;
	}
	SYSTEM_POWER_STATUS sps;
	memset(&sps, 0, sizeof(sps));
	GetSystemPowerStatus(&sps);
		if (sps.BatteryLifePercent < 15 && (sps.BatteryFlag&BATTERY_FLAG_CHARGING) != BATTERY_FLAG_CHARGING)
	{
		MessageBox(TEXT("Low battery（<15%）,Please plug in the adapter and try it again!"), TEXT("Error"), MB_ICONERROR);
		return;
	}
	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpdateThread, this, 0, NULL));
}

UINT CUtilityToolDlg::UpdateThread(LPVOID lp)
{
	CUtilityToolDlg* p = (CUtilityToolDlg*)lp;
	EnableMenuItem(::GetSystemMenu(p->m_hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED);
	p->GetDlgItem(IDC_UPGRADE)->EnableWindow(0);
	p->UpdateBios();
	p->GetDlgItem(IDC_UPGRADE)->EnableWindow(1);
	EnableMenuItem(::GetSystemMenu(p->m_hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
	return 0;
}

int CUtilityToolDlg::UpdateBios()
{
	CFile fp;
	DWORD uLen, dwUEFICode = 0xFFFFFFFF, dwExitCode = 0;
	CString wsPath;
	wchar_t szErrMsg[256] = TEXT("Device doesn't match with the firmware!");;
	char lpCmd[1024] = { 0 };
	BYTE *lpBuff;
	wstring ws = dmi.get_sku_number();

	//m_hUEFI.GetWindowText(wsPath);
	//58E324E020190327113851 ---> 54C358EC20190619112123 ----- BOE
	//48619F6020180907181327 ---> F61CD77420190621102822 ----- AUO
	if (wcscmp(L"58E324E020190327113851", ws.c_str()) == 0)//BOE-OLD
	{
		wsPath = TEXT("boe.fv");
	}
	else if (wcscmp(L"48619F6020180907181327", ws.c_str()) == 0)//AUO-OLD
	{
		wsPath = TEXT("auo.fv");
	}
	else if (wcscmp(L"54C358EC20190619112123", ws.c_str()) == 0)//BOE-NEW
	{
		MessageBox(TEXT("The bios firmware is up to date"), TEXT("Upgrade Info"), MB_ICONINFORMATION);
		return 1;
	}
	else if (wcscmp(L"F61CD77420190621102822", ws.c_str()) == 0)//AUO-NEW
	{
		MessageBox(TEXT("The bios firmware is up to date"), TEXT("Upgrade Info"), MB_ICONINFORMATION);
		return 1;
	}
	else
	{
		goto end;
	}
	if (!fp.Open(wsPath, CFile::modeRead | CFile::typeBinary))
	{
		_tcscpy_s(szErrMsg, TEXT("Firmware not found!"));
		goto end;
	}
	uLen = (DWORD)fp.GetLength();
	lpBuff = new BYTE[uLen];
	fp.Read(lpBuff, uLen);
	fp.Close();
	if (!fp.Open(TEXT("fw.bin"), CFile::modeCreate | CFile::modeReadWrite))
	{
		_tcscpy_s(szErrMsg, TEXT("Open firmware failed!"));
		delete lpBuff;
		goto end;
	}
	fp.Write(lpBuff, uLen);
	fp.Close();
	delete lpBuff;

	SetDlgItemText(IDC_STATUS, TEXT("Upgrading......"));
	Sleep(2000);

	if (!dmi.GetFileExist(L"fptw.exe"))
	{
		_tcscpy_s(szErrMsg, TEXT("fptw.exe not found!"));
		goto end;
	}

	ExecuteCmd("cmd.exe /c fptw.exe -f fw.bin -y", 0, &dwUEFICode, 1);

	DeleteFile(TEXT("fw.bin"));
	if (dwUEFICode == 0)
	{
		SetDlgItemText(IDC_STATUS, TEXT("Upgrade done!"));
		Sleep(1000);
	}
	else
	{
		SetDlgItemText(IDC_STATUS, TEXT("Upgrade failed!"));
		goto end;
	}

	if (wcsncmp(dmi.get_uuid().c_str(), L"00020003000400050006000700080009", 32) == 0)
	{
		SetDlgItemText(IDC_STATUS, TEXT("Updating UUID......"));
		Sleep(1000);
		ExecuteCmd("cmd.exe /c amidewin.exe /su auto", 0, &dwExitCode);
	}

	if (1)
	{
		strcpy_s(lpCmd, "cmd.exe /c amidewin.exe /ss \"");
		strcat_s(lpCmd, dmi.wstring2string(dmi.get_system_serial_number()).c_str());
		strcat_s(lpCmd, "\"");
		ExecuteCmd(lpCmd, 0, &dwExitCode);
	}
end:
	if (dwUEFICode == 0)
	{
		HANDLE hToken=0;
		TOKEN_PRIVILEGES tkp = { 0 };

		SetDlgItemText(IDC_STATUS, TEXT("System will reboot after 3 seconds......"));
		Sleep(3000);

		OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
		LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(TOKEN_PRIVILEGES),NULL,0);
		ExitWindowsEx(EWX_FORCE|EWX_REBOOT,0);
		CloseHandle(hToken);
	}
	else
	{
		MessageBox(szErrMsg, TEXT("Upgrade Info"), MB_ICONERROR);
	}

	return 0;
}



BOOL CUtilityToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
	{
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CUtilityToolDlg::OnBnClickedUpgrade()
{
	// TODO: 在此添加控件通知处理程序代码
	On_UEFI_Update();
}

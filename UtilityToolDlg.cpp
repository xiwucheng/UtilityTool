
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
	, m_nKey(1)
	, m_nSN(0)
	, m_nTool(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUtilityToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECKDPK, m_nKey);
	DDX_Check(pDX, IDC_CHECKSN, m_nSN);
	DDX_Radio(pDX, IDC_FPTW, m_nTool);
}

BEGIN_MESSAGE_MAP(CUtilityToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
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
	DWORD dwLen;
	CFile fp;
	BOOL bRet;
	HRSRC hSrc = FindResource(NULL, MAKEINTRESOURCE(IDR_CFG1), L"CFG");
	HGLOBAL hGl = LoadResource(NULL, hSrc);
	dwLen = SizeofResource(NULL, hSrc);
	LPBYTE lpBuf = (LPBYTE)LockResource(hGl);
	bRet = fp.Open(L"OA3ToolFile.cfg", CFile::modeCreate | CFile::modeReadWrite);
	fp.Write((LPBYTE)lpBuf, dwLen);
	fp.Close();
	m_hDPK.SubclassDlgItem(IDC_NEWDPK, this);
	m_hDPK.SetType(1);
	m_hUEFI.SubclassDlgItem(IDC_FWPATH, this);
	InitDMInformation();
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

void CUtilityToolDlg::InitDMInformation()
{
	dmi.Init();
	SetDlgItemText(IDC_VER, dmi.get_bios_version().c_str());
	SetDlgItemText(IDC_DATE, dmi.get_bios_release_date().c_str());
	SetDlgItemText(IDC_MANU, dmi.get_system_manufacturer().c_str());
	SetDlgItemText(IDC_SKU, dmi.get_sku_number().c_str());
	SetDlgItemText(IDC_BBMANU, dmi.get_baseboard_manufacturer().c_str());
	SetDlgItemText(IDC_TID, dmi.get_oem_table_id().c_str());
	SetDlgItemText(IDC_UUID, dmi.get_uuid().c_str());
	SetDlgItemText(IDC_PMODEL, dmi.get_system_product_name().c_str());
	SetDlgItemText(IDC_PSN, dmi.get_system_serial_number().c_str());
	SetDlgItemText(IDC_BSN, dmi.get_baseboard_serial_number().c_str());
	SetDlgItemText(IDC_FFACTOR, dmi.get_form_factor().c_str());

	SetDlgItemText(IDC_SYSVERVAL, dmi.get_system_version().c_str());
	SetDlgItemText(IDC_SYSFMLYVAL, dmi.get_family().c_str());
	SetDlgItemText(IDC_MBMDVAL, dmi.get_baseboard_product_name().c_str());
	SetDlgItemText(IDC_MBVERVAL, dmi.get_baseboard_version().c_str());

	m_nFormFactorType = dmi.get_form_factor_type();
	SetDlgItemText(IDC_OEMID, dmi.get_oem_id().c_str());
	wstring ver, var;
	dmi.GetOSVersion(var);
	IsWow64Process(GetCurrentProcess(), &m_bIs64);
	ver = TEXT("    处理器: ") + dmi.get_processor_version();
	SetDlgItemText(IDC_CPUINFO, ver.c_str());
	ver = TEXT("  操作系统: ") + var;
	SetDlgItemText(IDC_OSINFO, ver.c_str());
	dmi.GetMemoryInfo(var);
	ver = TEXT("已安装内存: ") + var;
	SetDlgItemText(IDC_MEMINFO, ver.c_str());
}

void CUtilityToolDlg::EnableButtons(BOOL bEnable)
{
	EnableMenuItem(::GetSystemMenu(m_hWnd, FALSE), SC_CLOSE, MF_BYCOMMAND | (bEnable ? MF_ENABLED : MF_DISABLED));
	GetDlgItem(IDC_NEWDPK)->EnableWindow(bEnable);
	GetDlgItem(IDC_FPTW)->EnableWindow(bEnable);
	GetDlgItem(IDC_AMITOOL)->EnableWindow(bEnable);
	GetDlgItem(IDC_SKUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_MANUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_MDUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_SYSVER)->EnableWindow(bEnable);
	GetDlgItem(IDC_PSNUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_FMLY)->EnableWindow(bEnable);
	GetDlgItem(IDC_UUIDUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BMANUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_BPMUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_MBVER)->EnableWindow(bEnable);
	GetDlgItem(IDC_BSNUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_FFUP)->EnableWindow(bEnable);
	GetDlgItem(IDC_SAVE)->EnableWindow(bEnable);
	GetDlgItem(IDC_INJECT)->EnableWindow(bEnable);
	GetDlgItem(IDC_QUERY)->EnableWindow(bEnable);
	GetDlgItem(IDC_BROWSE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECKSN)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECKDPK)->EnableWindow(bEnable);
	GetDlgItem(IDC_UEFIUP)->EnableWindow(bEnable);
}

void CUtilityToolDlg::UpdateStatus(CString szTips)
{
	SetDlgItemText(IDC_STATUS, szTips);
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

BOOL CUtilityToolDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (HIWORD(wParam) == BN_CLICKED)
	{
		switch (LOWORD(wParam))
		{
		case IDC_VERUP:
			break;
		case IDC_DATEUP:
			break;
		case IDC_MANUP:
			EnableButtons(0);
			Manufacture_Update();
			EnableButtons(1);
			break;
		case IDC_BMANUP:
			EnableButtons(0);
			Baseboard_Manufacture_Update();
			EnableButtons(1);
			break;
		case IDC_SKUP:
			EnableButtons(0);
			SKU_Update();
			EnableButtons(1);
			break;
		case IDC_TIDUP:
			break;
		case IDC_UUIDUP:
			EnableButtons(0);
			UUID_Update();
			EnableButtons(1);
			break;
		case IDC_MDUP:
			EnableButtons(0);
			Product_Model_Update();
			EnableButtons(1);
			break;
		case IDC_PSNUP:
			EnableButtons(0);
			Product_Serial_Number_Update();
			EnableButtons(1);
			break;
		case IDC_BSNUP:
			EnableButtons(0);
			Baseboard_Serial_Number_Update();
			EnableButtons(1);
			break;
		case IDC_FFUP:
			EnableButtons(0);
			Form_Factor_Update();
			EnableButtons(1);
			break;
		case IDC_OEMIDUP:
			break;
		case IDC_QUERY:
			EnableButtons(0);
			On_Query();
			EnableButtons(1);
			break;
		case IDC_SYSVER:
			EnableButtons(0);
			SysVer_Update();
			EnableButtons(1);
			break;
		case IDC_FMLY:
			EnableButtons(0);
			SysFamily_Update();
			EnableButtons(1);
			break;
		case IDC_BPMUP:
			EnableButtons(0);
			MBModel_Update();
			EnableButtons(1);
			break;
		case IDC_MBVER:
			EnableButtons(0);
			MBVer_Update();
			EnableButtons(1);
			break;

		case IDC_SAVE:
			EnableButtons(0);
			On_Save();
			EnableButtons(1);
			break;
		case IDC_INJECT:
			//EnableButtons(0);
			On_Inject();
			//EnableButtons(1);
			break;
		case IDC_BROWSE:
			EnableButtons(0);
			On_Browse();
			EnableButtons(1);
			break;
		case IDC_UEFIUP:
			//EnableButtons(0);
			On_UEFI_Update();
			//EnableButtons(1);
			break;
		}
	}
	return CDialogEx::OnCommand(wParam, lParam);
}

void CUtilityToolDlg::SKU_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(0);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /sk \"%s\""), dlg.m_strModify);
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新SKU信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新SKU信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::Manufacture_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(0);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /sm \"%s\""), dlg.m_strModify);
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新系统制造商信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新系统制造商信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::Baseboard_Manufacture_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(0);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /bm \"%s\""), dlg.m_strModify);
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新主板制造商信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新主板制造商信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::UUID_Update()
{
	CString szCmd,szUUID;
	DWORD dwExitCode;
	GetDlgItemText(IDC_UUID, szUUID);
	if (szUUID.Compare(L"00020003000400050006000700080009"))
	{
		UpdateStatus(TEXT("UUID已存在，无需更新！"));
		MessageBox(TEXT("UUID已存在，无需更新！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	szCmd = TEXT("cmd.exe /c amidewin.exe /su auto");
	string sCmd = CT2A(szCmd.GetString());
	ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
	if (dwExitCode)
	{
		UpdateStatus(TEXT("更新UUID信息失败！"));
	}
	else
	{
		UpdateStatus(TEXT("更新UUID信息成功！"));
		InitDMInformation();
	}
}

void CUtilityToolDlg::Product_Model_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(0);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /sp \"%s\""), dlg.m_strModify);
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新产品型号信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新产品型号信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::Product_Serial_Number_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(0);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /ss \"%s\""), dlg.m_strModify);
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新产品序列号信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新产品序列号信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::Baseboard_Serial_Number_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(0);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /bs \"%s\""), dlg.m_strModify);
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新主板序列号信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新主板序列号信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::Form_Factor_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(m_nFormFactorType);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /ct \"%02x\""), dlg.GetFormFactorType());
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新产品形态信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新产品形态信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::SysVer_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(0);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /sv \"%s\""), dlg.m_strModify);
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新系统版本信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新系统版本信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::SysFamily_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(0);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /sf \"%s\""), dlg.m_strModify);
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新产品家族信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新产品家族信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::MBModel_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(0);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /bp \"%s\""), dlg.m_strModify);
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新主板产品型号信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新主板产品型号信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::MBVer_Update()
{
	CString szCmd;
	DWORD dwExitCode;
	CSmbiosDlg dlg(0);
	if (!dmi.GetFileExist(L"amidewin.exe"))
	{
		MessageBox(TEXT("缺少amidewin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	if (IDOK == dlg.DoModal())
	{
		szCmd.Format(TEXT("cmd.exe /c amidewin.exe /bv \"%s\""), dlg.m_strModify);
		string sCmd = CT2A(szCmd.GetString());
		ExecuteCmd((LPSTR)sCmd.c_str(), 0, &dwExitCode);
		if (dwExitCode)
		{
			UpdateStatus(TEXT("更新主板版本信息失败！"));
		}
		else
		{
			UpdateStatus(TEXT("更新主板版本信息成功！"));
			InitDMInformation();
		}
	}
}

void CUtilityToolDlg::On_Browse()
{
	wchar_t wsPath[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, wsPath);
	CFileDialog dlg(TRUE, 0, 0, 6, TEXT("bios文件|*.bin||"), this);
	dlg.m_ofn.lpstrInitialDir = wsPath;
	if (dlg.DoModal() == IDOK)
	{
		m_hUEFI.SetWindowText(dlg.GetPathName());
	}
	SetCurrentDirectory(wsPath);
}

void CUtilityToolDlg::On_Query()
{
	DWORD dwExitCode = 0,nVer;
	CFile fp;
	wstring ws;
	if (m_bIs64)
	{
		if (!dmi.GetVersion(L"oa3tool_x64.exe", &nVer))
		{
			MessageBox(TEXT("缺少oa3tool_x64.exe 文件！"), TEXT("错误"), MB_ICONERROR);
			return;
		}
		if (nVer < 17763)
		{
			MessageBox(TEXT("oa3tool 版本过低，请使用17763（RS5）及以上版本的工具！"), TEXT("错误"), MB_ICONERROR);
			return;
		}
		ws = ExecuteCmd("oa3tool_x64.exe /validate", 1, &dwExitCode);
	}
	else
	{
		if (!dmi.GetVersion(L"oa3tool_x86.exe", &nVer))
		{
			MessageBox(TEXT("缺少oa3tool_x86.exe 文件！"), TEXT("错误"), MB_ICONERROR);
			return;
		}
		if (nVer < 17763)
		{
			MessageBox(TEXT("oa3tool 版本过低，请使用17763（RS5）及以上版本的工具！"), TEXT("错误"), MB_ICONERROR);
			return;
		}
		ws = ExecuteCmd("oa3tool_x86.exe /validate", 1, &dwExitCode);
	}
	if (dwExitCode == 0x80070490)
	{
		UpdateStatus(L"机器中没有KEY！");
		return;
	}
	else if (dwExitCode)
	{
		CString szErr;
		szErr.Format(TEXT("查询KEY失败，错误代码:0x%08X！"), dwExitCode);
		UpdateStatus(szErr);
		return;
	}

	int n, len = ws.size();
	wchar_t *szKey = new wchar_t[len + 1];
	memset(szKey, 0, len + 1);
	ws._Copy_s(szKey, len, len,0);
	wchar_t *lpSub = wcsstr(szKey, L"The ACPI MSDM table in hex");
	if (!lpSub)
	{
		delete szKey;
		UpdateStatus(L"查询KEY失败，BIOS里没有写KEY");
		return;
	}
	wchar_t *szContext,*szToken = wcstok_s(lpSub, L"\r\n", &szContext);
	wchar_t DPK[30] = { 0 };
	n = -1;
	do
	{
		szToken = wcstok_s(NULL, L"\r\n ", &szContext);
		n++;
		if (n < 20)
		{
			continue;
		}
		else if (n < 49)
		{
			DPK[n - 20] = (wchar_t)wcstoul(szToken, 0, 16);
		}
		else
		{
			break;
		}
	} while (szToken);

	if (n == 49)
	{
		SetDlgItemText(IDC_DPK, DPK);
		UpdateStatus(L"查询KEY成功！");
	}
	delete szKey;

	if (m_bIs64)
	{
		ExecuteCmd("oa3tool_x64.exe /report /configfile=oa3toolfile.cfg", 0, &dwExitCode);
	}
	else
	{
		ExecuteCmd("oa3tool_x86.exe /report /configfile=oa3toolfile.cfg", 0, &dwExitCode);
	}
	if (dwExitCode)
	{
		UpdateStatus(L"查询KEY失败，BIOS里没有写KEY");
		return;
	}
	if (!fp.Open(TEXT("oa3.xml"), CFile::modeRead | CFile::typeBinary))
	{
		return;
	}
	len = (int)fp.GetLength();
	char* fBuff = new char[len+1];
	char pkid[14] = { 0 };
	fp.Read(fBuff, len);
	fp.Close();
	char* dpk = strstr(fBuff, "<ProductKeyID>");
	if (dpk)
	{
		strncpy_s(pkid, dpk + 14, 13);
	}
	memset(DPK, 0, sizeof(DPK));
	for (int i = 0; i < 13; i++)//PKID
	{
		DPK[i] = pkid[i];
	}
	delete fBuff;
	SetDlgItemText(IDC_DPKID, DPK);
	DeleteFile(L"oa3.xml");
}

void CUtilityToolDlg::On_Save()
{
	CString szPath,szExt;

	if (GetDlgItem(IDC_DPK)->GetWindowTextLength() == 0)
	{
		MessageBox(TEXT("未能获取到OA3密钥！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	char szDPK[20] = { 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x1d,0x00,0x00,0x00 };
	char szText[30] = { 0 };
	GetDlgItemTextA(m_hWnd, IDC_DPK, szText, 30);
	CFile fp;
	if (fp.Open(TEXT("key.bin"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		fp.Write(szDPK, 20);
		fp.Write(szText, 29);
		fp.Close();
		MessageBox(TEXT("OA3密钥保存成功！"), TEXT("提示"), MB_ICONINFORMATION);
	}
}

void CUtilityToolDlg::On_Inject()
{
	string sKey;
	CString wsKey;
	wchar_t wsOrig[32], *wsToken, *wsContext, *p;
	if (m_hDPK.GetWindowTextLength() != 29)
	{
		MessageBox(L"Key 格式错误", L"格式错误", MB_ICONERROR);
		return;
	}
	m_hDPK.GetWindowText(wsKey);
	wcscpy_s(wsOrig, wsKey.GetString());
	wsToken = wcstok_s(wsOrig, L"-", &wsContext);
	int i = 5;
	while (i--)
	{
		p = wsToken;
		if (wcslen(p) != 5)
		{
			MessageBox(L"Key 格式错误", L"格式错误", MB_ICONERROR);
			return;
		}
		while (*p)
		{
			if (!isalnum(*p))
			{
				MessageBox(L"Key中含有非常法字符", L"格式错误", MB_ICONERROR);
				return;
			}
			p++;
		}
		wsToken = wcstok_s(NULL, L"-", &wsContext);
	}
	//HANDLE hPid = 
	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpdateOAThread, this, 0, NULL));
	//WaitForSingleObject(hPid, INFINITE);
}

void CUtilityToolDlg::On_UEFI_Update()
{
	GUID BYTCR = { 0x0ff0a55a, 0x0003, 0x0204,{ 0x06, 0x02, 0x10, 0x0b, 0x20, 0x00, 0x21, 0x00 } };
	GUID CHT3 =  { 0x0ff0a55a, 0x0003, 0x0204,{ 0x06, 0x02, 0x10, 0x0e, 0x20, 0x00, 0x21, 0x00 } };
	GUID APL =   { 0x0ff0a55a, 0x0003, 0x0004,{ 0x08, 0x02, 0x10, 0x13, 0x00, 0x00, 0x00, 0x00 } };
	GUID GLK =   { 0x0ff0a55a, 0x0003, 0x0004,{ 0x08, 0x02, 0x10, 0x17, 0x00, 0x00, 0x00, 0x00 } };
	GUID* pFw = 0;
	char buff[16] = { 0 };
	if (wcsstr(dmi.get_processor_version().c_str(), L"Z8300") ||
		wcsstr(dmi.get_processor_version().c_str(), L"Z8350") 
		)
	{
		pFw = &CHT3;
	}
	else if (wcsstr(dmi.get_processor_version().c_str(), L"Z3735"))
	{
		pFw = &BYTCR;
	}
	else if (wcsstr(dmi.get_processor_version().c_str(), L"N3350") || 
		wcsstr(dmi.get_processor_version().c_str(), L"N3450") || 
		wcsstr(dmi.get_processor_version().c_str(), L"N4200"))
	{
		pFw = &APL;
	}
	else if (wcsstr(dmi.get_processor_version().c_str(), L"N4000") || 
		wcsstr(dmi.get_processor_version().c_str(), L"N4100") || 
		wcsstr(dmi.get_processor_version().c_str(), L"N5000"))
	{
		pFw = &GLK;
	}

	if (pFw == NULL)
	{
		MessageBox(TEXT("不支持当前平台！"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	CFile fp;
	CString wsPath;
	m_hUEFI.GetWindowText(wsPath);
	if (!fp.Open(wsPath, CFile::modeRead | CFile::typeBinary))
	{
		MessageBox(TEXT("未发现BIOS文件"), TEXT("错误"), MB_ICONERROR);
		return;
	}
	ULONG len = (ULONG)fp.GetLength();
	if (len != 8388608)
	{
		MessageBox(TEXT("无效的BIOS文件"), TEXT("错误"), MB_ICONERROR);
		fp.Close();
		return;
	}
	fp.Seek(0x10, SEEK_SET);
	fp.Read(buff, 16);
	if (memcmp(buff, (char*)pFw, 16))
	{
		MessageBox(TEXT("BIOS与此平台不匹配"), TEXT("错误"), MB_ICONERROR);
		fp.Close();
		return;
	}
	SYSTEM_POWER_STATUS sps;
	memset(&sps, 0, sizeof(sps));
	GetSystemPowerStatus(&sps);
		if (sps.BatteryLifePercent < 15 && (sps.BatteryFlag&BATTERY_FLAG_CHARGING) != BATTERY_FLAG_CHARGING)
	{
		MessageBox(TEXT("电池电量低（<15%）,请插上适配器再做升级！"), TEXT("错误"), MB_ICONERROR);
		fp.Close();
		return;
	}
	fp.Close();
	UpdateData();
	//HANDLE hPid =
	CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpdateThread, this, 0, NULL));
	//MsgWaitForMultipleObjects(1, &hPid, TRUE, INFINITE, QS_ALLINPUT);
	//WaitForSingleObject(hPid, INFINITE);
}

UINT CUtilityToolDlg::UpdateThread(LPVOID lp)
{
	CUtilityToolDlg* p = (CUtilityToolDlg*)lp;
	p->EnableButtons(0);
	p->UpdateBios();
	p->EnableButtons(1);
	return 0;
}

UINT CUtilityToolDlg::UpdateOAThread(LPVOID lp)
{
	CUtilityToolDlg* p = (CUtilityToolDlg*)lp;
	p->EnableButtons(0);
	p->UpdateKey();
	p->EnableButtons(1);
	return 0;
}

int CUtilityToolDlg::UpdateKey()
{
	DWORD dwExitCode = 0xFFFFFFFF;
	CFile fp;
	string sKey;
	CString wsKey;
	char szDPK[20] = { 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x1d,0x00,0x00,0x00 };
	m_hDPK.GetWindowText(wsKey);
	sKey = dmi.wstring2string(wsKey.GetString());
	if (!dmi.GetFileExist(L"afuwin.exe"))
	{
		MessageBox(TEXT("缺少afuwin.exe 文件！"), TEXT("错误"), MB_ICONERROR);
		return 1;
	}
	if (fp.Open(TEXT("key.bin"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		fp.Write(szDPK, 20);
		fp.Write(sKey.c_str(), 29);
		fp.Close();
	}
	ExecuteCmd("cmd.exe /c afuwin.exe /oad", 0, &dwExitCode);
	ExecuteCmd("cmd.exe /c afuwin.exe /akey.bin", 0, &dwExitCode);
	if (dwExitCode == 0)
	{
		SetDlgItemText(IDC_STATUS, TEXT("刷KEY成功！"));
	}
	else
	{
		SetDlgItemText(IDC_STATUS, TEXT("刷KEY失败！"));
	}
	DeleteFile(TEXT("key.bin"));
	return 0;
}

int CUtilityToolDlg::UpdateBios()
{
	CFile fp;
	DWORD uLen, dwUEFICode = 0xFFFFFFFF, dwExitCode = 0,nVer;
	CString wsPath;
	wstring ws;
	char DPK[30] = { 0 };
	wchar_t szErrMsg[256] = TEXT("未知错误！");;
	char lpCmd[1024] = { 0 };
	int n, len;
	wchar_t  *szKey, *lpSub, *szContext, *szToken;
	BYTE *lpBuff;

	m_bExistKey = FALSE;
	if (m_bIs64)
	{
		if (!dmi.GetVersion(L"oa3tool_x64.exe", &nVer))
		{
			_tcscpy_s(szErrMsg, TEXT("缺少oa3tool_x64.exe 文件！"));
			goto end;
		}
		if (nVer < 17763)
		{
			_tcscpy_s(szErrMsg, TEXT("oa3tool 版本过低，请使用17763（RS5）及以上版本的工具！"));
			goto end;
		}
		ws = ExecuteCmd("cmd.exe /c oa3tool_x64.exe /validate", 1, &dwExitCode);
	}
	else
	{
		if (!dmi.GetVersion(L"oa3tool_x86.exe", &nVer))
		{
			_tcscpy_s(szErrMsg, TEXT("缺少oa3tool_x86.exe 文件！"));
			goto end;
		}
		if (nVer < 17763)
		{
			_tcscpy_s(szErrMsg, TEXT("oa3tool 版本过低，请使用17763（RS5）及以上版本的工具！"));
			goto end;
		}
		ws = ExecuteCmd("cmd.exe /c oa3tool_x86.exe /validate", 1, &dwExitCode);
	}
	if (dwExitCode == 0x80070490)
	{
		UpdateStatus(L"查询KEY失败，机器中没有KEY！");
		goto NoKey;
	}
	else if (dwExitCode)
	{
		CString szErr;
		szErr.Format(TEXT("查询KEY失败，错误代码:0x%08X！"), dwExitCode);
		UpdateStatus(szErr);
		goto end;
	}

	len = wcslen(ws.c_str());
	szKey = new wchar_t[len+1];
	memset(szKey, 0, sizeof(wchar_t)*(len+1));
	wcscpy_s(szKey,len+1,ws.c_str());
	lpSub = wcsstr(szKey, L"The ACPI MSDM table in hex");
	if (!lpSub)
	{
		delete szKey;
		UpdateStatus(L"BIOS里没有找到KEY");
		goto NoKey;
	}
	szToken = wcstok_s(lpSub, L"\r\n", &szContext);
	
	n = -1;
	do
	{
		szToken = wcstok_s(NULL, L"\r\n ", &szContext);
		n++;
		if (n < 20)
		{
			continue;
		}
		else if (n < 49)
		{
			DPK[n - 20] = (char)wcstoul(szToken, 0, 16);
		}
		else
		{
			break;
		}
	} while (szToken);

	delete[] szKey;
	if (n != 49)
	{
		UpdateStatus(L"BIOS里没有找到KEY！");
		goto NoKey;
	}

	UpdateStatus(L"查询KEY成功！");
	m_bExistKey = TRUE;
	char szDPK[20] = { 0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x1d,0x00,0x00,0x00 };
	if (fp.Open(TEXT("key.bin"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		fp.Write(szDPK, 20);
		fp.Write(DPK, 29);
		fp.Close();
	}
NoKey:
	m_hUEFI.GetWindowText(wsPath);
	if (!fp.Open(wsPath, CFile::modeRead | CFile::typeBinary))
	{
		_tcscpy_s(szErrMsg, TEXT("未发现BIOS文件，请确认其是否存在！"));
		goto end;
	}
	uLen = (DWORD)fp.GetLength();
	lpBuff = new BYTE[uLen];
	fp.Read(lpBuff, uLen);
	fp.Close();
	if (!fp.Open(TEXT("fw.bin"), CFile::modeCreate | CFile::modeReadWrite))
	{
		_tcscpy_s(szErrMsg, TEXT("打开BIOS文件失败！"));
		goto end;
	}
	fp.Write(lpBuff, uLen);
	fp.Close();

	SetDlgItemText(IDC_STATUS, TEXT("正在刷写BIOS......"));
	Sleep(2000);
	if (m_nTool == 0)
	{
		if (!dmi.GetFileExist(L"fptw.exe"))
		{
			_tcscpy_s(szErrMsg, TEXT("缺少fptw.exe 文件！"));
			goto end;
		}
		if (!dmi.GetFileExist(L"amidewin.exe"))
		{
			_tcscpy_s(szErrMsg, TEXT("缺少amidewin.exe 文件！"));
			goto end;
		}
		ExecuteCmd("cmd.exe /c fptw.exe -f fw.bin -y", 0, &dwUEFICode,1);
	}
	else if (m_nTool == 1)
	{
		/*
		+---------------------------------------------------------------------------+
		|                 AMI Firmware Update Utility v5.11.03.1778                 |
		|      Copyright (C)2018 American Megatrends Inc. All Rights Reserved.      |
		+---------------------------------------------------------------------------+
		| Usage: AFUWIN.EXE <ROM File Name> [Option 1] [Option 2]...                |
		|           or                                                              |
		|        AFUWIN.EXE <Input or Output File Name> <Command>                   |
		|           or                                                              |
		|        AFUWIN.EXE <Command>                                               |
		| ------------------------------------------------------------------------- |
		| Commands:                                                                 |
		|        /FV - Copy Secure FV to EFI.                                       |
		|         /O - Save current ROM image to file                               |
		|         /U - Display ROM File's ROMID                                     |
		|         /S - Refer to Options: /S                                         |
		|         /D - Verification test of given ROM File without flashing BIOS.   |
		|        /A: - Refer to Options: /A:                                        |
		|       /OAD - Refer to Options: /OAD                                       |
		| /CLNEVNLOG - Refer to Options: /CLNEVNLOG                                 |
		| Options:                                                                  |
		|      /CMD: - Send special command to BIOS. /CMD:{xxx}                     |
		|   /OEMCMD: - Send special value to BIOS. /OEMCMD:xxx                      |
		|       /DPC - Don't Check Aptio 4 and Aptio 5 platform.                    |
		|       /PW: - Input password for file.                                     |
		|     /MEUL: - Program ME Entire Firmware Block, which supports             |
		|              Production.BIN and PreProduction.BIN files.                  |
		|         /Q - Silent execution                                             |
		|         /X - Don't Check ROM ID                                           |
		|         /S - Display current system's ROMID                               |
		|       /JBC - Don't Check AC adapter and battery                           |
		|  /HOLEOUT: - Save specific ROM Hole according to RomHole GUID.            |
		|              NewRomHole1.BIN /HOLEOUT:GUID                                |
		|        /SP - Preserve Setup setting.                                      |
		|         /R - Preserve ALL SMBIOS structure during programming             |
		|        /Rn - Preserve SMBIOS type N during programming(n=0-255)           |
		|         /B - Program Boot Block                                           |
		|         /P - Program Main BIOS                                            |
		|         /N - Program NVRAM                                                |
		|         /K - Program all non-critical blocks.                             |
		|        /Kn - Program n'th non-critical block(n=0-15).                     |
		|      /RLC: - To set default option for Rom layout change.(E:Entire ROM, A:|
		|              Abort, F:Force)                                              |
		|    /CLRCFG - Program without preserving setup configuration               |
		|    /BCPALL - Save all question values before flash                        |
		|     /HOLE: - Update specific ROM Hole according to RomHole GUID.          |
		|              NewRomHole1.BIN /HOLE:GUID                                   |
		|         /L - Program all ROM Holes.                                       |
		|        /Ln - Program n'th ROM Hole only(n=0-15).                          |
		|      /ECUF - Update EC BIOS when newer version is detected.               |
		|         /E - Program Embedded Controller Block                            |
		|        /ME - Program ME Entire Firmware Block.                            |
		|      /MEUF - Program ME Ignition Firmware Block.                          |
		|        /A: - Oem Activation file                                          |
		|       /OAD - Delete Oem Activation key                                    |
		| /CLNEVNLOG - Clear Event Log.                                             |
		|   /CAPSULE - Override Secure Flash policy to Capsule                      |
		|  /RECOVERY - Override Secure Flash policy to Recovery                     |
		|        /EC - Program Embedded Controller Block. (Flash Type)              |
		|    /REBOOT - Reboot after programming.                                    |
		|  /SHUTDOWN - Shutdown after programming.                                  |
		+---------------------------------------------------------------------------+
		*/
		if (!dmi.GetFileExist(L"afuwin.exe"))
		{
			_tcscpy_s(szErrMsg, TEXT("缺少afuwin.exe 文件！"));
			goto end;
		}
		if (!dmi.GetFileExist(L"amidewin.exe"))
		{
			_tcscpy_s(szErrMsg, TEXT("缺少amidewin.exe 文件！"));
			goto end;
		}
		ExecuteCmd("cmd.exe /c afuwin.exe fw.bin /P /N /X", 0, &dwUEFICode,1);
	}
	DeleteFile(TEXT("fw.bin"));
	if (dwUEFICode == 0)
	{
		SetDlgItemText(IDC_STATUS, TEXT("升级完成！"));
		Sleep(1000);
	}
	else
	{
		SetDlgItemText(IDC_STATUS, TEXT("升级失败！"));
		goto end;
	}

	if (wcsncmp(dmi.get_uuid().c_str(), L"00020003000400050006000700080009", 32) == 0)
	{
		SetDlgItemText(IDC_STATUS, TEXT("正在更新系统UUID......"));
		Sleep(1000);
		ExecuteCmd("cmd.exe /c amidewin.exe /su auto", 0, &dwExitCode);
	}

	if (m_nSN == BST_CHECKED)
	{
		strcpy_s(lpCmd, "cmd.exe /c amidewin.exe /ss \"");
		strcat_s(lpCmd, dmi.wstring2string(dmi.get_system_serial_number()).c_str());
		strcat_s(lpCmd, "\"");
		ExecuteCmd(lpCmd, 0, &dwExitCode);
	}

	if (m_bExistKey && m_nSN == BST_CHECKED)
	{
		if (fp.Open(TEXT("key.bin"), CFile::modeRead))
		{
			fp.Close();
			SetDlgItemText(IDC_STATUS, TEXT("正在重新写入机器中的微软密钥......"));
			Sleep(2000);
			ExecuteCmd("cmd.exe /c afuwin.exe /oad", 0, &dwExitCode);
			ExecuteCmd("cmd.exe /c afuwin.exe /akey.bin", 0, &dwExitCode);
			if (dwExitCode == 0)
			{
				SetDlgItemText(IDC_STATUS, TEXT("刷KEY成功！"));
			}
			else
			{
				SetDlgItemText(IDC_STATUS, TEXT("刷KEY失败！"));
			}
			DeleteFile(TEXT("key.bin"));
		}
	}
end:
	if (dwUEFICode == 0)
	{
		HANDLE hToken=0;
		TOKEN_PRIVILEGES tkp = { 0 };

		SetDlgItemText(IDC_STATUS, TEXT("正在重启系统......"));
		Sleep(1000);
		//ExecuteCmd("cmd.exe /c fptw.exe -greset", 0, &dwExitCode);

		/*
		*/
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
		MessageBox(szErrMsg, TEXT("升级错误"), MB_ICONERROR);
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

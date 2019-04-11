
// UtilityToolDlg.h : 头文件
//

#pragma once
#include "DragEdit.h"
#include <string>
#include "dmi.h"
#define MAX_DLG	7
using namespace std;

// CUtilityToolDlg 对话框
class CUtilityToolDlg : public CDialogEx
{
// 构造
public:
	CUtilityToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UTILITYTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	BOOL m_bIs64;
	BYTE m_nFormFactorType;
	CDragEdit m_hDPK, m_hUEFI;
	BOOL m_bExistKey;
	CDmi dmi;
	static UINT UpdateThread(LPVOID lp);
	static UINT UpdateOAThread(LPVOID lp);
	int UpdateBios();
	int UpdateKey();
	void EnableButtons(BOOL bEnable);
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	wstring ExecuteCmd(LPSTR lpCmd, BOOL bWantResult, LPDWORD lpExitCode, BOOL bShow = FALSE);
	void InitDMInformation();
	void UpdateStatus(CString szTips);
	void SKU_Update();
	void Manufacture_Update();
	void Baseboard_Manufacture_Update();
	void UUID_Update();
	void Product_Model_Update();
	void Product_Serial_Number_Update();
	void Baseboard_Serial_Number_Update();
	void Form_Factor_Update();
	void SysVer_Update();
	void SysFamily_Update();
	void MBModel_Update();
	void MBVer_Update();
	void On_Browse();
	void On_Query();
	void On_Save();
	void On_Inject();
	void On_UEFI_Update();
	int m_nKey;
	int m_nSN;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_nTool;
};

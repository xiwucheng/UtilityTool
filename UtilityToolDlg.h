
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
	CDmi dmi;
	static UINT UpdateThread(LPVOID lp);
	int UpdateBios();
	int UpdateKey();
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	wstring ExecuteCmd(LPSTR lpCmd, BOOL bWantResult, LPDWORD lpExitCode, BOOL bShow = FALSE);
	void On_UEFI_Update();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedUpgrade();
};


// UtilityToolDlg.h : ͷ�ļ�
//

#pragma once
#include "DragEdit.h"
#include <string>
#include "dmi.h"
#define MAX_DLG	7
using namespace std;

// CUtilityToolDlg �Ի���
class CUtilityToolDlg : public CDialogEx
{
// ����
public:
	CUtilityToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UTILITYTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CDmi dmi;
	static UINT UpdateThread(LPVOID lp);
	int UpdateBios();
	int UpdateKey();
	// ���ɵ���Ϣӳ�亯��
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

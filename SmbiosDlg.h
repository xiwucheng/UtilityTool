#pragma once


// CSmbiosDlg 对话框

class CSmbiosDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSmbiosDlg)

public:
	CSmbiosDlg(int nType, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSmbiosDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMBDLG };
#endif

protected:
	int m_nType;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strModify;
	int GetFormFactorType();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedValup();
};

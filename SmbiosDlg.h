#pragma once


// CSmbiosDlg �Ի���

class CSmbiosDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSmbiosDlg)

public:
	CSmbiosDlg(int nType, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSmbiosDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMBDLG };
#endif

protected:
	int m_nType;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strModify;
	int GetFormFactorType();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedValup();
};

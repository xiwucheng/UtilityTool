// SmbiosDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UtilityTool.h"
#include "SmbiosDlg.h"
#include "afxdialogex.h"


// CSmbiosDlg 对话框

IMPLEMENT_DYNAMIC(CSmbiosDlg, CDialogEx)

CSmbiosDlg::CSmbiosDlg(int nType, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SMBDLG, pParent)
{
	m_nType = nType;
}

CSmbiosDlg::~CSmbiosDlg()
{
}

void CSmbiosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSmbiosDlg, CDialogEx)
	ON_BN_CLICKED(IDC_VALUP, &CSmbiosDlg::OnBnClickedValup)
END_MESSAGE_MAP()


// CSmbiosDlg 消息处理程序


BOOL CSmbiosDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_nType == 0)
	{
		GetDlgItem(IDC_NEWVAL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_FORMFACTOR)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_NEWVAL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_FORMFACTOR)->ShowWindow(SW_SHOW);
		((CComboBox*)GetDlgItem(IDC_FORMFACTOR))->SetCurSel(m_nType-1);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSmbiosDlg::OnBnClickedValup()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItemText(IDC_NEWVAL, m_strModify);
	m_nType = ((CComboBox*)GetDlgItem(IDC_FORMFACTOR))->GetCurSel() + 1;
	CDialogEx::OnOK();
}

int CSmbiosDlg::GetFormFactorType()
{
	return m_nType;
}
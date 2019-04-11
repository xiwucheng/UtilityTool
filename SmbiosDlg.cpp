// SmbiosDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UtilityTool.h"
#include "SmbiosDlg.h"
#include "afxdialogex.h"


// CSmbiosDlg �Ի���

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


// CSmbiosDlg ��Ϣ�������


BOOL CSmbiosDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CSmbiosDlg::OnBnClickedValup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItemText(IDC_NEWVAL, m_strModify);
	m_nType = ((CComboBox*)GetDlgItem(IDC_FORMFACTOR))->GetCurSel() + 1;
	CDialogEx::OnOK();
}

int CSmbiosDlg::GetFormFactorType()
{
	return m_nType;
}
#pragma once
#include "afxwin.h"
class CDragEdit :
	public CEdit
{
public:
	CDragEdit();
	virtual ~CDragEdit();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDropFiles(HDROP hDropInfo);
	virtual void PreSubclassWindow();
private:
	int m_nType;
public:
	void SetType(int nType);
};


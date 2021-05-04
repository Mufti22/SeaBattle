// SaveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SeaBattle.h"
#include "SaveDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSaveDialog dialog


CSaveDialog::CSaveDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSaveDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}


void CSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSaveDialog)
	DDX_Control(pDX, IDC_EDIT1, m_EditName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSaveDialog, CDialog)
	//{{AFX_MSG_MAP(CSaveDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSaveDialog message handlers

void CSaveDialog::OnOK() 
{
	char buf[100];
	int count;

	count = m_EditName.GetWindowText(buf, 100);
	if(count == 100) buf[99] = 0;
	::SendMessage(m_pParent->m_hWnd, WM_STATIC_TO_DLG, WM_ADD_RESULT, (LPARAM)buf);
	CDialog::OnOK();
}

void CSaveDialog::SetParent(CWnd *pParent)
{
	m_pParent = pParent;
}

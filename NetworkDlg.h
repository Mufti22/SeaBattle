#if !defined(AFX_NETWORKDLG_H__2FB94820_232A_11D8_9461_F007684A9063__INCLUDED_)
#define AFX_NETWORKDLG_H__2FB94820_232A_11D8_9461_F007684A9063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNetworkDlg dialog

class CNetworkDlg : public CDialog
{
// Construction
public:
	bool IsByHand;
	CNetworkDlg(CWnd* pParent = NULL);   // standard constructor
	CWnd *parent;
// Dialog Data
	//{{AFX_DATA(CNetworkDlg)
	enum { IDD = IDD_NETWORK };
	CButton	m_Cansel;
	CStatic	m_Static;
	CButton	m_Ok;
	CIPAddressCtrl	m_IPAddr;
	CListBox	m_Servers;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetworkDlg)
	afx_msg void OnConnect();
	afx_msg void OnCreateServ();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRefresh();
	afx_msg void OnByHand();
	virtual BOOL OnInitDialog();
	afx_msg void OnOkCon();
	afx_msg void OnCansel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKDLG_H__2FB94820_232A_11D8_9461_F007684A9063__INCLUDED_)

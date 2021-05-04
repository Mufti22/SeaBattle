#if !defined(AFX_SAVEDIALOG_H__752E511A_9A2E_4CFC_AC34_9F07B417D758__INCLUDED_)
#define AFX_SAVEDIALOG_H__752E511A_9A2E_4CFC_AC34_9F07B417D758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SaveDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSaveDialog dialog

class CSaveDialog : public CDialog
{
// Construction
public:
	void SetParent(CWnd *pParent);
	CSaveDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSaveDialog)
	enum { IDD = IDD_SAVE_RESULT };
	CEdit	m_EditName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSaveDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSaveDialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CWnd* m_pParent;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEDIALOG_H__752E511A_9A2E_4CFC_AC34_9F07B417D758__INCLUDED_)

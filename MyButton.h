#if !defined(AFX_MYBUTTON_H__DE572970_679E_4780_BCAE_646AA3DC1D78__INCLUDED_)
#define AFX_MYBUTTON_H__DE572970_679E_4780_BCAE_646AA3DC1D78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton window

class CMyButton : public CButton
{
private:
	int m_iState;
	CWnd *m_pParent;
// Construction
public:
	CMyButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init(CWnd *pParent);
	void SetState(int state);
	int GetState() const;
	virtual ~CMyButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBUTTON_H__DE572970_679E_4780_BCAE_646AA3DC1D78__INCLUDED_)

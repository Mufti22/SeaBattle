#if !defined(AFX_BUTWND_H__2D18ABA0_392E_11D8_9461_DDC3DC409760__INCLUDED_)
#define AFX_BUTWND_H__2D18ABA0_392E_11D8_9461_DDC3DC409760__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButWnd.h : header file
//
#define BT_NONE 0
#define BT_SHIPS 1
#define BT_NET 2
#define BT_SINGLE 3
#define BT_HELP 4
/////////////////////////////////////////////////////////////////////////////
// CButWnd window

class CButWnd : public CWnd
{
// Construction
public:
	CWnd *m_Parent;
	CPicture ActPic, Pic;
	CButWnd();
	int mode;
	POINT LastPoint;
	int IsFocus;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetMode(int md, CWnd* parent);
	virtual ~CButWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButWnd)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTWND_H__2D18ABA0_392E_11D8_9461_DDC3DC409760__INCLUDED_)

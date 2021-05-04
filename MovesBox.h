#if !defined(AFX_MOVESBOX_H__95A0B700_3BCA_11D8_9461_AFB8D4438360__INCLUDED_)
#define AFX_MOVESBOX_H__95A0B700_3BCA_11D8_9461_AFB8D4438360__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MovesBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMovesBox window

class CMovesBox : public CListCtrl
{
// Construction
public:
	CMovesBox();
	CDC m_FonDC;
	CBitmap *pOld, m_bmp;
	CWnd *m_Parent;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMovesBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init(CWnd *pParent);
	virtual ~CMovesBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMovesBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOVESBOX_H__95A0B700_3BCA_11D8_9461_AFB8D4438360__INCLUDED_)

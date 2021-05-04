// SeaBattleDlg.h : header file
//

#if !defined(AFX_SEABATTLEDLG_H__D4FFCEA6_0177_11D8_9461_DD37122E2C60__INCLUDED_)
#define AFX_SEABATTLEDLG_H__D4FFCEA6_0177_11D8_9461_DD37122E2C60__INCLUDED_

#include "NetworkDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Field.h"
#include "ButWnd.h"	// Added by ClassView
#include "MovesBox.h"	// Added by ClassView
#include "MyButton.h"
#include "SaveDialog.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CSeaBattleDlg dialog

//Button states
#define BS_RESULTS 10000
#define BS_HISTORY 10001
//processes
#define PLACE 1003
#define SINGLE_PLAY 1004
#define FUCK 1005
#define NET_GAME 1006

//operations
#define UNDEFINED 1
#define LBUTTONDOWN 2
#define FINDITEM 3

//to
#define TO_MINE 3002
#define TO_COMP 3003


class CSeaBattleDlg : public CDialog
{
// Construction
public:
	void SaveFile();
	void MoveOwnWnd(CWnd *wnd);
	bool ReadFile();
	void DrawHisResButton(bool IsDraw = true);
	void MoveButtons(bool IsTn);
	CBitmap bClose, bHide, bTnOn, bTnOut;
	CMovesBox m_MovesBox;
	void SetTurn();
	void SetBtDown(int but);
	CButWnd m_ShipWnd, m_NetWnd, m_SingleWnd, m_HelpWnd;
	bool OwnMove, IsTurn;
	void DrawMove();
	void RefreshList();
	void GetServName(char *name);
	void NetworkStart();
	void MoveNetworkWnd();
	CNetworkDlg NetworkWnd;
	void MoveHelp();
	int IsButton(POINT point);
	POINT MovePoint;
	int GenerateNumber(int a);
	CSeaBattleDlg(CWnd* pParent = NULL);	// standard constructor
	CField m_OwnField, m_ForeignField, m_LetterO, m_LetterF,
		m_NumberO, m_NumberF;
	int m_Process;
	bool m_CompFirstStep;
	HCURSOR m_Sight;
	CDC m_FonDC;
	CBitmap *pOld;
	int m_Operation;
	HCURSOR m_cHand, m_cDefault;
	CDialog Help;

// Dialog Data
	//{{AFX_DATA(CSeaBattleDlg)
	enum { IDD = IDD_SEABATTLE_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeaBattleDlg)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CListCtrl m_Results;
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CSeaBattleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg HRESULT ReceiveMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CSaveDialog m_SaveDialog;
	int m_iButtonState;
	CFile m_ResFile;
	int m_MinResult;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEABATTLEDLG_H__D4FFCEA6_0177_11D8_9461_DD37122E2C60__INCLUDED_)

#if !defined(AFX_FIELD_H__D4FFCEAE_0177_11D8_9461_DD37122E2C60__INCLUDED_)
#define AFX_FIELD_H__D4FFCEAE_0177_11D8_9461_DD37122E2C60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Field.h : header file
//

struct GENDATA
{
	short x, y;
	int IsKilled;
};

//objects
#define OWN_FIELD 1
#define FOREIGN_FIELD 2
#define LETTERS 3
#define NUMBERS 4

//directions
#define RIGHT   3000
#define BOTTOM    3001
#define LEFT  3002
#define TOP 3003
#define HOR 3004
#define VERT 3005
#define ONLYONE 3006

//operations
#define PLACEMENT 10000
#define NONE      10001
#define FIRE      10002
#define WAIT      10003

//messages
#define WM_PLACEMENT 11000
#define WM_NONE      11001
#define WM_SET_TIMER 11002
#define WM_IS_OK     11003
#define WM_COUNT_PLACEMENT 11004
#define WM_FIRE 11005
#define WM_CLEARFIELD 11006
#define WM_FINISH 11007
#define WM_IS_PLACEMENT 11008

//properties
#define EMPTY 1000
#define SHIP 1001
#define NEARSHIP 1002
#define SHIPWOUND 1003
#define SHIPKILLED 1004
#define CANNOT 1005
#define SHIPNOTKILLED 1006
#define NEARKILLEDSHIP 1007

//timer
#define ACT_SHIP 14000

//dir
#define RT 0
#define BT 1
/////////////////////////////////////////////////////////////////////////////
// CField window

//struct for send data



class CField : public CStatic
{
// Construction
public:
	CField();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CField)
	//}}AFX_VIRTUAL

// Implementation
private:
	int m_GoodSteps, m_Steps;
public:
	int GetReport();
	void SetParent(CWnd *wnd);
	void SetShipKilled(POINT pt);
	POINT NetPoint;
	bool IsNet;
	int NetStep(int x, int y);
	void RecvNetData(int x, int y);
//	RECT* Draw(CDC* memDC, LPRECT lpRect);
	void DrawObject(RECT rect, CDC *dc, HBITMAP hbm);
	void CompMove();
	GENDATA GenerateMove(int x=-1, int y=-1);
	bool ConvertPropertiesToMassive(POINT pos, int decks, int dir);
	void CountPlacement();
	int GenerateNumber(int a);
	void DrawShip(RECT rect, CDC *dc);
	RECT GetRect();
	int ReceiveMessage(int wParam, long lParam);
	void SetMode(int mode);
	virtual ~CField();
	CWnd* m_Parent;
	int KilledShips;
	int Alpha;
	int m_Operation;

	// Generated message map functions
protected:
	//{{AFX_MSG(CField)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int m_Draw;
	CBitmap bmp, *pOld;
	CDC WaterDC;
	HBITMAP m_Bmp[8], m_bFire, m_bMonoFire;
	int m_Mode;
	RECT m_Ship;
	int m_Decks, m_Direction;
	POINT m_Position;
	short m_Event[10][10];
	bool ConvertPropertiesToMassive();
	short m_Count;
	HCURSOR m_cFire;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELD_H__D4FFCEAE_0177_11D8_9461_DD37122E2C60__INCLUDED_)

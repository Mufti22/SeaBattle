// SeaBattle.h : main header file for the SEABATTLE application
//

#if !defined(AFX_SEABATTLE_H__D4FFCEA4_0177_11D8_9461_DD37122E2C60__INCLUDED_)
#define AFX_SEABATTLE_H__D4FFCEA4_0177_11D8_9461_DD37122E2C60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSeaBattleApp:
// See SeaBattle.cpp for the implementation of this class
//

class CSeaBattleApp : public CWinApp
{
public:
	CSeaBattleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSeaBattleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSeaBattleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEABATTLE_H__D4FFCEA4_0177_11D8_9461_DD37122E2C60__INCLUDED_)

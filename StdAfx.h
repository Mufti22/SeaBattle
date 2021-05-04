// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#if !defined(AFX_STDAFX_H__D4FFCEA8_0177_11D8_9461_DD37122E2C60__INCLUDED_)
#define AFX_STDAFX_H__D4FFCEA8_0177_11D8_9461_DD37122E2C60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <atlbase.h>
#include <afxdisp.h>
#include <afxpriv2.h>
#include "Picture.h"

//#include <afxsock.h>		// MFC socket extensions
//messages
#define WM_STATIC_TO_DLG WM_USER+1

//Own messages
#define WM_NEW_SHIP 13333
#define WM_WIN_COMP 13334
#define WM_COMP_MOVE 13335
#define WM_WIN_YOU 13336
#define WM_KILL 13337
#define WM_REFRESH_LIST 13338
#define WM_INV_STR 13339
#define WM_OWN_STEP 13340
#define WM_WIN_NET 13341
#define WM_SET_STEP 13342
#define WM_SEND_DATA 13343
#define WM_FILL_DC 13344
#define WM_LBT_DOWN 13345
#define WM_FILL_LIST_DC 13346
#define WM_SET_MOVE 13347
#define WM_NEW_GAME 13348
#define WM_LBUTTONDOWN_BUT 13349
#define WM_ADD_RESULT 13350

//Buttons
#define B_PLACEMENT 19000
#define B_SINGLE 19001
#define B_NETWORK 19002
#define B_HELP 19003
#define B_CLOSE 19004
#define B_HIDE 19005
#define B_NONE 19006
#define B_TURN 19007
#define B_RESHIS 19008

//Network messages
#define N_MESSAGE -120
#define N_SHIPKILLED 127
#define N_SHIPWOUND 126
#define N_EMPTY 125
#define N_WIN_YOU 124
#define N_NO_WIN_YOU 123

struct NET_DATA
{
	char *buf;
	int lenght;
};

struct DATA
{
	int dir, decks;
	POINT pos;
};
struct LISTDATA
{
	int x, y, state;
	char *str;
};
#include "Picture.h"
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D4FFCEA8_0177_11D8_9461_DD37122E2C60__INCLUDED_)

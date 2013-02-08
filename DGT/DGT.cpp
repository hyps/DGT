// DGT.cpp : This is win32 insanity wrapper for Application

#include "stdafx.h"
#include "DGT.h"

#define THIS_CLASSNAME "DGT_window_window_class"
#define ID_CONNECTDISCONNECT       2000
#define ID_EXIT        2001

static Application* gApp = NULL;

//===================================================================================
//ShowPopupMenu
//===================================================================================
BOOL ShowPopupMenu( HWND hWnd, POINT *curpos, int wDefaultItem ) 
{
	//ADD MENU ITEMS.------------------------------------------------------------------
	HMENU hPop = CreatePopupMenu();

	InsertMenu( hPop, 0, MF_BYPOSITION | MF_STRING, ID_CONNECTDISCONNECT, 
		gApp->connected() ? gApp->disconnectedTitle() : gApp->connectedTitle());
	InsertMenu( hPop, 1, MF_BYPOSITION | MF_SEPARATOR, 0, NULL);
	InsertMenu( hPop, 2, MF_BYPOSITION | MF_STRING, ID_EXIT , gApp->quitTitle() );

	//CAN DO WITHOUT STUFF.------------------------------------------------------------
	SetMenuDefaultItem( hPop, ID_CONNECTDISCONNECT, FALSE );
	SetFocus          ( hWnd );
	SendMessage       ( hWnd, WM_INITMENUPOPUP, (WPARAM)hPop, 0 );

	//SHOW POPUP MENU.-----------------------------------------------------------------
	{
		//GET CURSOR POSITION TO CREATE POPUP THERE.-------------------------------------
		POINT pt;
		if (!curpos) {
			GetCursorPos( &pt );
			curpos = &pt;
		}

		{
			//DISPLAT MENU AND WAIT FOR SELECTION.-----------------------------------------
			WORD cmd = TrackPopupMenu( hPop, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY, curpos->x, curpos->y, 0, hWnd, NULL );

			//SEND MESSAGE MAPPED TO SELECTED ITEM.----------------------------------------
			SendMessage( hWnd, WM_COMMAND, cmd, 0 );
		}
	}

	DestroyMenu(hPop);

	return 0;
}

//===================================================================================
//RemoveTrayIcon
//===================================================================================
void RemoveTrayIcon( HWND hWnd, UINT uID )
{
	NOTIFYICONDATA  nid;
	nid.hWnd = hWnd;
	nid.uID  = uID;

	Shell_NotifyIcon( NIM_DELETE, &nid );
}

//===================================================================================
//AddTrayIcon
//===================================================================================
void AddTrayIcon( HINSTANCE hInst, HWND hWnd, UINT uID, UINT uCallbackMsg, UINT uIcon )
{
	//CREATE SYSTEN TRAY ICON.---------------------------------------------------------------------
	NOTIFYICONDATA  nid;
	ZeroMemory(&nid, sizeof(nid));

	nid.hWnd             = hWnd;
	nid.uID              = uID;
	nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = uCallbackMsg;

	nid.hIcon = (HICON)LoadImage(hInst,
		MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	//SEND MESSAGE TO SYSTEM TRAY TO ADD ICON.--------------------------------------------
	Shell_NotifyIcon( NIM_ADD, &nid );
}

//===================================================================================
//WndProc
//===================================================================================

static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{ 
	switch (uMsg) {

		case WM_CREATE:
			AddTrayIcon( gApp->hInst(), hWnd, 1, WM_APP, 0 );
			return 0;

		case WM_CLOSE:
			RemoveTrayIcon (hWnd, 1);
			PostQuitMessage(0);                                 //Sends WM_QUIT message
			return DefWindowProc( hWnd, uMsg, wParam, lParam ); //Calls WM_DESTROY

		case WM_COMMAND:
			switch (LOWORD(wParam)) 
			{
				case ID_CONNECTDISCONNECT:
					if (gApp->connected())
					{
						gApp->disconnect();
					}
					else
					{
						gApp->connect();
					}
					return 0;

				case ID_EXIT:
					PostMessage( hWnd, WM_CLOSE, 0, 0 );
					return 0;
			}
			return 0;

		case WM_APP:
			switch (lParam) 
			{
				case WM_LBUTTONDBLCLK:
				case WM_LBUTTONUP:
				case WM_RBUTTONUP:
					SetForegroundWindow( hWnd );
					ShowPopupMenu(hWnd, NULL, -1 );
					PostMessage( hWnd, WM_APP + 1, 0, 0 );
					return 0;
			}
			return 0;

	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );

}

//===================================================================================
//WinMain
//===================================================================================
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE prev, LPSTR cmdline, int show )
{
	Application theApp(hInst);

	gApp = &theApp;

	{
		//CHECK IF PREVIOUS ISTANCE IS RUNNING.-----------------------------------------------------
		HWND hPrev = NULL;
		if ( hPrev = FindWindow( THIS_CLASSNAME, gApp->uniqueWindowTitle()) ) 
		{
			return 0;
		}
	}

	{
		//REGISTER WINDOW.--------------------------------------------------------------------------
		WNDCLASSEX wclx; 
		memset(&wclx, 0, sizeof(wclx));

		wclx.cbSize         = sizeof( wclx );
		wclx.style          = 0;
		wclx.lpfnWndProc    = &WndProc;
		wclx.cbClsExtra     = 0;
		wclx.cbWndExtra     = 0;
		wclx.hInstance      = hInst;
		//wclx.hIcon        = LoadIcon( hInstance, MAKEINTRESOURCE( IDI_TRAYICON ) );
		//wclx.hIconSm      = LoadSmallIcon( hInstance, IDI_TRAYICON );
		wclx.hCursor        = LoadCursor( NULL, IDC_ARROW );
		wclx.hbrBackground  = (HBRUSH)( COLOR_BTNFACE + 1 );   

		wclx.lpszMenuName   = NULL;
		wclx.lpszClassName  = THIS_CLASSNAME;

		RegisterClassEx( &wclx );
	}

	{
		//CREATE WINDOW.----------------------------------------------------------------------------
		HWND hWnd = CreateWindow( THIS_CLASSNAME, gApp->uniqueWindowTitle(), WS_OVERLAPPEDWINDOW, 100, 100, 250, 150, NULL, NULL, hInst, NULL );
		if ( !hWnd ) {
			return 1;
		}
	}

	{
		//MONITOR MESSAGE QUEUE.--------------------------------------------------------------------
		MSG msg;
		while (GetMessage ( &msg, NULL, 0, 0 ) ) {
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		}

		//DESTROY WINDOW.---------------------------------------------------------------------------
		UnregisterClass( THIS_CLASSNAME, hInst );

		return msg.wParam;
	}

}
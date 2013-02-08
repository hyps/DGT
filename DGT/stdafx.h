// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <windows.h>
#include <winioctl.h>
#include <memory>
#include <boost/system/system_error.hpp>

#ifdef UNICODE
#define stringcopy wcscpy
#else
#define stringcopy strcpy
#endif

#define ID_TRAY_APP_ICON                5000
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM  3000
#define WM_TRAYICON ( WM_USER + 1 )

UINT WM_TASKBARCREATED = 0 ;

#define NO_CRYPT


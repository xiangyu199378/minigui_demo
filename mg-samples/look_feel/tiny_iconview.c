///////////////////////////////////////////////////////////////////////////////
//
//                          IMPORTANT NOTICE
//
// The following open source license statement does not apply to any
// entity in the Exception List published by FMSoft.
//
// For more information, please visit:
//
// https://www.fmsoft.cn/exception-list
//
//////////////////////////////////////////////////////////////////////////////
/*
** $Id: tiny_iconview.c 406 2008-02-01 03:12:21Z zhounuohua $
**
** tiny_iconview.c: Usage of ICONVIEW control.
**
** Copyright (C) 2007 ~ 2017 FMSoft (http://www.fmsoft.cn).
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _MGCTRL_ICONVIEW

#define IDC_ICONVIEW    100

extern int tiny_listbox1 (HWND hwnd);
extern HWND hwnd_my;

static BITMAP myicons [12];

static const char* iconfiles[12] =
{
    "../src/res/acroread.png",
    "../src/res/icons.png",
    "../src/res/looknfeel.png",
    "../src/res/package_games.png",
    "../src/res/tux.png",
    "../src/res/xemacs.png",
    "../src/res/gimp.png",
    "../src/res/kpilot.png",
    "../src/res/multimedia.png",
    "../src/res/realplayer.png",
    "../src/res/usb.png",
    "../src/res/xmms.png"
};

static const char *iconlabels[12] =
{
    "reader",
    "icons",
    "star",
    "games",
    "tux",
    "xemacs",
    "gimp",
    "kpilot",
    "music",
    "realone",
    "usb",
    "xmms"
};

static WNDPROC old_callback;
static LRESULT notif_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case MSG_COMMAND:
            switch (wParam) {
                case IDOK:
                    printf("get IDOK message\n");
                    PostMessage (GetParent(hwnd), MSG_CLOSE, 0, 0);
                    tiny_listbox1 (hwnd_my);
                    return 0;
                case IDCANCEL:
                    PostMessage (GetParent(hwnd), MSG_CLOSE, 0, 0);
                    ShowWindow (hwnd_my, SW_SHOWNORMAL);
                    return 0;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return old_callback(hwnd, message, wParam, lParam);
}

static LRESULT BookProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HWND hIconView = GetDlgItem (hwnd, IDC_ICONVIEW);
    switch (message)
    {
        case MSG_INITDIALOG:
        {
            IVITEMINFO ivii;
            int i = 0, j = 0;
            SendMessage (hIconView, IVM_SETITEMSIZE, 75, 65);
            old_callback = SetWindowCallbackProc (hIconView, notif_proc);

            for (i=0; i<TABLESIZE(myicons); i++) {
                LoadBitmap (HDC_SCREEN, &myicons[i], iconfiles[i]);
            }

            for (j = 0; j < 3; j ++) {
                for (i = 0; i < TABLESIZE(myicons); i++) {
                    memset (&ivii, 0, sizeof(IVITEMINFO));
                    ivii.bmp = &myicons[i];
                    ivii.nItem = 12 * j + i;
                    ivii.label = iconlabels[i];
                    ivii.addData = (DWORD)iconlabels[i];
                    SendMessage (hIconView, IVM_ADDITEM, 0, (LPARAM)&ivii);
                }
            }
            break;
        }
        case MSG_KEYUP:
        {
            if (wParam == SCANCODE_ESCAPE)
            {
                PostMessage(hwnd,MSG_CLOSE,0,0);
            }
        }
        break;

        case MSG_CLOSE:
        {
            EndDialog (hwnd, 0);
            ShowWindow (hwnd_my, SW_SHOWNORMAL);
            //return 0;
        }

    } /* end switch */

    return DefaultDialogProc (hwnd, message, wParam, lParam);
}

static CTRLDATA CtrlBook[] =
{
    {
        CTRL_ICONVIEW,
        WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
         0, 0, 240, 320,
        IDC_ICONVIEW,
        "",
        0,
        WS_EX_TRANSPARENT |
        WS_EX_LFRDR_CUSTOM1
//        0        
            | WS_EX_LFRDR_CUSTOM2 
            | WS_EX_LFRDR_CUSTOM4
//        | WS_EX_NOCLOSEBOX
//          | WS_EX_LFRDR_CUSTOM3
          ,
        "tiny",
        0
    }
};

static DLGTEMPLATE DlgIcon =
{
    WS_VISIBLE,
    WS_EX_NONE,
    0, 0, 240, 320,
#ifdef _LANG_ZHCN
    "????????????",
#else
    "My Friends",
#endif
    0, 0,
    TABLESIZE(CtrlBook), CtrlBook,
    0
};

int tiny_iconview (HWND hwnd)
{
    //DialogBoxIndirectParam (&DlgPassword, hWnd, DialogBoxProc3, 0L);
    DialogBoxIndirectParamEx (&DlgIcon, hwnd, BookProc, 0L,
           "tiny", 0, 0, 0 );
    return 0;
}

#else /* _MGCTRL_ICONVIEW */

int tiny_iconview (HWND hwnd)
{
    return MessageBox (hwnd, "WARNING", "Please enable ICONVIEW control support in MiniGUI.", MB_OK);
}

#endif /* _MGCTRL_ICONVIEW */

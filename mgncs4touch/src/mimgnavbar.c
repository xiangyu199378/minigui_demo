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
 *   This file is part of mGNCS4Touch, a component for MiniGUI.
 *
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Or,
 *
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 *
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 *
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/blog/minigui-licensing-policy/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <mgncs/mgncs.h>

#include "mtouchcomm.h"
#include "mtouchrdr.h"
#include "mimgnavbar.h"
#include "pieces/mshapepushbuttonpiece.h"
#include "pieces/mshapeboxpiece.h"
#include "pieces/msimagepiece.h"


#define INB_TEXT_LEN        128


static BOOL _onLeftButtonClick(mImgNavBar* self, mHotPiece* sender,
        int event_id, DWORD param)
{
    if (event_id ==  NCSN_ABP_CLICKED) {
        ncsNotifyParentEx((mWidget*)self, NCSN_INB_IMGCLICK,
                (DWORD)NCSD_INB_LEFT);
    }

    return FALSE;
}


static BOOL _onRightButtonClick(mImgNavBar* self, mHotPiece* sender,
        int event_id, DWORD param)
{
    if (event_id ==  NCSN_ABP_CLICKED) {
        ncsNotifyParentEx((mWidget*)self, NCSN_INB_IMGCLICK,
                (DWORD)NCSD_INB_RIGHT);
    }

    return FALSE;
}


static mHotPiece* create_left_piece(mImgNavBar* self)
{
    mSImagePiece* imagePiece = NULL;
    int event_ids[] = { NCSN_ABP_CLICKED, 0 };

    imagePiece = NEWPIECE(mSImagePiece);
    if (NULL != imagePiece) {
        _M(imagePiece, setProperty, NCSP_IMAGEPIECE_DRAWMODE, NCS_DM_SCALED);
        ncsAddEventListeners((mObject*)imagePiece, (mObject*)self,
                (NCS_CB_ONPIECEEVENT)_onLeftButtonClick, event_ids);
    }

    return (mHotPiece*)imagePiece;
}


static mHotPiece* create_right_piece(mImgNavBar* self)
{
    mSImagePiece* imagePiece = NULL;
    int event_ids[]= { NCSN_ABP_CLICKED, 0 };

    imagePiece = NEWPIECE(mSImagePiece);
    if (NULL != imagePiece) {
        _M(imagePiece, setProperty, NCSP_IMAGEPIECE_DRAWMODE, NCS_DM_SCALED);
        ncsAddEventListeners((mObject*)imagePiece, (mObject*)self,
                (NCS_CB_ONPIECEEVENT)_onRightButtonClick, event_ids);
    }

    return (mHotPiece*)imagePiece;
}


static void mImgNavBar_construct(mImgNavBar* self, DWORD param)
{
    Class(mWidget).construct((mWidget*)self, param);

    self->margin_w = 3;
    self->img_h    = 0;
}


static BOOL mImgNavBar_onCreate(mImgNavBar* self, DWORD add)
{
    RECT rc;
    GetClientRect(self->hwnd, &rc);
    self->leftbt_w = self->rightbt_w = ((RECTH(rc)) - (self->margin_w << 1)) << 1;

    _M(self, setProperty, NCSP_WIDGET_RDR, (DWORD)NCS4TOUCH_RENDERER);

    return TRUE;
}


static mObject* mImgNavBar_createBody(mImgNavBar* self)
{
    mHBoxLayoutPiece* hboxpiece = NEWPIECEEX(mHBoxLayoutPiece, INB_MAX_PIECE_ID);

    _c(hboxpiece)->setCell(hboxpiece, INB_LEFT_PIECE_ID, create_left_piece(self));
    _c(hboxpiece)->setCell(hboxpiece, INB_RIGHT_PIECE_ID, create_right_piece(self));

    _c(hboxpiece)->setCellInfo(hboxpiece, INB_LEFT_PIECE_ID,
            self->leftbt_w , NCS_LAYOUTPIECE_ST_FIXED, FALSE);
    _c(hboxpiece)->setCellInfo(hboxpiece, INB_RIGHT_PIECE_ID,
            self->rightbt_w, NCS_LAYOUTPIECE_ST_FIXED, FALSE);

    _c(hboxpiece)->setProperty(hboxpiece, NCSP_BOXLAYOUTPIECE_MARGIN, self->margin_w);

    return (mObject*)hboxpiece;
}


static BOOL get_text_rect(RECT* src, RECT* dst, int margin_w, int lbutton_w, int rbutton_w)
{
     if (src == NULL || dst == NULL)
        return FALSE;

    memcpy(dst, src, sizeof(RECT));

    if (lbutton_w > rbutton_w) {
        dst->left  += margin_w + lbutton_w + 2;
        dst->right -= margin_w + lbutton_w - 2;
    } else if (lbutton_w <= rbutton_w) {
        dst->left  += margin_w + rbutton_w + 2;
        dst->right -= margin_w + rbutton_w - 2;
    }

    return TRUE;
}


static void resetBodyRect(mImgNavBar* self, RECT* rcClient)
{
    if (self->body) {
        RECT rc = {0, 0, RECTWP(rcClient), RECTHP(rcClient)};

        if (self->img_h > 0 && self->img_h + (self->margin_w << 1) < RECTH(rc)) {
            rc.top += (RECTH(rc) - self->img_h - (self->margin_w << 1)) >> 1;
            rc.bottom = rc.top + self->img_h + (self->margin_w << 1);
        }
        _c((mHotPiece*)(self->body))->setRect((mHotPiece*)self->body, &rc);
    }
}


static int mImgNavBar_onSizeChanged(mImgNavBar* self, RECT* rtClient)
{
    resetBodyRect(self, rtClient);

    return 0;
}


static void mImgNavBar_onPaint(mImgNavBar* self, HDC hdc, const PCLIPRGN pinv_clip)
{
    RECT text_rc, rc;
    char text[INB_TEXT_LEN] = { 0 };
    int oldMode = 0;
    gal_pixel oldTxtColor = 0;

    GetClientRect(self->hwnd, &rc);
    GetWindowText(self->hwnd, text, INB_TEXT_LEN);
    get_text_rect(&rc, &text_rc, self->margin_w, self->leftbt_w, self->rightbt_w );

    SelectFont(hdc, self->caption_font);
    oldTxtColor = SetTextColor(hdc,
            ncsColor2Pixel(hdc, ncsGetElement(self, NCS4TOUCH_FGC_INB_TEXT)));
    oldMode = SetBkMode(hdc, BM_TRANSPARENT);
    DrawText(hdc, text, -1, &text_rc, DT_CENTER|DT_VCENTER | DT_SINGLELINE);
    SetBkMode(hdc, oldMode);
    SetTextColor(hdc, oldTxtColor);

    Class(mWidget).onPaint((mWidget*)self, hdc, pinv_clip);
}


static BOOL mImgNavBar_setProperty(mImgNavBar* self, int id, DWORD value)
{
    mHBoxLayoutPiece* hboxpiece = NULL;
    mSImagePiece* imagePiece = NULL;

    if (id >= NCSP_INB_MAX)
        return FALSE;

    switch (id) {
        case NCSP_WIDGET_RDR:
            if (strcmp((char*)value, NCS4TOUCH_RENDERER) == 0)
                break;
            else
                return FALSE;

        case NCSP_INB_MARGIN:
            if (self->margin_w != value) {
                self->margin_w = value;
                hboxpiece = (mHBoxLayoutPiece*)self->body;
                _c(hboxpiece)->setProperty(hboxpiece, NCSP_BOXLAYOUTPIECE_MARGIN, value);
                InvalidateRect(self->hwnd, NULL, TRUE);
            }
            return TRUE;

        case NCSP_INB_FONT:
            if (self->caption_font != (PLOGFONT)value) {
                self->caption_font = (PLOGFONT)value;
                InvalidateRect(self->hwnd, NULL, TRUE);
                return TRUE;
            }
            return FALSE;

        case NCSP_INB_LEFT_W:
            if (self->leftbt_w != value) {
                self->leftbt_w = value;
                hboxpiece = (mHBoxLayoutPiece*)self->body;
                _c(hboxpiece)->setCellInfo(hboxpiece, INB_LEFT_PIECE_ID,
                        self->leftbt_w , NCS_LAYOUTPIECE_ST_FIXED, TRUE);
            }
            return TRUE;

        case NCSP_INB_RIGHT_W:
            if (self->rightbt_w != value) {
                self->rightbt_w = value;
                hboxpiece = (mHBoxLayoutPiece*)self->body;
                _c(hboxpiece)->setCellInfo(hboxpiece, INB_RIGHT_PIECE_ID,
                        self->rightbt_w , NCS_LAYOUTPIECE_ST_FIXED, TRUE);
            }
            return TRUE;

        case NCSP_INB_LBITMAP:
            if (NULL != (imagePiece = (mSImagePiece*)GetLeftPiece(self))) {
                _M(imagePiece, setProperty, NCSP_IMAGEPIECE_IMAGE, value);
                InvalidateRect(self->hwnd, NULL, TRUE);
            }
            return TRUE;

        case NCSP_INB_RBITMAP:
            if (NULL != (imagePiece = (mSImagePiece*)GetRightPiece(self))) {
                _M(imagePiece, setProperty, NCSP_IMAGEPIECE_IMAGE, value);
                InvalidateRect(self->hwnd, NULL, TRUE);
            }
            return TRUE;

        case NCSP_INB_IMG_H: {
            RECT rc;
            GetWindowRect(self->hwnd, &rc);

            if (value < 0 || value > RECTH(rc) - (self->margin_w << 1))
                return FALSE;

            self->img_h = value;
            resetBodyRect(self, &rc);
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;
        }
        case NCSP_INB_DRAWMODE:
            if (NULL != (imagePiece = (mSImagePiece*)GetLeftPiece(self))) {
                _M(imagePiece, setProperty, NCSP_IMAGEPIECE_DRAWMODE, value);
            }
            if (NULL != (imagePiece = (mSImagePiece*)GetRightPiece(self))) {
                _M(imagePiece, setProperty, NCSP_IMAGEPIECE_DRAWMODE, value);
            }
            InvalidateRect(self->hwnd, NULL, TRUE);
            return TRUE;

        default:
            break;
    }

    return Class(mWidget).setProperty((mWidget*)self, id, value);
}


static DWORD mImgNavBar_getProperty(mImgNavBar* self, int id)
{
    if (id >= NCSP_INB_MAX)
        return 0;

    switch (id) {
        case NCSP_INB_MARGIN:
            return self->margin_w;

        case NCSP_INB_FONT:
            return (DWORD)self->caption_font;

        case NCSP_INB_LEFT_W:
            return self->leftbt_w;

        case NCSP_INB_RIGHT_W:
            return self->rightbt_w;

        case NCSP_INB_IMG_H:
            return self->img_h;

        case NCSP_INB_DRAWMODE: {
            mSImagePiece* imagePiece = NULL;

            if (NULL != (imagePiece = (mSImagePiece*)GetLeftPiece(self))) {
                return _M(imagePiece, getProperty, NCSP_IMAGEPIECE_DRAWMODE);
            } else if (NULL != (imagePiece = (mSImagePiece*)GetRightPiece(self))) {
                return _M(imagePiece, getProperty, NCSP_IMAGEPIECE_DRAWMODE);
            }
            return 0;
        }
        default:
            break;
    }

    return Class(mWidget).getProperty((mWidget*)self, id);
}


static BOOL mImgNavBar_onEraseBkgnd(mImgNavBar* self, HDC hdc, const RECT* pinv)
{
    RECT rc;
    int line = 0, gh = 0;
    int r0 = 0, g0 = 0, b0 = 0;
    int r1 = 0, g1 = 0, b1 = 0;
    int r  = 0, g  = 0, b  = 0;
    DWORD endColor;
    DWORD beginColor;

    if (self->bkimg.img.pbmp != NULL) {
        return Class(mWidget).onEraseBkgnd((mWidget*)self, hdc, pinv);
    }

    endColor   = Pixel2DWORD(hdc, GetWindowBkColor(self->hwnd));
    beginColor = ncsCommRDRCalc3dboxColor(endColor, NCSR_COLOR_LIGHTEST);

    GetClientRect(self->hwnd, &rc);

    r0 = GetRValue(beginColor);
    g0 = GetGValue(beginColor);
    b0 = GetBValue(beginColor);
    r1 = GetRValue(endColor);
    g1 = GetGValue(endColor);
    b1 = GetBValue(endColor);

    gh = RECTHP(&rc) / 2;

    for (line = 0; line < gh; line++) {
        r = r0 + line * (r1 - r0) / gh;
        g = g0 + line * (g1 - g0) / gh;
        b = b0 + line * (b1 - b0) / gh;
        SetPenColor(hdc, RGB2Pixel(hdc, r, g, b));
        LineEx(hdc, rc.left, rc.top + line, rc.right, rc.top + line);
    }

    SetBrushColor(hdc, RGB2Pixel(hdc, r1, g1, b1));
    FillBox(hdc, rc.left, rc.top + gh, RECTWP(&rc), RECTHP(&rc)-gh);

    SetPenColor(hdc, RGB2Pixel(hdc, r0, g0, b0));
    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

    return TRUE;
}


BEGIN_CMPT_CLASS(mImgNavBar, mWidget)
    CLASS_METHOD_MAP(mImgNavBar, construct)
    CLASS_METHOD_MAP(mImgNavBar, createBody)
    CLASS_METHOD_MAP(mImgNavBar, onCreate)
    CLASS_METHOD_MAP(mImgNavBar, onSizeChanged)
    CLASS_METHOD_MAP(mImgNavBar, onPaint)
    CLASS_METHOD_MAP(mImgNavBar, setProperty)
    CLASS_METHOD_MAP(mImgNavBar, getProperty)
    CLASS_METHOD_MAP(mImgNavBar, onEraseBkgnd)
END_CMPT_CLASS


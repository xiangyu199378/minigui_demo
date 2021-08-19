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
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>
#include <mgplus/mgplus.h>
#include <mgeff/mgeff.h>

#include "mgncs4touch.h"


#define ITEMPIECE_ADDREF(piece)     \
{                                   \
    do {                            \
        if (NULL != piece) {        \
            ADDREFPIECE(piece);     \
        }                           \
    } while(0);                     \
}

#define ITEMPIECE_UNREF(piece)      \
{                                   \
    do {                            \
        if (NULL != piece) {        \
            UNREFPIECE(piece);      \
        }                           \
    } while(0);                     \
}


mNavigationItem* ncsCreateNavigationItem (mHotPiece* content, const char* title, unsigned int style)
{
    /* create NavigationItem */
    mNavigationItem* navigation_item = (mNavigationItem*)NEW(mNavigationItem);
    if ( NULL == navigation_item )
        return NULL;

    ADDREF(navigation_item);
    /* configure the content */
    //navigation_item->content = content;
    _c(navigation_item)->setProperty(navigation_item, NCSP_NAVIGATIONITEM_CONTENT, (DWORD)content);
    navigation_item->title = (char*)title;
    navigation_item->style = style;

    return navigation_item;
}


void ncsDestroyNavigationItem (mNavigationItem* item)
{
    if ( NULL == item )
        return;
    UNREF(item);
}


void ncsAdjustNavigationItemRectWithTitle (mHotPiece* piece, const char* title, PLOGFONT font)
{
    int w;
    HDC hdc;
    RECT rect, piece_rc;
    SIZE size;
    PLOGFONT old_font;

    /* according to title len calc width */
    size.cx = 9999;
    hdc = HDC_SCREEN;
    if ( NULL != font ) {
        old_font = SelectFont(hdc, font);
        GetTextExtentPoint(hdc, title, strlen(title), 1000, NULL, NULL, NULL, &size);
        size.cx += NAVIGATIONBAR_DEFAULT_SHARPWIDTH*2;
        SelectFont(hdc, old_font);
    }

    _c(piece)->getRect(piece, &piece_rc);
    w = NAVIGATIONBAR_W/3;
    w = size.cx > NAVIGATIONBAR_W ? w : size.cx;

    SetRect(&rect, 0, 0, w, RECTH(piece_rc));
    _c(piece)->setRect(piece, &rect);
}


static void mNavigationItem_construct (mNavigationItem* self, DWORD param)
{
    Class(mObject).construct((mObject*)self, param);

    self->default_button_font = CreateLogFontEx ("ttf", "helvetica", "UTF-8",
        FONT_WEIGHT_REGULAR,
        FONT_SLANT_ROMAN,
        FONT_FLIP_NONE,
        FONT_OTHER_NONE,
        FONT_DECORATE_NONE, FONT_RENDER_SUBPIXEL,
        NAVIGATIONBAR_DEFAULT_BUTTON_FONT_SIZE, 0);

    self->style = NAVIGATION_STYLE_NORMAL;
    self->title = NULL;

    self->background = NULL;
    self->content = NULL;
    self->bar_left_button = NULL;
    self->bar_title_button = NULL;
    self->bar_right_button = NULL;
}


static void mNavigationItem_destroy (mNavigationItem* self, DWORD param)
{
    ITEMPIECE_UNREF(self->background);
    ITEMPIECE_UNREF(self->content);
    ITEMPIECE_UNREF(self->bar_left_button);
    ITEMPIECE_UNREF(self->bar_title_button);
    ITEMPIECE_UNREF(self->bar_right_button);

    DestroyLogFont(self->default_button_font);

    Class(mObject).destroy((mObject*)self);
}


static BOOL mNavigationItem_setProperty (mNavigationItem* self, int id, DWORD value)
{
    if ( NULL == self )
        return FALSE;

    switch (id) {
        case NCSP_NAVIGATIONITEM_CONTENT:
            if (self->content != (mHotPiece*)value) {
                ITEMPIECE_UNREF(self->content);
                self->content = (mHotPiece*)value;
                ITEMPIECE_ADDREF(self->content);
            }
            break;

        case NCSP_NAVIGATIONITEM_BACKGROUND:
            if (self->background != (mShapeTransRoundPiece*)value) {
                ITEMPIECE_UNREF(self->background);
                self->background = (mShapeTransRoundPiece*)value;
                ITEMPIECE_ADDREF(self->background);
            }
            break;

        case NCSP_NAVIGATIONITEM_BAR_BKG:
            if ( NULL == self->background )
                return FALSE;
            return _c(self->background)->setProperty(self->background, NCSP_TRANROUND_BKCOLOR, value);

        case NCSP_NAVIGATIONITEM_TITLE:
            self->title = (char*)value;
            break;

        case NCSP_NAVIGATIONITEM_STYLE:
            self->style = *(unsigned int*)value;
            break;

        case NCSP_NAVIGATIONITEM_LEFT_BUTTON:
            if (self->bar_left_button != (mButtonPanelPiece*)value) {
                ITEMPIECE_UNREF(self->bar_left_button);
                self->bar_left_button = (mButtonPanelPiece*)value;
                ITEMPIECE_ADDREF(self->bar_left_button);
            }
            break;

        case NCSP_NAVIGATIONITEM_TITLE_BUTTON:
            if (self->bar_title_button != (mTextPiece*)value) {
                ITEMPIECE_UNREF(self->bar_title_button);
                self->bar_title_button = (mTextPiece*)value;
                ITEMPIECE_ADDREF(self->bar_title_button);
            }
            break;

        case NCSP_NAVIGATIONITEM_RIGHT_BUTTON:
            if (self->bar_right_button != (mButtonPanelPiece*)value) {
                ITEMPIECE_UNREF(self->bar_right_button);
                self->bar_right_button = (mButtonPanelPiece*)value;
                ITEMPIECE_ADDREF(self->bar_right_button);
            }
            break;

        case NCSP_NAVIGATIONITEM_DEFAULT_BUTTON_FONT:
            return TRUE;

        default:
            return FALSE;
    }

    return TRUE;
}


static DWORD mNavigationItem_getProperty (mNavigationItem* self, int id)
{
    if ( NULL == self )
        return (DWORD)NULL;

    switch (id) {
        case NCSP_NAVIGATIONITEM_CONTENT:
            return (DWORD)self->content;

        case NCSP_NAVIGATIONITEM_BACKGROUND:
            return (DWORD)self->background;

        case NCSP_NAVIGATIONITEM_BAR_BKG:
            if ( NULL == self->background )
                return (DWORD)0;
            return _c(self->background)->getProperty(self->background, NCSP_TRANROUND_BKCOLOR);

        case NCSP_NAVIGATIONITEM_TITLE:
            return (DWORD)self->title;

        case NCSP_NAVIGATIONITEM_STYLE:
            return (DWORD)self->style;

        case NCSP_NAVIGATIONITEM_LEFT_BUTTON:
            return (DWORD)self->bar_left_button;

        case NCSP_NAVIGATIONITEM_TITLE_BUTTON:
            return (DWORD)self->bar_title_button;

        case NCSP_NAVIGATIONITEM_RIGHT_BUTTON:
            return (DWORD)self->bar_right_button;

        case NCSP_NAVIGATIONITEM_DEFAULT_BUTTON_FONT:
            return (DWORD)self->default_button_font;

        default:
            break;
    }

    return (DWORD)NULL;
}


static m3DButtonPiece* mNavigationItem_createDefaultButton (mNavigationItem* self,
        const char* title, DWORD add_data)
{
    RECT rect;
    mTextPiece* text_piece;
    m3DButtonPiece* button_piece;
    mShapeTransRoundPiece* bk_piece;

    ARGB normal_colors[] = {
        NAVIGATIONBAR_NORMALBUTTON_COLOR1,
        NAVIGATIONBAR_NORMALBUTTON_COLOR2};

    ARGB pushed_colors[] = {
        NAVIGATIONBAR_PUSHBUTTON_COLOR1,
        NAVIGATIONBAR_PUSHBUTTON_COLOR2,
        NAVIGATIONBAR_PUSHBUTTON_COLOR3};

    ARGB border_colors[] = {
        NAVIGATIONBAR_BORDER_COLOR1,
        NAVIGATIONBAR_BORDER_COLOR2};

    float normal_pos[] = {
        NAVIGATIONBAR_NORMAL_POS1,
        NAVIGATIONBAR_NORMAL_POS2};

    float border_pos[] = {
        NAVIGATIONBAR_BORDER_POS1,
        NAVIGATIONBAR_BORDER_POS2};

    /* create button_piece */
    button_piece  = (m3DButtonPiece*)NEWPIECEEX(m3DButtonPiece, add_data);

    /* set property */
    SetRect(&rect, 0, 0, NAVIGATIONBAR_W/4, NAVIGATIONBAR_DEFAULT_BUTTON_H);
    _c(button_piece)->setRect(button_piece, &rect);
    _c(button_piece)->setGradientBackgroundColor(button_piece,
            normal_colors,
            normal_pos,
            TABLESIZE(normal_pos),
            pushed_colors, NULL,
            TABLESIZE(pushed_colors));
    _c(button_piece)->setGradientBorderColor(button_piece,
            border_colors,
            border_pos,
            TABLESIZE(border_pos));
    _c(button_piece)->setStates(button_piece);

    text_piece = button_piece->txtPiece;
    _c(text_piece)->setProperty(text_piece, NCSP_TEXTPIECE_LOGFONT, (DWORD)self->default_button_font);
    _c(text_piece)->setProperty(text_piece, NCSP_LABELPIECE_LABEL, (DWORD)title);
    _c(text_piece)->setProperty(text_piece, NCSP_TEXTPIECE_TEXTCOLOR,(DWORD)NAVIGATIONBAR_DEFAULT_FONT_COLOR);
    _c(text_piece)->setProperty(text_piece, NCSP_TEXTPIECE_TEXTSHADOWCOLOR,(DWORD)NAVIGATIONBAR_DEFAULT_FONT_SHADOW_COLOR);

    bk_piece = _c(button_piece)->getBkgndPiece(button_piece);
    _c(bk_piece)->setProperty(bk_piece, NCSP_TRANROUND_SHARPWIDTH, NAVIGATIONBAR_DEFAULT_SHARPWIDTH);
    _c(bk_piece)->setProperty(bk_piece, NCSP_TRANROUND_RADIUS, NAVIGATIONBAR_DEFAULT_ROUND_RADIUS);
    _c(bk_piece)->setProperty(bk_piece, NCSP_TRANROUND_BORDERSIZE, NAVIGATIONBAR_BORDER_SIZE);

    return button_piece;
}


BEGIN_MINI_CLASS(mNavigationItem, mObject)
    CLASS_METHOD_MAP(mNavigationItem, construct)
    CLASS_METHOD_MAP(mNavigationItem, destroy)
    CLASS_METHOD_MAP(mNavigationItem, setProperty)
    CLASS_METHOD_MAP(mNavigationItem, getProperty)
    CLASS_METHOD_MAP(mNavigationItem, createDefaultButton)
END_MINI_CLASS


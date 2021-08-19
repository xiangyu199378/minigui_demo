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

#include <mgplus/mgplus.h>
#include <mgeff/mgeff.h>
#include <mgncs/mgncs.h>

#include "mgncs4touch.h"

#define ANINATIOM_DURATION 200 /*200ms*/
#define TEXT_BUFFER_LENGTH 15
#define TEXT_ZOOM_NO 3

static void cb_display_string (MGEFF_ANIMATION handle, void* target, intptr_t id, void* value)
{
    mAnimationEditPiece *self= (mAnimationEditPiece *)target;
    HWND hwnd = (HWND)id;
    RECT rc;

    _c(self)->getRect(self,&rc);
    self->text_offset = *(int *)value;
    UpdateWindow (hwnd, TRUE);
}

static int calculate_string_width (HWND hwnd, mAnimationEditPiece *self, const char *str)
{
    /* calculate move distance */
    SIZE size;
    HDC hdc = GetDC (hwnd);
    if (self->selected)
        SelectFont(hdc, self->selected);
    GetTextExtent (hdc, str, strlen(str), &size);
    ReleaseDC (hdc);
    return size.cx;
}

static void select_font (HWND hwnd, mAnimationEditPiece *self, const char *str)
{
    int i;
    RECT rc;
    int width_win;
    HDC hdc;
    LOGFONT logfont;

    _c(self)->getRect(self,&rc);
    width_win = RECTW (rc);

    if (self->fonts == NULL || self->fonts[0] == NULL)
        return;

    logfont = *(self->fonts[0]);
    if (self->fonts[1] == NULL&&self->fonts[2] == NULL)
    {
        for (i=1;i<TEXT_ZOOM_NO;i++)/*font[0] come from user*/
        {
            self->fonts[i] = CreateLogFontEx (logfont.type, logfont.family,logfont.charset,
                FONT_WEIGHT_REGULAR,
                FONT_SLANT_ROMAN,
                FONT_FLIP_NONE,
                FONT_OTHER_NONE,
                FONT_DECORATE_NONE, FONT_RENDER_SUBPIXEL,
                logfont.size*1/(i+1), 0);
        }
    }

    hdc = GetDC (hwnd);
    for (i=0; i<TEXT_ZOOM_NO; ++i) {
        // VW[2019-05-16]: Do not call SetWindowFont in NCS
        // SetWindowFont (hwnd, self->fonts[i]);
        // width = calculate_string_width (hwnd, self->fonts[i], self, str);

        SIZE size;
        SelectFont(hdc, self->fonts[i]);
        GetTextExtent(hdc, str, strlen(str), &size);
        if (size.cx <= width_win) {
            self->selected = self->fonts[i];
            break;
        }
    }

    ReleaseDC (hdc);
    return;
}

static void animation_modify_string (HWND hwnd, mAnimationEditPiece *self,
        const char *key_str, BOOL append)
{
    MGEFF_ANIMATION animation_handle = NULL;
    int move_distance;
    RECT rc;

    int width_win;
    int width_text;
    int endval;
    /* set string font size */
    char *tmpstr = (char *)calloc (1, self->content_length);
    memset (tmpstr, 0, self->content_length);
    if (append == TRUE) {
        sprintf (tmpstr, "%s%s", self->text, key_str);
    } else {
        sprintf (tmpstr, "%s", self->text);
        tmpstr[strlen (tmpstr) - strlen (key_str)] = 0;
    }

    select_font (hwnd, self, tmpstr);
    move_distance = calculate_string_width (hwnd, self, key_str);

    /* create animation */
    animation_handle = mGEffAnimationCreate(self, cb_display_string, (intptr_t)hwnd, MGEFF_INT);

    /* start val */
    _c(self)->getRect(self,&rc);
    width_win = RECTW (rc);
    width_text = calculate_string_width (hwnd, self, self->text);
    self->text_offset = width_win - width_text;
    mGEffAnimationSetStartValue (animation_handle, &self->text_offset);

    /* end val */
    endval = self->text_offset + ((append == FALSE)?move_distance:(0-move_distance));
    mGEffAnimationSetEndValue (animation_handle, &endval);

    /* misc */
    mGEffAnimationSetProperty (animation_handle, MGEFF_PROP_DURATION, ANINATIOM_DURATION);
    mGEffAnimationSetCurve (animation_handle, InQuad);

    if (append == TRUE) {
        strcat (self->text, key_str);
        mGEffAnimationSyncRun (animation_handle);
        mGEffAnimationDelete (animation_handle);
    } else {
        int string_length = strlen(self->text);
        int key_str_len = strlen (key_str);
        mGEffAnimationSyncRun (animation_handle);
        mGEffAnimationDelete (animation_handle);
        memset (&self->text[string_length - key_str_len], 0,key_str_len);
    }
   free (tmpstr);
}

static void mAnimationEditPiece_construct(mAnimationEditPiece *self, DWORD param)
{
    int i;

    Class(mStaticPiece).construct((mStaticPiece*)self, param);

    self->content_length = TEXT_BUFFER_LENGTH+1;
    self->text = (char *)calloc (1, self->content_length);
    if (self->text == NULL) {
        printf("calloc text mem failure!!\n");
        return;
    }
    memset (self->text, 0, self->content_length);

    self->text_offset = 0;
    self->text_color = 0xffffffff;
    self->text_shadow_color = 0xffffffff;

    self->fonts = (PLOGFONT *)calloc (TEXT_ZOOM_NO, sizeof(PLOGFONT));

    if (self->fonts == NULL) {
        printf("calloc font mem failure!!\n");
        return;
    }

    for (i=0;i<TEXT_ZOOM_NO;i++) {
        self->fonts[i] = NULL;
    }

    self->selected = NULL;
}

static void mAnimationEditPiece_destroy (mAnimationEditPiece *self, DWORD param)
{
    int i;
    free (self->text);
    self->text = NULL;
    self->content_length = 0;

    for (i=1; i<TEXT_ZOOM_NO; ++i) {/*font[0] come from user,will be released by user*/
        DestroyLogFont (self->fonts[i]);
    }

    free (self->fonts);
    self->fonts = NULL;
}

static void mAnimationEditPiece_setContent(HWND hwnd,
        mAnimationEditPiece *self, const char* str,text_align align)
{
    RECT rc;
    int string_width;

    /* check content for new string */
    if (strlen (str) >= self->content_length) {
        char *p = realloc (self->text, strlen(str)+1);
        if (!p) {
            printf ("realloc error!\n");
            return;
        }
        self->text = p;
        self->content_length = strlen(str)+1;
    }

    memset (self->text, 0, self->content_length);
    strcpy (self->text, str);

    /* calculate new string's offest */
    _c(self)->getRect(self,&rc);
    select_font (hwnd, self, self->text);
    string_width = calculate_string_width (hwnd, self, self->text);
    self->text_offset =(RECTW (rc) - string_width)>>1;
    if (align == TEXT_ALIGN_RIGHT)
    {
        self->text_offset = RECTW (rc) - string_width;
    }
    if (align == TEXT_ALIGN_LEFT)
    {
        self->text_offset = 0;
    }
}

static int mAnimationEditPiece_getContent(mAnimationEditPiece *self, char *strbuff, int buf_len)
{
    if (buf_len <= strlen (self->text)) {
        return -1;
    }
    strcpy (strbuff, self->text);
    return 0;
}

static void mAnimationEditPiece_append(HWND hwnd,mAnimationEditPiece *self, const char* str)
{
    /* check content for append */
    if ((strlen (str) + strlen (self->text)) >= self->content_length) {
        char *p =  realloc (self->text, self->content_length*2);
        if (!p) {
            printf ("realloc error!\n");
            return;
        }

        self->text = p;
        self->content_length = self->content_length * 2;
    }

    animation_modify_string (hwnd,self, str, TRUE);
}

static void mAnimationEditPiece_tailDelete(HWND hwnd,mAnimationEditPiece *self, int num)
{
    assert (num>=0);

    /* had deleted all */
    if (self->text == NULL) {
        return;
    }

    /* delete num > strlen */
    if (num > strlen (self->text)) {
        num = strlen (self->text);
    }

    animation_modify_string (hwnd,self, &self->text[strlen(self->text) - num], FALSE);
}

static void mAnimationEditPiece_paint (mAnimationEditPiece *self, HDC hdc,
        mWidget *owner, DWORD add_data)
{
    SIZE size;
    int h;
    int y;
    DWORD color;
    PLOGFONT of = NULL;

    SetBkMode(hdc,BM_TRANSPARENT);

    if (self->selected)
        of = SelectFont(hdc, self->selected);

    /* get font y  coordinate */
    GetTextExtent (hdc, "A", 1, &size);
    h = GetGDCapability (hdc, GDCAP_MAXY) + 1;
    y = (h - size.cy)/2;

    color = (DWORD)self->text_shadow_color;
    SetTextColor (hdc, RGBA2Pixel(hdc, color>>16&0xff,color>>8&0xff, color&0xff, color>>24&0xff));
    TextOut (hdc, self->text_offset+1, y+1, self->text);

    color = (DWORD)self->text_color;
    SetTextColor (hdc, RGBA2Pixel(hdc, color>>16&0xff, color>>8&0xff, color&0xff, color>>24&0xff));
    TextOut (hdc, self->text_offset, y, self->text);

    if (of)
        SelectFont(hdc, of);
}

static BOOL mAnimationEditPiece_setProperty(mAnimationEditPiece *self, int id, DWORD value)
{
    switch (id) {
        case NCSP_ANIMATIONEDITPIECE_FONT:
            if (self->fonts != NULL) {
                self->fonts[0] = (PLOGFONT)value;
            }
            break;
        case NCSP_ANIMATIONEDITPIECE_TEXTCOLOR:
            self->text_color = (ARGB)value;
            break;
        case NCSP_ANIMATIONEDITPIECE_TEXTSHADOWCOLOR:
            self->text_shadow_color = (ARGB)value;
            break;
        default:
            return Class(mStaticPiece).setProperty((mStaticPiece*)self, id, value);
    }
    return TRUE;
}

static DWORD mAnimationEditPiece_getProperty(mAnimationEditPiece *self, int id)
{
    switch(id)
    {
        case NCSP_ANIMATIONEDITPIECE_FONT:
            if (self->fonts != NULL)
            {
                return (DWORD)self->fonts[0];
            }
            else
            {
                return (DWORD)NULL;
            }
        case NCSP_ANIMATIONEDITPIECE_TEXTCOLOR:
            return (DWORD)self->text_color;
        case NCSP_ANIMATIONEDITPIECE_TEXTSHADOWCOLOR:
            return (DWORD)self->text_shadow_color;
    }
    return Class(mStaticPiece).getProperty((mStaticPiece*)self, id);
}


BEGIN_MINI_CLASS(mAnimationEditPiece, mStaticPiece)
    CLASS_METHOD_MAP(mAnimationEditPiece, construct)
    CLASS_METHOD_MAP(mAnimationEditPiece, destroy)
    CLASS_METHOD_MAP(mAnimationEditPiece, paint)
    CLASS_METHOD_MAP(mAnimationEditPiece, setProperty)
    CLASS_METHOD_MAP(mAnimationEditPiece, getProperty)
    CLASS_METHOD_MAP(mAnimationEditPiece, setContent)
    CLASS_METHOD_MAP(mAnimationEditPiece, getContent)
    CLASS_METHOD_MAP(mAnimationEditPiece, append)
    CLASS_METHOD_MAP(mAnimationEditPiece, tailDelete)
END_MINI_CLASS


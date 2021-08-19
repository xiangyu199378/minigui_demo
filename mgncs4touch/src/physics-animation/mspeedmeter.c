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
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgncs/mgncs.h>

#include "mspeedmeter.h"

typedef struct {
    int m_x;
    int m_y;
    unsigned int m_t;
} record_t;

typedef struct{
    int m_precision_ms;
    int m_size;
    int m_index;
    int m_count;
    record_t *m_records;
} mSpeedMeter_t;

static int s_mod(int a, int b) {
    assert(b > 0);
    while (a < 0) {
        a += b;
    }
    while (a > b) {
        a -= b;
    }
    assert(a >= 0 && a < b);
    return a;
}

SPEEDMETER mSpeedMeter_create(int duration_ms, int precision_ms){
    mSpeedMeter_t *handle;

    handle = (mSpeedMeter_t *)calloc(1, sizeof(*handle));

    handle->m_precision_ms = precision_ms;
    assert(handle->m_precision_ms > 0);

    handle->m_size = duration_ms / precision_ms;
    assert(handle->m_size > 0);

    handle->m_records = (record_t *)calloc(handle->m_size, sizeof(handle->m_records[0]));

    return (SPEEDMETER) handle;
}

void mSpeedMeter_destroy(SPEEDMETER _handle){
    mSpeedMeter_t *handle = (mSpeedMeter_t *)_handle;
    assert(handle);

    if (handle->m_records) {
        free(handle->m_records);
    }
    free(handle);
}

void mSpeedMeter_append(SPEEDMETER _handle, int x, int y, unsigned int t){
    mSpeedMeter_t *handle = (mSpeedMeter_t *)_handle;
    int old_index = s_mod(handle->m_index - 1, handle->m_size);
    assert(handle->m_size > 0);
    assert(handle);

    if (handle->m_count > 0 && handle->m_records[old_index].m_t == t) {
        _DBG_PRINTF("M [%d] (%d,%u)\n", old_index, y, t);
        handle->m_records[old_index].m_x = x;
        handle->m_records[old_index].m_y = y;
    }else{
        _DBG_PRINTF("+ [%d] (%d,%u)\n", handle->m_index, y, t);
        handle->m_records[handle->m_index].m_x = x;
        handle->m_records[handle->m_index].m_y = y;
        handle->m_records[handle->m_index].m_t = t;

        if (++handle->m_index >= handle->m_size) {
            handle->m_index = 0;
        }
        if (handle->m_count < handle->m_size) {
            assert(handle->m_index < handle->m_size);
            ++ handle->m_count;
        }
    }
}

void mSpeedMeter_stop (SPEEDMETER _handle)
{
    mSpeedMeter_t *handle = (mSpeedMeter_t *)_handle;
    assert(handle);
}

void mSpeedMeter_reset (SPEEDMETER _handle)
{
    mSpeedMeter_t *handle = (mSpeedMeter_t *)_handle;
    assert(handle);
    handle->m_index = 0;
    handle->m_count = 0;
}

int mSpeedMeter_velocity (SPEEDMETER _handle, float *v_x, float *v_y)
{
    mSpeedMeter_t *handle = (mSpeedMeter_t *)_handle;
    int step, I;
    int i;
    float WX, WY, W;

    if (!handle) {
        *v_x = *v_y = 0.0f;
        return -1;
    }

    if (handle->m_count <= 1) {
        *v_x = 0.0f;
        *v_y = 0.0f;
        return -1;
    }

    WX = WY = W = 0;
    step = handle->m_count / 2;
    I = (handle->m_count / 2) + (handle->m_count % 2);

    for (i=0; i<I; ++i) {
        int i1, i2;
        unsigned int dT;
        float vx, vy, w;

        i2 = s_mod(handle->m_index - 1 - i, handle->m_size);
        i1 = s_mod(i2 - step, handle->m_size);

        dT = handle->m_records[i2].m_t - handle->m_records[i1].m_t; 
        if (dT <= 0) {
            fprintf(stderr, "[WARNING speedmeter]: step=%d, I=%d, t(%d)=%u t(%d)=%u\n",
                    step, I,
                    i1, handle->m_records[i1].m_t, 
                    i2, handle->m_records[i2].m_t);
            *v_x = 0.0f;
            *v_y = 0.0f;
            return -1;
        }
        vx = 1.0f * (handle->m_records[i2].m_x - handle->m_records[i1].m_x) / dT;
        vy = 1.0f * (handle->m_records[i2].m_y - handle->m_records[i1].m_y) / dT;
        w = 1.0f; /* TODO */

        _DBG_PRINTF("[%d] v=(%.2f, %.2f) dT=%d w=%.2f\n", i, vx, vy, dT, w);
        WX += vx * w;
        WY += vy * w;
        W += w;
    }
    *v_x = WX / W;
    *v_y = WY / W;

#ifdef _DEBUG
    {
        int i;
        for (i=0; i<handle->m_count; ++i) {
            int index = s_mod(handle->m_index - handle->m_count + i, handle->m_size);
            _DBG_PRINTF("[%d] %d %u\n", index, handle->m_records[index].m_y, handle->m_records[index].m_t);
        }
        _DBG_PRINTF("\n");
        _DBG_PRINTF("v_x=%.4f v_y=%.4f\n", 1000.0f * *v_x, 1000.0f * *v_y);
    }
#endif
    return 0;
}

/* VW (2020-03-08): */
int mSpeedMeter_query_velocity (SPEEDMETER _handle, float *v_x, float *v_y)
{
    float f_x, f_y;
    //mSpeedMeter_t *handle = (mSpeedMeter_t *)_handle;

    if (mSpeedMeter_velocity (_handle, &f_x, &f_y) == 0) {
        *v_x = 1000 * f_x;
        *v_y = 1000 * f_y;
        return 0;
    }

    return -1;
}

/* VW (2020-03-8): backward definiation */
int SpeedMeterProc (HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    // do nothging
    return 0;
}

/* VW (2020-03-8): backward definiation */
BOOL SpeedMeterMessageHandler(mWidget *notused, int message, int x, int y, DWORD keyStatus)
{
    // do nothging
    return FALSE;
}

#if 0   /* deprecated code */
/*
int mSpeedMeter_getpath(SPEEDMETER _handle, POINT *points, unsigned int *times, int count){
    mSpeedMeter_t *handle = (mSpeedMeter_t *)_handle;
    assert(handle);

    assert(0);
    return -1;
}
*/

static SPEEDMETER s_speedmeter;
static BOOL s_pressed;

#ifndef NCSCTRL_WIDGET
typedef int mWidget;
#endif
BOOL SpeedMeterMessageHandler(mWidget *notused, int message, int x, int y, DWORD keyStatus) {
    switch (message) {
        case MSG_LBUTTONDOWN:
            s_pressed = TRUE;
            if (! s_speedmeter){
                s_speedmeter = mSpeedMeter_create(1000, 10);
            }
            mSpeedMeter_reset(s_speedmeter);
            mSpeedMeter_append(s_speedmeter, x, y, GetTickCount() * 10);
            break;
        case MSG_MOUSEMOVE:
            if (s_pressed) {
                if (s_speedmeter) {
                    mSpeedMeter_append(s_speedmeter, x, y, GetTickCount() * 10);
                }else{
                    fprintf(stderr, "[WARNING speedmeter]: MSG_MOUSEMOVE, pressed, but speedmeter=NULL\n");
                }
            }
            break;
        case MSG_LBUTTONUP:
            if (s_speedmeter) {
                mSpeedMeter_append(s_speedmeter, x, y, GetTickCount() * 10);
                mSpeedMeter_stop(s_speedmeter);
            }else{
                fprintf(stderr, "[WARNING speedmeter]: MSG_LBUTTONUP, speedmeter=NULL\n");
            }
            s_pressed = FALSE;
            break;
    }
    return 0;
}

int SpeedMeterProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam){
    switch (message) {
        case MSG_MOUSEMOVE:
        case MSG_LBUTTONDOWN:
        case MSG_LBUTTONUP:
            SpeedMeterMessageHandler(NULL, message, LOSWORD(lParam), HISWORD(lParam), (DWORD)wParam);
            break;
        case MSG_DESTROY:
            mSpeedMeter_destroy(s_speedmeter);
            s_speedmeter = NULL;
            s_pressed = FALSE;
            break;
        default:
            break;
    }
    return 0;
}

int QueryMouseMoveVelocity(float *v_x, float *v_y){
    float f_x, f_y;
    if (mSpeedMeter_velocity(s_speedmeter, &f_x, &f_y) == 0) {
        *v_x = 1000 * f_x;
        *v_y = 1000 * f_y;
        return 0;
    }else{
        return -1;
    }
}

#endif   /* deprecated code */

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
//----------------------------------------------------------------------------
// Anti-Grain Geometry (AGG) - Version 2.5
// A high quality rendering engine for C++
// Copyright (C) 2002-2006 Maxim Shemanarev
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://antigrain.com
// 
// AGG is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// AGG is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with AGG; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
// MA 02110-1301, USA.
//----------------------------------------------------------------------------

#include "agg_vpgen_clip_polygon.h"
#include "agg_clip_liang_barsky.h"

namespace agg
{

    //------------------------------------------------------------------------
    // Determine the clipping code of the vertex according to the 
    // Cyrus-Beck line clipping algorithm
    //
    //        |        |
    //  0110  |  0010  | 0011
    //        |        |
    // -------+--------+-------- clip_box.y2
    //        |        |
    //  0100  |  0000  | 0001
    //        |        |
    // -------+--------+-------- clip_box.y1
    //        |        |
    //  1100  |  1000  | 1001
    //        |        |
    //  clip_box.x1  clip_box.x2
    //
    // 
    unsigned vpgen_clip_polygon::clipping_flags(double x, double y)
    {
        if(x < m_clip_box.x1) 
        {
            if(y > m_clip_box.y2) return 6;
            if(y < m_clip_box.y1) return 12;
            return 4;
        }

        if(x > m_clip_box.x2) 
        {
            if(y > m_clip_box.y2) return 3;
            if(y < m_clip_box.y1) return 9;
            return 1;
        }

        if(y > m_clip_box.y2) return 2;
        if(y < m_clip_box.y1) return 8;

        return 0;
    }

    //----------------------------------------------------------------------------
    void vpgen_clip_polygon::reset()
    {
        m_vertex = 0;
        m_num_vertices = 0;
    }

    //----------------------------------------------------------------------------
    void vpgen_clip_polygon::move_to(double x, double y)
    {
        m_vertex = 0;
        m_num_vertices = 0;
        m_clip_flags = clipping_flags(x, y);
        if(m_clip_flags == 0)
        {
            m_x[0] = x;
            m_y[0] = y;
            m_num_vertices = 1;
        }
        m_x1  = x;
        m_y1  = y;
        m_cmd = path_cmd_move_to;
    }


    //----------------------------------------------------------------------------
    void vpgen_clip_polygon::line_to(double x, double y)
    {
        m_vertex = 0;
        m_num_vertices = 0;
        unsigned flags = clipping_flags(x, y);

        if(m_clip_flags == flags)
        {
            if(flags == 0)
            {
                m_x[0] = x;
                m_y[0] = y;
                m_num_vertices = 1;
            }
        }
        else
        {
            m_num_vertices = clip_liang_barsky(m_x1, m_y1, 
                                               x, y, 
                                               m_clip_box, 
                                               m_x, m_y);
        }

        m_clip_flags = flags;
        m_x1 = x;
        m_y1 = y;
    }


    //----------------------------------------------------------------------------
    unsigned vpgen_clip_polygon::vertex(double* x, double* y)
    {
        if(m_vertex < m_num_vertices)
        {
            *x = m_x[m_vertex];
            *y = m_y[m_vertex];
            ++m_vertex;
            unsigned cmd = m_cmd;
            m_cmd = path_cmd_line_to;
            return cmd;
        }
        return path_cmd_stop;
    }


}

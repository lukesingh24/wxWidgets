/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/brush.h
// Purpose:     wxBrush class
// Author:      David Elliott <dfe@cox.net>
// Modified by:
// Created:     2003/07/03
// RCS-ID:      $Id$
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_BRUSH_H__
#define __WX_COCOA_BRUSH_H__

#include "wx/gdicmn.h"
#include "wx/gdiobj.h"
#include "wx/bitmap.h"

class WXDLLIMPEXP_FWD_CORE wxBrush;

// ========================================================================
// wxBrush
// ========================================================================
class WXDLLEXPORT wxBrush: public wxBrushBase
{
    DECLARE_DYNAMIC_CLASS(wxBrush)
// ------------------------------------------------------------------------
// initialization
// ------------------------------------------------------------------------
public:
    wxBrush();
    wxBrush(const wxColour& col, wxBrushStyle style = wxBRUSHSTYLE_SOLID);
    wxBrush(const wxBitmap& stipple);
    virtual ~wxBrush();

// ------------------------------------------------------------------------
// Implementation
// ------------------------------------------------------------------------
    virtual void SetColour(const wxColour& col) ;
    virtual void SetColour(unsigned char r, unsigned char g, unsigned char b) ;
    virtual void SetStyle(wxBrushStyle style) ;
    virtual void SetStipple(const wxBitmap& stipple) ;

    // comparison
    bool operator == (const wxBrush& brush) const
    {   return m_refData == brush.m_refData; }
    bool operator != (const wxBrush& brush) const
    {   return m_refData != brush.m_refData; }

    // accessors
    wxColour GetColour() const;
    virtual wxBrushStyle GetStyle() const;
    wxBitmap *GetStipple() const;

    // wxCocoa
    WX_NSColor GetNSColor();

protected:
    wxGDIRefData *CreateGDIRefData() const;
    wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;
};

#endif // __WX_COCOA_BRUSH_H__

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/dc.h
// Purpose:     wxDC
// Author:      David Elliott
// Modified by:
// Created:     2003/04/01
// RCS-ID:      $Id$
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_DC_H__
#define __WX_COCOA_DC_H__

class WXDLLEXPORT wxDC;
WX_DECLARE_LIST(wxDC, wxCocoaDCStack);

//=========================================================================
// wxDC
//=========================================================================
class WXDLLEXPORT wxDC: public wxDCBase
{
    DECLARE_DYNAMIC_CLASS(wxDC)
    DECLARE_NO_COPY_CLASS(wxDC)
//-------------------------------------------------------------------------
// Initialization
//-------------------------------------------------------------------------
public:
    wxDC();
    ~wxDC();
    
//-------------------------------------------------------------------------
// wxCocoa specifics
//-------------------------------------------------------------------------
public:
    static void CocoaInitializeTextSystem();
    static void CocoaShutdownTextSystem();
    static WX_NSTextStorage sm_cocoaNSTextStorage;
    static WX_NSLayoutManager sm_cocoaNSLayoutManager;
    static WX_NSTextContainer sm_cocoaNSTextContainer;
protected:
// DC stack
    static wxCocoaDCStack sm_cocoaDCStack;
    virtual bool CocoaLockFocus();
    virtual bool CocoaUnlockFocus();
    bool CocoaUnwindStackAndTakeFocus();
    inline bool CocoaTakeFocus()
    {
        wxCocoaDCStack::Node *node = sm_cocoaDCStack.GetFirst();
        if(node && (node->GetData() == this))
            return true;
        return CocoaUnwindStackAndTakeFocus();
    }
    void CocoaUnwindStackAndLoseFocus();
// DC flipping/transformation
    void CocoaApplyTransformations();
    float m_cocoaHeight;
    bool m_cocoaFlipped;
// Blitting
    virtual bool CocoaDoBlitOnFocusedDC(wxCoord xdest, wxCoord ydest,
        wxCoord width, wxCoord height, wxCoord xsrc, wxCoord ysrc,
        int logicalFunc, bool useMask, wxCoord xsrcMask, wxCoord ysrcMask);
//-------------------------------------------------------------------------
// Implementation
//-------------------------------------------------------------------------
public:
    // implement base class pure virtuals
    // ----------------------------------

    virtual void Clear();

    virtual bool StartDoc( const wxString& WXUNUSED(message) ) { return TRUE; }
    virtual void EndDoc(void) {};
    
    virtual void StartPage(void) {};
    virtual void EndPage(void) {};

    virtual void SetFont(const wxFont& font) {}
    virtual void SetPen(const wxPen& pen);
    virtual void SetBrush(const wxBrush& brush);
    virtual void SetBackground(const wxBrush& brush);
    virtual void SetBackgroundMode(int mode) { m_backgroundMode = mode; }
    virtual void SetPalette(const wxPalette& palette);

    virtual void DestroyClippingRegion();

    virtual wxCoord GetCharHeight() const;
    virtual wxCoord GetCharWidth() const;
    virtual void DoGetTextExtent(const wxString& string,
                                 wxCoord *x, wxCoord *y,
                                 wxCoord *descent = NULL,
                                 wxCoord *externalLeading = NULL,
                                 wxFont *theFont = NULL) const;

    virtual bool CanDrawBitmap() const;
    virtual bool CanGetTextExtent() const;
    virtual int GetDepth() const;
    virtual wxSize GetPPI() const;

    virtual void SetMapMode(int mode);
    virtual void SetUserScale(double x, double y);

    virtual void SetLogicalScale(double x, double y);
    virtual void SetLogicalOrigin(wxCoord x, wxCoord y);
    virtual void SetDeviceOrigin(wxCoord x, wxCoord y);
    virtual void SetAxisOrientation(bool xLeftRight, bool yBottomUp);
    virtual void SetLogicalFunction(int function);

    virtual void SetTextForeground(const wxColour& colour) ;
    virtual void SetTextBackground(const wxColour& colour) ;

    void ComputeScaleAndOrigin(void);
protected:
    virtual bool DoFloodFill(wxCoord x, wxCoord y, const wxColour& col,
                             int style = wxFLOOD_SURFACE);

    virtual bool DoGetPixel(wxCoord x, wxCoord y, wxColour *col) const;

    virtual void DoDrawPoint(wxCoord x, wxCoord y);
    virtual void DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);

    virtual void DoDrawArc(wxCoord x1, wxCoord y1,
                           wxCoord x2, wxCoord y2,
                           wxCoord xc, wxCoord yc);
    
    virtual void DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h,
                                   double sa, double ea);

    virtual void DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    virtual void DoDrawRoundedRectangle(wxCoord x, wxCoord y,
                                        wxCoord width, wxCoord height,
                                        double radius);
    virtual void DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height);

    virtual void DoCrossHair(wxCoord x, wxCoord y);

    virtual void DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y);
    virtual void DoDrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y,
                              bool useMask = FALSE);

    virtual void DoDrawText(const wxString& text, wxCoord x, wxCoord y);
    virtual void DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y,
                                   double angle);

    virtual bool DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                        wxDC *source, wxCoord xsrc, wxCoord ysrc,
                        int rop = wxCOPY, bool useMask = FALSE, wxCoord xsrcMask = -1, wxCoord ysrcMask = -1);

    // this is gnarly - we can't even call this function DoSetClippingRegion()
    // because of virtual function hiding
    virtual void DoSetClippingRegionAsRegion(const wxRegion& region);
    virtual void DoSetClippingRegion(wxCoord x, wxCoord y,
                                     wxCoord width, wxCoord height);

    virtual void DoGetSize(int *width, int *height) const;
    virtual void DoGetSizeMM(int* width, int* height) const;

    virtual void DoDrawLines(int n, wxPoint points[],
                             wxCoord xoffset, wxCoord yoffset);
    virtual void DoDrawPolygon(int n, wxPoint points[],
                               wxCoord xoffset, wxCoord yoffset,
                               int fillStyle = wxODDEVEN_RULE);
};

#endif // __WX_COCOA_DC_H__

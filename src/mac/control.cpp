/////////////////////////////////////////////////////////////////////////////
// Name:        control.cpp
// Purpose:     wxControl class
// Author:      AUTHOR
// Modified by:
// Created:     ??/??/98
// RCS-ID:      $Id$
// Copyright:   (c) AUTHOR
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "control.h"
#endif

#include "wx/defs.h"

#include "wx/control.h"
#include "wx/panel.h"
#include "wx/app.h"
#include "wx/dc.h"
#include "wx/dcclient.h"
#include "wx/notebook.h"
#include "wx/tabctrl.h"
#include "wx/radiobox.h"
#include "wx/spinbutt.h"
#include "wx/scrolbar.h"
#include "wx/button.h"
#include "wx/dialog.h"
#include "wx/statbox.h"
#include "wx/sizer.h"
#include "wx/stattext.h"

#if !USE_SHARED_LIBRARY
IMPLEMENT_ABSTRACT_CLASS(wxControl, wxWindow)

BEGIN_EVENT_TABLE(wxControl, wxWindow)
    EVT_MOUSE_EVENTS( wxControl::OnMouseEvent ) 
    EVT_CHAR( wxControl::OnKeyDown ) 
    EVT_PAINT( wxControl::OnPaint ) 
END_EVENT_TABLE()
#endif

#include "wx/mac/uma.h"

// Item members

ControlActionUPP wxMacLiveScrollbarActionUPP = NULL ;

pascal void wxMacLiveScrollbarActionProc( ControlHandle control , ControlPartCode partCode ) ;
pascal void wxMacLiveScrollbarActionProc( ControlHandle control , ControlPartCode partCode )
{
    if ( partCode != 0)
    {
        wxControl*  wx = (wxControl*) GetControlReference( control ) ;
        if ( wx )
        {
            wx->MacHandleControlClick( control , partCode ) ;
        }
    }
}

wxControl::wxControl()
{
    m_macControl = NULL ;
    m_macHorizontalBorder = 0 ; // additional pixels around the real control
    m_macVerticalBorder = 0 ;
    m_backgroundColour = *wxWHITE;
    m_foregroundColour = *wxBLACK;
#if WXWIN_COMPATIBILITY
  m_callback = 0;
#endif // WXWIN_COMPATIBILITY

    if ( wxMacLiveScrollbarActionUPP == NULL )
    {
#if defined(UNIVERSAL_INTERFACES_VERSION) && (UNIVERSAL_INTERFACES_VERSION >= 0x0340)
        wxMacLiveScrollbarActionUPP = NewControlActionUPP( wxMacLiveScrollbarActionProc );
#else
        wxMacLiveScrollbarActionUPP = NewControlActionProc( wxMacLiveScrollbarActionProc ) ;
#endif
    }
}

bool wxControl::Create(wxWindow *parent, wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size, long style,
                       const wxValidator& validator,
                       const wxString& name)
{
    m_macControl = NULL ;
    m_macHorizontalBorder = 0 ; // additional pixels around the real control
    m_macVerticalBorder = 0 ;
    bool rval = wxWindow::Create(parent, id, pos, size, style, name);
    if (rval) {
#if wxUSE_VALIDATORS
        SetValidator(validator);
#endif
    }
    return rval;
}

wxControl::~wxControl()
{
    m_isBeingDeleted = TRUE;
    // If we delete an item, we should initialize the parent panel,
    // because it could now be invalid.
    wxWindow *parent = GetParent() ;
    if ( parent )
    {
        if (parent->GetDefaultItem() == (wxButton*) this)
            parent->SetDefaultItem(NULL);
    }
    if ( (ControlHandle) m_macControl )
    {
        ::DisposeControl( (ControlHandle) m_macControl ) ;
        m_macControl = NULL ;
    }
}

void wxControl::SetLabel(const wxString& title)
{
    m_label = title ;

    if ( (ControlHandle) m_macControl )
    {
        Str255 maclabel ;
        wxString label ;
    
        if( wxApp::s_macDefaultEncodingIsPC )
            label = wxMacMakeMacStringFromPC( title ) ;
        else
            label = title ;
        
#if TARGET_CARBON
        c2pstrcpy( (StringPtr) maclabel , label ) ;
#else
        strcpy( (char *) maclabel , label ) ;
        c2pstr( (char *) maclabel ) ;
#endif
        ::SetControlTitle( (ControlHandle) m_macControl , maclabel ) ;
    }
    Refresh() ;
}

wxSize wxControl::DoGetBestSize() const
{
    Rect    bestsize = { 0 , 0 , 0 , 0 } ;
    short   baselineoffset ;
    int bestWidth, bestHeight ;
    ::GetBestControlRect( (ControlHandle) m_macControl , &bestsize , &baselineoffset ) ;

    if ( EmptyRect( &bestsize ) )
    {
        baselineoffset = 0;
        bestsize.left = bestsize.top = 0 ;
        bestsize.right = 16 ;
        bestsize.bottom = 16 ;
        if ( IsKindOf( CLASSINFO( wxScrollBar ) ) )
        {
            bestsize.bottom = 16 ;
        }
        else if ( IsKindOf( CLASSINFO( wxSpinButton ) ) )
        {
            bestsize.bottom = 24 ; 
        }
    }

    if ( IsKindOf( CLASSINFO( wxButton ) ) )
    {
        bestWidth = m_label.Length() * 8 + 12 ;
        if ( bestWidth < 70 )
          bestWidth = 70 ;
    }
    else if ( IsKindOf( CLASSINFO( wxStaticText ) ) )
    {
        bestWidth = m_label.Length() * 8 ;
    }
    else
        bestWidth = bestsize.right - bestsize.left ;
        
    bestWidth += 2 * m_macHorizontalBorder ;

    bestHeight = bestsize.bottom - bestsize.top ;
    if ( bestHeight < 10 )
        bestHeight = 13 ;

    bestHeight += 2 * m_macVerticalBorder;

        
    return wxSize(bestWidth, bestHeight);
}

bool wxControl::ProcessCommand (wxCommandEvent & event)
{
  // Tries:
  // 1) A callback function (to become obsolete)
  // 2) OnCommand, starting at this window and working up parent hierarchy
  // 3) OnCommand then calls ProcessEvent to search the event tables.
#if WXWIN_COMPATIBILITY
    if ( m_callback )
    {
        (void)(*m_callback)(this, event);

        return TRUE;
    }
    else
#endif // WXWIN_COMPATIBILITY
    {
      return GetEventHandler()->ProcessEvent(event);
    }
}

// ------------------------
wxList *wxWinMacControlList = NULL;
wxControl *wxFindControlFromMacControl(ControlHandle inControl )
{
    wxNode *node = wxWinMacControlList->Find((long)inControl);
    if (!node)
        return NULL;
    return (wxControl *)node->Data();
}

void wxAssociateControlWithMacControl(ControlHandle inControl, wxControl *control)
{
    // adding NULL WindowRef is (first) surely a result of an error and
    // (secondly) breaks menu command processing
    wxCHECK_RET( inControl != (ControlHandle) NULL, "attempt to add a NULL WindowRef to window list" );

    if ( !wxWinMacControlList->Find((long)inControl) )
        wxWinMacControlList->Append((long)inControl, control);
}

void wxRemoveMacControlAssociation(wxControl *control)
{
    wxWinMacControlList->DeleteObject(control);
}

void wxControl::MacPreControlCreate( wxWindow *parent, wxWindowID id, wxString label , 
             const wxPoint& pos,
             const wxSize& size, long style,
             const wxValidator& validator,
             const wxString& name , WXRECTPTR outBounds , unsigned char* maclabel ) 
{
    m_label = label ;
  SetName(name);
  if ( &validator )
    SetValidator(validator);

  m_windowStyle = style;
  parent->AddChild(this);

  m_backgroundColour = parent->GetBackgroundColour() ;
  m_foregroundColour = parent->GetForegroundColour() ;

  if (id == -1)
      m_windowId = NewControlId();
  else
      m_windowId = id;

   // These sizes will be adjusted in MacPostControlCreate

    m_width = size.x ;
    m_height = size.y ;
    m_x = pos.x ;
    m_y = pos.y ;
        
    ((Rect*)outBounds)->top = -10;
    ((Rect*)outBounds)->left = -10;
    ((Rect*)outBounds)->bottom = 0;
    ((Rect*)outBounds)->right = 0;

    char c_text[255];
    strcpy( c_text , label ) ;
    if( wxApp::s_macDefaultEncodingIsPC )
    {
        wxMacConvertFromPCForControls( c_text ) ;
    }

#if TARGET_CARBON
    c2pstrcpy( (StringPtr) maclabel , c_text ) ;
#else
    strcpy( (char *) maclabel , c_text ) ;
    c2pstr( (char *) maclabel ) ;
#endif
}

void wxControl::MacPostControlCreate()
{
    wxASSERT_MSG( (ControlHandle) m_macControl != NULL , "No valid mac control" ) ;
    
    if ( IsKindOf( CLASSINFO( wxScrollBar ) ) )
    {
        // no font
    }
    else if ( !UMAHasAquaLayout() && (IsKindOf( CLASSINFO( wxStaticBox ) ) || IsKindOf( CLASSINFO( wxRadioBox ) ) || IsKindOf( CLASSINFO( wxButton ) ) ) )
    {
        ControlFontStyleRec     controlstyle ;
        controlstyle.flags = kControlUseFontMask ;
        controlstyle.font = kControlFontSmallBoldSystemFont ;
        
        ::SetControlFontStyle( (ControlHandle) m_macControl , &controlstyle ) ;
    }
    else
    {
        ControlFontStyleRec     controlstyle ;
        controlstyle.flags = kControlUseFontMask ;
        controlstyle.font = kControlFontSmallSystemFont ;
        
        ::SetControlFontStyle( (ControlHandle) m_macControl , &controlstyle ) ;
    }
    ControlHandle container = (ControlHandle) GetParent()->MacGetContainerForEmbedding() ;
    wxASSERT_MSG( container != NULL , wxT("No valid mac container control") ) ;
    ::EmbedControl( (ControlHandle) m_macControl , container ) ;
    m_macControlIsShown  = true ;

    wxAssociateControlWithMacControl( (ControlHandle) m_macControl , this ) ;

 
     // Adjust the controls size and position
     wxPoint pos(m_x, m_y);
     wxSize best_size( DoGetBestSize() );
     wxSize new_size( m_width, m_height );
 
     m_x = m_y = m_width = m_height = -1;  // Forces SetSize to move/size the control
 
     if (new_size.x == -1) {
        new_size.x = best_size.x;
     }
     if (new_size.y == -1) {
         new_size.y = best_size.y;
     }
 
     SetSize(pos.x, pos.y, new_size.x, new_size.y);
 
     UMAShowControl( (ControlHandle) m_macControl ) ;
     Refresh() ;
}

void wxControl::MacAdjustControlRect() 
{
    wxASSERT_MSG( (ControlHandle) m_macControl != NULL , wxT("No valid mac control") ) ;
    if ( m_width == -1 || m_height == -1 )
    {
        Rect    bestsize = { 0 , 0 , 0 , 0 } ;
        short   baselineoffset ;
        
        ::GetBestControlRect( (ControlHandle) m_macControl , &bestsize , &baselineoffset ) ;

        if ( EmptyRect( &bestsize ) )
        {
            baselineoffset = 0;
            bestsize.left = bestsize.top = 0 ;
            bestsize.right = 16 ;
            bestsize.bottom = 16 ;
            if ( IsKindOf( CLASSINFO( wxScrollBar ) ) )
            {
                bestsize.bottom = 16 ;
            }
            else if ( IsKindOf( CLASSINFO( wxSpinButton ) ) )
            {
                bestsize.bottom = 24 ; 
            }
        }

        if ( m_width == -1 )
        {
            if ( IsKindOf( CLASSINFO( wxButton ) ) )
            {
                m_width = m_label.Length() * 8 + 12 ;
                if ( m_width < 70 )
                  m_width = 70 ;
            }
            else if ( IsKindOf( CLASSINFO( wxStaticText ) ) )
            {
                m_width = m_label.Length() * 8 ;
            }
            else
                m_width = bestsize.right - bestsize.left ;
            
            m_width += 2 * m_macHorizontalBorder ;
        }
        if ( m_height == -1 )
        {
            m_height = bestsize.bottom - bestsize.top ;
            if ( m_height < 10 )
                m_height = 13 ;

            m_height += 2 * m_macVerticalBorder;
        }
        
        UMASizeControl( (ControlHandle) m_macControl , m_width - 2 * m_macHorizontalBorder, m_height -  2 * m_macVerticalBorder ) ;
    }
}

WXWidget wxControl::MacGetContainerForEmbedding() 
{
    if ( m_macControl )
        return m_macControl ;

    return wxWindow::MacGetContainerForEmbedding() ;
}

void wxControl::MacSuperChangedPosition() 
{
    if ( (ControlHandle) m_macControl )
    {
        Rect contrlRect ;       
        GetControlBounds( (ControlHandle) m_macControl , &contrlRect ) ; 
        int former_mac_x = contrlRect.left ;
        int former_mac_y = contrlRect.top ;
        int mac_x = m_x ;
        int mac_y = m_y ;
        GetParent()->MacWindowToRootWindow( & mac_x , & mac_y ) ;
        
        WindowRef rootwindow = (WindowRef) MacGetRootWindow() ;

        if ( mac_x + m_macHorizontalBorder != former_mac_x || 
            mac_y + m_macVerticalBorder != former_mac_y )
        {
            {
                Rect inval = { former_mac_y , former_mac_x , former_mac_y + m_height , former_mac_x + m_width } ;
                InvalWindowRect( rootwindow , &inval ) ;
            }
            UMAMoveControl( (ControlHandle) m_macControl , mac_x + m_macHorizontalBorder , mac_y + m_macVerticalBorder ) ;
            {
                Rect inval = { mac_y , mac_x , mac_y + m_height , mac_x + m_width } ;
                InvalWindowRect( rootwindow , &inval ) ;
            }
        }
    }

    wxWindow::MacSuperChangedPosition() ;
}

void wxControl::MacSuperEnabled( bool enabled ) 
{
    Refresh(FALSE) ;
    wxWindow::MacSuperEnabled( enabled ) ;
}

void  wxControl::MacSuperShown( bool show ) 
{
    if ( (ControlHandle) m_macControl )
    {
        if ( !show )
        {
            if ( m_macControlIsShown )
            {
                ::UMAHideControl( (ControlHandle) m_macControl ) ;
                m_macControlIsShown = false ;
            }
        }
        else
        {
            if ( MacIsReallyShown() && !m_macControlIsShown )
            {
                ::UMAShowControl( (ControlHandle) m_macControl ) ;
                m_macControlIsShown = true ;
            }
        }
    }
        
    wxWindow::MacSuperShown( show ) ;
}

void  wxControl::DoSetSize(int x, int y,
            int width, int height,
            int sizeFlags )
{
    if ( (ControlHandle) m_macControl == NULL )
    {
        wxWindow::DoSetSize( x , y ,width , height ,sizeFlags ) ;
        return ;
    }

     Rect oldbounds;
     int new_x, new_y, new_width, new_height;
     int mac_x, mac_y;
 
     new_x = m_x;
     new_y = m_y;
     new_width  = m_width;
     new_height = m_height;
 
     if (sizeFlags & wxSIZE_ALLOW_MINUS_ONE)
     {
         new_x      = x;
         new_y      = y;
         new_width  = width;
         new_height = height;
     }
     else
     {
         if (x != -1)      new_x      = x;
         if (y != -1)      new_y      = y;
         if (width != -1)  new_width  = width;
         if (height != -1) new_height = height;
     }
 
     if(sizeFlags & wxSIZE_AUTO)
     {
         wxSize size = GetBestSize();
         if (sizeFlags & wxSIZE_AUTO_WIDTH)
         {
             if (width == -1)   new_width = size.x;
         }
         if (sizeFlags & wxSIZE_AUTO_HEIGHT)
         {
             if (height == -1)   new_height = size.y;
         }
     }
     AdjustForParentClientOrigin(new_x, new_y, sizeFlags);
 
     mac_x = new_x;
     mac_y = new_y;
     if(GetParent()) {
         GetParent()->MacWindowToRootWindow(&mac_x, &mac_y);
     }
     GetControlBounds( (ControlHandle) m_macControl, &oldbounds);
     oldbounds.right = oldbounds.left + m_width;
     oldbounds.bottom = oldbounds.top + m_height;
 
     bool doMove = false;
     bool doResize = false;
 
     if ( mac_x != (oldbounds.left - m_macHorizontalBorder) ||
          mac_y != (oldbounds.top - m_macVerticalBorder) )
     {
         doMove = true ;
     }
     if ( new_width != oldbounds.right - oldbounds.left - 2 * m_macHorizontalBorder ||
          new_height != oldbounds.bottom - oldbounds.top - 2 * m_macVerticalBorder)
     {
         doResize = true ;
     }
 
     if ( doMove || doResize )
     {
         Refresh() ;
     
         // Ensure resize is within constraints
         if ((m_minWidth != -1) && (new_width < m_minWidth)) {
             new_width = m_minWidth;
         }
         if ((m_minHeight != -1) && (new_height < m_minHeight)) {
             new_height = m_minHeight;
         }
         if ((m_maxWidth != -1) && (new_width > m_maxWidth)) {
             new_width = m_maxWidth;
         }
         if ((m_maxHeight != -1) && (new_height > m_maxHeight)) {
             new_height = m_maxHeight;
         }
 
         if ( doMove )
         {
             m_x = new_x;
             m_y = new_y;
 
             UMAMoveControl( (ControlHandle) m_macControl,
                            mac_x + m_macHorizontalBorder, mac_y + m_macVerticalBorder);

             wxMoveEvent event(wxPoint(m_x, m_y), m_windowId);
             event.SetEventObject(this);
             GetEventHandler()->ProcessEvent(event) ;
         }
         if ( doResize )
         {
             m_width = new_width;
             m_height = new_height;
 
             UMASizeControl( (ControlHandle) m_macControl,
                             m_width - 2 * m_macHorizontalBorder,
                             m_height -  2 * m_macVerticalBorder ) ;
 
 
             wxSizeEvent event(wxSize(m_width, m_height), m_windowId);
             event.SetEventObject(this);
             GetEventHandler()->ProcessEvent(event);
         }
 
          Refresh() ;
     }
}

bool  wxControl::Show(bool show) 
{
    if ( !wxWindow::Show( show ) )
        return FALSE ;
        
    if ( (ControlHandle) m_macControl )
    {
        if ( !show )
        {
            if ( m_macControlIsShown )
            {
                ::UMAHideControl( (ControlHandle) m_macControl ) ;
                m_macControlIsShown = false ;
            }
        }
        else
        {
            if ( MacIsReallyShown() && !m_macControlIsShown )
            {
                ::UMAShowControl( (ControlHandle) m_macControl ) ;
                m_macControlIsShown = true ;
            }
        }
    }
    return TRUE ;
}

bool  wxControl::Enable(bool enable) 
{
    if ( !wxWindow::Enable(enable) )
        return FALSE;

    if ( (ControlHandle) m_macControl )
    {
        if ( enable )
            UMAActivateControl( (ControlHandle) m_macControl ) ;
        else
            UMADeactivateControl( (ControlHandle) m_macControl ) ;
    }
    return TRUE ;
}

void wxControl::Refresh(bool eraseBack, const wxRect *rect)
{
    wxWindow::Refresh( eraseBack , rect ) ;
}

void wxControl::MacRedrawControl()
{
    if ( (ControlHandle) m_macControl && MacGetRootWindow() )
    {
        wxClientDC dc(this) ;
        wxMacPortSetter helper(&dc) ;
        
        // the controls sometimes draw outside their boundaries, this
        // should be resolved differently but is not trivial (e.g. drop shadows)
        // since adding them to the border would yield in enormous gaps between
        // the controls
        Rect r = { 0 , 0 , 32000 , 32000 } ;
        ClipRect( &r ) ;
        wxDC::MacSetupBackgroundForCurrentPort( MacGetBackgroundBrush() ) ;
        UMADrawControl( (ControlHandle) m_macControl ) ;
    }
}

void wxControl::OnPaint(wxPaintEvent& event)
{
    if ( (ControlHandle) m_macControl )
    {
        wxPaintDC dc(this) ;
        wxMacPortSetter helper(&dc) ;
        // the controls sometimes draw outside their boundaries, this
        // should be resolved differently but is not trivial (e.g. drop shadows)
        // since adding them to the border would yield in enormous gaps between
        // the controls
        Rect r = { 0 , 0 , 32000 , 32000 } ;
        ClipRect( &r ) ;

        wxDC::MacSetupBackgroundForCurrentPort( MacGetBackgroundBrush() ) ;
        UMADrawControl( (ControlHandle) m_macControl ) ;
    }
    else
    {
      event.Skip() ;
    }
}
void wxControl::OnEraseBackground(wxEraseEvent& event)
{
    wxWindow::OnEraseBackground( event ) ; 
}


void  wxControl::OnKeyDown( wxKeyEvent &event ) 
{
    if ( (ControlHandle) m_macControl == NULL )
        return ;
    
    EventRecord *ev = (EventRecord*) wxTheApp->MacGetCurrentEvent() ;
    short keycode ;
    short keychar ;
    keychar = short(ev->message & charCodeMask);
    keycode = short(ev->message & keyCodeMask) >> 8 ;

    ::HandleControlKey( (ControlHandle) m_macControl , keycode , keychar , ev->modifiers ) ;
}

void  wxControl::OnMouseEvent( wxMouseEvent &event ) 
{
    if ( (ControlHandle) m_macControl == NULL )
    {
        event.Skip() ;
        return ;
    }
        
    if (event.GetEventType() == wxEVT_LEFT_DOWN || event.GetEventType() == wxEVT_LEFT_DCLICK )
    {
            
        int x = event.m_x ;
        int y = event.m_y ;
        
        MacClientToRootWindow( &x , &y ) ;
            
        ControlHandle   control ;
        Point       localwhere ;
        SInt16      controlpart ;
        WindowRef   window = (WindowRef) MacGetRootWindow() ;
        
        localwhere.h = x ;
        localwhere.v = y ;
    
        short modifiers = 0;
        
        if ( !event.m_leftDown && !event.m_rightDown )
            modifiers  |= btnState ;
    
        if ( event.m_shiftDown )
            modifiers |= shiftKey ;
            
        if ( event.m_controlDown )
            modifiers |= controlKey ;
    
        if ( event.m_altDown )
            modifiers |= optionKey ;
    
        if ( event.m_metaDown )
            modifiers |= cmdKey ;
/*
#if TARGET_CARBON
        control = FindControlUnderMouse( localwhere , window , &controlpart ) ;
#else
        controlpart = FindControl( localwhere , window , &control ) ;
#endif
*/
        {
        /*
            if ( AcceptsFocus() && FindFocus() != this )
            {
                SetFocus() ;
            }
        */
            control = (ControlHandle) m_macControl ;
            if ( control && ::IsControlActive( control ) )
            {
                {
                    controlpart = ::HandleControlClick( control , localwhere , modifiers , (ControlActionUPP) -1 ) ;
                    wxTheApp->s_lastMouseDown = 0 ;
                    if ( control && controlpart != kControlNoPart && 
                        ! IsKindOf( CLASSINFO( wxScrollBar ) ) 
                    ) // otherwise we will get the event twice for scrollbar
                    {
                        MacHandleControlClick( control , controlpart ) ;
                    }
                }
            }
        }
    }
}

bool wxControl::MacCanFocus() const
{
        { if ( (ControlHandle) m_macControl == NULL ) 
                return true ; 
            else
            return false ; 
        }
}

void wxControl::MacHandleControlClick( WXWidget control , wxInt16 controlpart ) 
{
    wxASSERT_MSG( (ControlHandle) m_macControl != NULL , "No valid mac control" ) ;
}


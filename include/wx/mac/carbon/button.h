/////////////////////////////////////////////////////////////////////////////
// Name:        button.h
// Purpose:     wxButton class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BUTTON_H_
#define _WX_BUTTON_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "button.h"
#endif

#include "wx/control.h"
#include "wx/gdicmn.h"

WXDLLEXPORT_DATA(extern const wxChar*) wxButtonNameStr;

// Pushbutton
class WXDLLEXPORT wxButton: public wxButtonBase
{
  DECLARE_DYNAMIC_CLASS(wxButton)
 public:
  inline wxButton() {}
  inline wxButton(wxWindow *parent, wxWindowID id, const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxButtonNameStr)
  {
      Create(parent, id, label, pos, size, style, validator, name);
  }
  
  inline wxButton(wxWindow *parent, wxWindowID id, wxStockItemID stock,
           const wxString& descriptiveLabel = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxButtonNameStr)
  {
      Create(parent, id, stock, descriptiveLabel, pos, style, validator, name);
  }

  bool Create(wxWindow *parent, wxWindowID id, const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxButtonNameStr);
    
  bool Create(wxWindow *parent, wxWindowID id, wxStockItemID stock,
           const wxString& descriptiveLabel = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxButtonNameStr)
  {
      return CreateStock(parent, id, stock, descriptiveLabel,
                         pos, style, validator, name);
  }

    virtual wxInt32 MacControlHit( WXEVENTHANDLERREF handler , WXEVENTREF event ) ;
    static wxSize GetDefaultSize();

  virtual void SetDefault();
  virtual void Command(wxCommandEvent& event);
protected:
    virtual wxSize DoGetBestSize() const ;
};

#endif
    // _WX_BUTTON_H_

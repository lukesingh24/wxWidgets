/////////////////////////////////////////////////////////////////////////////
// Name:        wx/mac/carbon/listbox.h
// Purpose:     wxListBox class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_LISTBOX_H_
#define _WX_LISTBOX_H_

// ----------------------------------------------------------------------------
// simple types
// ----------------------------------------------------------------------------
#include  "wx/dynarray.h"
#include  "wx/arrstr.h"

// forward decl for GetSelections()
class wxArrayInt;

// exposed for subclasses like wxCheckListBox

class wxMacListControl
{
public:
    virtual void            MacDelete( unsigned int n ) = 0;
    virtual void            MacInsert( unsigned int n, const wxString& item ) = 0;
    virtual void            MacInsert( unsigned int n, const wxArrayString& items ) = 0;
    // returns index of newly created line
    virtual int             MacAppend( const wxString& item ) = 0;
    virtual void            MacSetString( unsigned int n, const wxString& item ) = 0;
    virtual void            MacClear() = 0;
    virtual void            MacDeselectAll() = 0;
    virtual void            MacSetSelection( unsigned int n, bool select ) = 0;
    virtual int             MacGetSelection() const = 0;
    virtual int             MacGetSelections( wxArrayInt& aSelections ) const = 0;
    virtual bool            MacIsSelected( unsigned int n ) const = 0;
    virtual void            MacScrollTo( unsigned int n ) = 0;
    virtual wxString        MacGetString( unsigned int n) const = 0;
    virtual unsigned int    MacGetCount() const = 0;

    virtual void            MacSetClientData( unsigned int n, void * data) = 0;
    virtual void *          MacGetClientData( unsigned int) const = 0;

    virtual wxMacListControl() { }
};

// List box item

WX_DEFINE_ARRAY( char* , wxListDataArray );

// ----------------------------------------------------------------------------
// List box control
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxListBox : public wxListBoxBase
{
public:
    // ctors and such
    wxListBox();

    wxListBox(
        wxWindow *parent,
        wxWindowID winid,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        int n = 0, const wxString choices[] = NULL,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxListBoxNameStr)
    {
        Create(parent, winid, pos, size, n, choices, style, validator, name);
    }

    wxListBox(
        wxWindow *parent,
        wxWindowID winid,
        const wxPoint& pos,
        const wxSize& size,
        const wxArrayString& choices,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxListBoxNameStr)
    {
        Create(parent, winid, pos, size, choices, style, validator, name);
    }

    bool Create(
        wxWindow *parent,
        wxWindowID winid,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        int n = 0,
        const wxString choices[] = NULL,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxListBoxNameStr);

    bool Create(
        wxWindow *parent,
        wxWindowID winid,
        const wxPoint& pos,
        const wxSize& size,
        const wxArrayString& choices,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxListBoxNameStr);

    virtual ~wxListBox();

    // implement base class pure virtuals
    virtual void Refresh(bool eraseBack = true, const wxRect *rect = NULL);
    virtual void Clear();
    virtual void Delete(unsigned int n);

    virtual unsigned int GetCount() const;
    virtual wxString GetString(unsigned int n) const;
    virtual void SetString(unsigned int n, const wxString& s);
    virtual int FindString(const wxString& s, bool bCase = false) const;

    virtual bool IsSelected(int n) const;
    virtual int GetSelection() const;
    virtual int GetSelections(wxArrayInt& aSelections) const;

    virtual void EnsureVisible(int n);

    // wxCheckListBox support
    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);

    wxMacListControl* GetPeer() const;

protected:
    // from wxItemContainer
    virtual int DoAppend(const wxString& item);
    virtual void DoSetItemClientData(unsigned int n, void* clientData);
    virtual void* DoGetItemClientData(unsigned int n) const;
    virtual void DoSetItemClientObject(unsigned int n, wxClientData* clientData);
    virtual wxClientData* DoGetItemClientObject(unsigned int n) const;

    // from wxListBoxBase
    virtual void DoSetSelection(int n, bool select);
    virtual void DoInsertItems(const wxArrayString& items, unsigned int pos);
    virtual void DoSetItems(const wxArrayString& items, void **clientData);
    virtual void DoSetFirstItem(int n);
    virtual int DoListHitTest(const wxPoint& point) const;

    // free memory (common part of Clear() and dtor)
    // prevent collision with some BSD definitions of macro Free()
    void FreeData();

    virtual wxSize DoGetBestSize() const;

private:
    DECLARE_DYNAMIC_CLASS(wxListBox)
    DECLARE_EVENT_TABLE()
};

#endif // _WX_LISTBOX_H_

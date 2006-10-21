/////////////////////////////////////////////////////////////////////////////
// Name:        listctrl.h
// Purpose:     wxListCtrl class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_LISTCTRL_H_
#define _WX_LISTCTRL_H_

#include "wx/generic/listctrl.h"

class wxMacDataBrowserListCtrlControl;
class wxMacListControl;

class WXDLLEXPORT wxListCtrl: public wxControl
{
  DECLARE_DYNAMIC_CLASS(wxListCtrl)
 public:
  /*
   * Public interface
   */

    wxListCtrl() { Init(); }

    wxListCtrl(wxWindow *parent,
               wxWindowID id = wxID_ANY,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxLC_ICON,
               const wxValidator& validator = wxDefaultValidator,
               const wxString& name = wxListCtrlNameStr)
    {
        Init();

        Create(parent, id, pos, size, style, validator, name);
    }

    virtual ~wxListCtrl();

    bool Create(wxWindow *parent,
                wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxLC_ICON,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxListCtrlNameStr);

  // Attributes
  ////////////////////////////////////////////////////////////////////////////

  // Gets information about this column
  bool GetColumn(int col, wxListItem& item) const;

  // Sets information about this column
  // TODO: NOT const to be compatible with wxGenericListCtrl API
  bool SetColumn(int col, wxListItem& item) ;

  // Gets the column width
  int GetColumnWidth(int col) const;

  // Sets the column width
  bool SetColumnWidth(int col, int width) ;

  // Gets the number of items that can fit vertically in the
  // visible area of the list control (list or report view)
  // or the total number of items in the list control (icon
  // or small icon view)
  int GetCountPerPage() const;

  // Gets the edit control for editing labels.
  wxTextCtrl* GetEditControl() const;

  // Gets information about the item
  bool GetItem(wxListItem& info) const ;

  // Sets information about the item
  bool SetItem(wxListItem& info) ;

  // Sets a string field at a particular column
  long SetItem(long index, int col, const wxString& label, int imageId = -1);

  // Gets the item state
  int  GetItemState(long item, long stateMask) const ;

  // Sets the item state
  bool SetItemState(long item, long state, long stateMask) ;

  void AssignImageList(wxImageList *imageList, int which);

  // Sets the item image
  bool SetItemImage(long item, int image, int selImage = -1) ;
  bool SetItemColumnImage(long item, long column, int image);

  // Gets the item text
  wxString GetItemText(long item) const ;

  // Sets the item text
  void SetItemText(long item, const wxString& str) ;
  
  void SetItemTextColour(long item, const wxColour& colour) ;
  wxColour GetItemTextColour(long item) const;
  
  void SetItemBackgroundColour(long item, const wxColour& colour) ;
  wxColour GetItemBackgroundColour(long item) const;

  void SetItemFont( long item, const wxFont &f);
  wxFont GetItemFont( long item ) const;

  // Gets the item data
  long GetItemData(long item) const ;

  // Sets the item data
  bool SetItemData(long item, long data) ;

  // Gets the item rectangle
  bool GetItemRect(long item, wxRect& rect, int code = wxLIST_RECT_BOUNDS) const ;

  // Gets the item position
  bool GetItemPosition(long item, wxPoint& pos) const ;

  // Sets the item position
  bool SetItemPosition(long item, const wxPoint& pos) ;

  // Gets the number of items in the list control
  int GetItemCount() const;

  // Gets the number of columns in the list control
  int GetColumnCount() const;

  void SetItemSpacing( int spacing, bool isSmall = false );
  wxSize GetItemSpacing() const;

  // Gets the number of selected items in the list control
  int GetSelectedItemCount() const;
  
  wxRect GetViewRect() const;

  // Gets the text colour of the listview
  wxColour GetTextColour() const;

  // Sets the text colour of the listview
  void SetTextColour(const wxColour& col);

  // Gets the index of the topmost visible item when in
  // list or report view
  long GetTopItem() const ;

  // are we in report mode?
  bool InReportView() const { return HasFlag(wxLC_REPORT); }

  bool IsVirtual() const { return HasFlag(wxLC_VIRTUAL); }

  // Add or remove a single window style
  void SetSingleStyle(long style, bool add = true) ;

  // Set the whole window style
  void SetWindowStyleFlag(long style) ;

  // Searches for an item, starting from 'item'.
  // item can be -1 to find the first item that matches the
  // specified flags.
  // Returns the item or -1 if unsuccessful.
  long GetNextItem(long item, int geometry = wxLIST_NEXT_ALL, int state = wxLIST_STATE_DONTCARE) const ;

  // Implementation: converts wxWidgets style to MSW style.
  // Can be a single style flag or a bit list.
  // oldStyle is 'normalised' so that it doesn't contain
  // conflicting styles.
  long ConvertToMSWStyle(long& oldStyle, long style) const;

  // Gets one of the three image lists
  wxImageList *GetImageList(int which) const ;

  // Sets the image list
  // N.B. There's a quirk in the Win95 list view implementation.
  // If in wxLC_LIST mode, it'll *still* display images by the labels if
  // there's a small-icon image list set for the control - even though you
  // haven't specified wxLIST_MASK_IMAGE when inserting.
  // So you have to set a NULL small-icon image list to be sure that
  // the wxLC_LIST mode works without icons. Of course, you may want icons...
  void SetImageList(wxImageList *imageList, int which) ;

  // Operations
  ////////////////////////////////////////////////////////////////////////////

  // Arranges the items
  bool Arrange(int flag = wxLIST_ALIGN_DEFAULT);

  // Deletes an item
  bool DeleteItem(long item);

  // Deletes all items
  bool DeleteAllItems() ;

  // Deletes a column
  bool DeleteColumn(int col);

  // Deletes all columns
  bool DeleteAllColumns();

  // Clears items, and columns if there are any.
  void ClearAll();

  // Edit the label
  wxTextCtrl* EditLabel(long item, wxClassInfo* textControlClass = CLASSINFO(wxTextCtrl));

  // End label editing, optionally cancelling the edit
  bool EndEditLabel(bool cancel);

  // Ensures this item is visible
  bool EnsureVisible(long item) ;

  // Find an item whose label matches this string, starting from the item after 'start'
  // or the beginning if 'start' is -1.
  long FindItem(long start, const wxString& str, bool partial = false);

  // Find an item whose data matches this data, starting from the item after 'start'
  // or the beginning if 'start' is -1.
  long FindItem(long start, long data);

  // Find an item nearest this position in the specified direction, starting from
  // the item after 'start' or the beginning if 'start' is -1.
  long FindItem(long start, const wxPoint& pt, int direction);

  // Determines which item (if any) is at the specified point,
  // giving details in 'flags' (see wxLIST_HITTEST_... flags above)
  // Request the subitem number as well at the given coordinate.
  long HitTest(const wxPoint& point, int& flags, long* ptrSubItem = NULL) const;

  // Inserts an item, returning the index of the new item if successful,
  // -1 otherwise.
  // TOD: Should also have some further convenience functions
  // which don't require setting a wxListItem object
  long InsertItem(wxListItem& info);

  // Insert a string item
  long InsertItem(long index, const wxString& label);

  // Insert an image item
  long InsertItem(long index, int imageIndex);

  // Insert an image/string item
  long InsertItem(long index, const wxString& label, int imageIndex);

  // For list view mode (only), inserts a column.
  long InsertColumn(long col, wxListItem& info);

  long InsertColumn(long col, const wxString& heading, int format = wxLIST_FORMAT_LEFT,
    int width = -1);

  // Scrolls the list control. If in icon, small icon or report view mode,
  // x specifies the number of pixels to scroll. If in list view mode, x
  // specifies the number of columns to scroll.
  // If in icon, small icon or list view mode, y specifies the number of pixels
  // to scroll. If in report view mode, y specifies the number of lines to scroll.
  bool ScrollList(int dx, int dy);

  // Sort items.

  // fn is a function which takes 3 long arguments: item1, item2, data.
  // item1 is the long data associated with a first item (NOT the index).
  // item2 is the long data associated with a second item (NOT the index).
  // data is the same value as passed to SortItems.
  // The return value is a negative number if the first item should precede the second
  // item, a positive number of the second item should precede the first,
  // or zero if the two items are equivalent.

  // data is arbitrary data to be passed to the sort function.
  bool SortItems(wxListCtrlCompare fn, long data);

  wxMacListControl* GetPeer() const;

    // these functions are only used for virtual list view controls, i.e. the
    // ones with wxLC_VIRTUAL style

    void SetItemCount(long count);
    void RefreshItem(long item);
    void RefreshItems(long itemFrom, long itemTo);

    // return the text for the given column of the given item
    virtual wxString OnGetItemText(long item, long column) const;

    // return the icon for the given item. In report view, OnGetItemImage will
    // only be called for the first column. See OnGetItemColumnImage for
    // details.
    virtual int OnGetItemImage(long item) const;

    // return the icon for the given item and column.
    virtual int OnGetItemColumnImage(long item, long column) const;

    // return the attribute for the item (may return NULL if none)
    virtual wxListItemAttr *OnGetItemAttr(long item) const;

/* Why should we need this function? Leave for now.
 * We might need it because item data may have changed,
 * but the display needs refreshing (in string callback mode)
  // Updates an item. If the list control has the wxLI_AUTO_ARRANGE style,
  // the items will be rearranged.
  bool Update(long item);
*/

  void Command(wxCommandEvent& event) { ProcessCommand(event); };

  wxListCtrlCompare GetCompareFunc() { return m_compareFunc; };
  long GetCompareFuncData() { return m_compareFuncData; };

  
  // public overrides needed for pimpl approach  
  virtual bool SetFont(const wxFont& font);
  virtual bool SetForegroundColour(const wxColour& colour);
  virtual bool SetBackgroundColour(const wxColour& colour);

protected:
  // protected overrides needed for pimpl approach
  virtual void DoSetSize(int x, int y,
                         int width, int height,
                         int sizeFlags = wxSIZE_AUTO);

  // common part of all ctors
  void Init();
  
  wxGenericListCtrl* m_genericImpl;   // allow use of the generic impl.
  wxMacDataBrowserListCtrlControl* m_dbImpl;
  void* /*EventHandlerRef*/   m_macListCtrlEventHandler;
  wxListCtrlCompare m_compareFunc;
  long m_compareFuncData;
  
  wxTextCtrl*       m_textCtrl;        // The control used for editing a label
  wxImageList *     m_imageListNormal; // The image list for normal icons
  wxImageList *     m_imageListSmall;  // The image list for small icons
  wxImageList *     m_imageListState;  // The image list state icons (not implemented yet)
  
  // keep track of whether or not we should delete the image list ourselves.
  bool              m_ownsImageListNormal,
                    m_ownsImageListSmall,
                    m_ownsImageListState;

  long              m_baseStyle;  // Basic Windows style flags, for recreation purposes
  wxStringList      m_stringPool; // Pool of 3 strings to satisfy Windows callback
                                  // requirements
  int               m_colCount;   // Windows doesn't have GetColumnCount so must
                                  // keep track of inserted/deleted columns

  int               m_count; // for virtual lists, store item count
};

#endif
    // _WX_LISTCTRL_H_

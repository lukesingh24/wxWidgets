/////////////////////////////////////////////////////////////////////////////
// Name:        filedlgg.h
// Purpose:     wxGenericFileDialog
// Author:      Robert Roebling
// Modified by:
// Created:     8/17/99
// Copyright:   (c) Robert Roebling
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FILEDLGG_H_
#define _WX_FILEDLGG_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "filedlgg.h"
#endif

#include "wx/dialog.h"
#include "wx/listctrl.h"
#include "wx/datetime.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxBitmapButton;
class WXDLLEXPORT wxCheckBox;
class WXDLLEXPORT wxChoice;
class WXDLLEXPORT wxFileData;
class WXDLLEXPORT wxFileCtrl;
class WXDLLEXPORT wxGenericFileDialog;
class WXDLLEXPORT wxListEvent;
class WXDLLEXPORT wxListItem;
class WXDLLEXPORT wxStaticText;
class WXDLLEXPORT wxTextCtrl;

#if defined(__WXUNIVERSAL__)||defined(__WXGTK__)||defined(__WXX11__)||defined(__WXMGL__)||defined(__WXCOCOA__)
    #define USE_GENERIC_FILEDIALOG
#endif

//-------------------------------------------------------------------------
// wxGenericFileDialog
//-------------------------------------------------------------------------

class WXDLLEXPORT wxGenericFileDialog: public wxDialog
{
public:
    wxGenericFileDialog() { }

    wxGenericFileDialog(wxWindow *parent,
                 const wxString& message = wxFileSelectorPromptStr,
                 const wxString& defaultDir = _T(""),
                 const wxString& defaultFile = _T(""),
                 const wxString& wildCard = wxFileSelectorDefaultWildcardStr,
                 long style = 0,
                 const wxPoint& pos = wxDefaultPosition);
    virtual ~wxGenericFileDialog();

    void SetMessage(const wxString& message) { SetTitle(message); }
    void SetPath(const wxString& path);
    void SetDirectory(const wxString& dir) { m_dir = dir; }
    void SetFilename(const wxString& name) { m_fileName = name; }
    void SetWildcard(const wxString& wildCard) { m_wildCard = wildCard; }
    void SetStyle(long style) { m_dialogStyle = style; }
    void SetFilterIndex(int filterIndex);

    wxString GetMessage() const { return m_message; }
    wxString GetPath() const { return m_path; }
    wxString GetDirectory() const { return m_dir; }
    wxString GetFilename() const { return m_fileName; }
    wxString GetWildcard() const { return m_wildCard; }
    long GetStyle() const { return m_dialogStyle; }
    int GetFilterIndex() const { return m_filterIndex; }

    // for multiple file selection
    void GetPaths(wxArrayString& paths) const;
    void GetFilenames(wxArrayString& files) const;

    // implementation only from now on
    // -------------------------------

    virtual int ShowModal();

    void OnSelected( wxListEvent &event );
    void OnActivated( wxListEvent &event );
    void OnList( wxCommandEvent &event );
    void OnReport( wxCommandEvent &event );
    void OnUp( wxCommandEvent &event );
    void OnHome( wxCommandEvent &event );
    void OnListOk( wxCommandEvent &event );
    void OnNew( wxCommandEvent &event );
    void OnChoiceFilter( wxCommandEvent &event );
    void OnTextEnter( wxCommandEvent &event );
    void OnTextChange( wxCommandEvent &event );
    void OnCheck( wxCommandEvent &event );

    virtual void HandleAction( const wxString &fn );

    virtual void UpdateControls();

protected:
    // use the filter with the given index
    void DoSetFilterIndex(int filterindex);

    wxString       m_message;
    long           m_dialogStyle;
    wxString       m_dir;
    wxString       m_path; // Full path
    wxString       m_fileName;
    wxString       m_wildCard;
    int            m_filterIndex;
    wxString       m_filterExtension;
    wxChoice      *m_choice;
    wxTextCtrl    *m_text;
    wxFileCtrl    *m_list;
    wxCheckBox    *m_check;
    wxStaticText  *m_static;
    wxBitmapButton *m_upDirButton;
    wxBitmapButton *m_newDirButton;

private:
    DECLARE_DYNAMIC_CLASS(wxGenericFileDialog)
    DECLARE_EVENT_TABLE()

    // these variables are preserved between wxGenericFileDialog calls
    static long ms_lastViewStyle;     // list or report?
    static bool ms_lastShowHidden;    // did we show hidden files?
};

#ifdef USE_GENERIC_FILEDIALOG

class WXDLLEXPORT wxFileDialog: public wxGenericFileDialog
{
     DECLARE_DYNAMIC_CLASS(wxFileDialog)

public:
     wxFileDialog() {}

    wxFileDialog(wxWindow *parent,
                 const wxString& message = wxFileSelectorPromptStr,
                 const wxString& defaultDir = _T(""),
                 const wxString& defaultFile = _T(""),
                 const wxString& wildCard = wxFileSelectorDefaultWildcardStr,
                 long style = 0,
                 const wxPoint& pos = wxDefaultPosition)
          :wxGenericFileDialog(parent, message, defaultDir, defaultFile, wildCard, style, pos)
     {
     }
};

#endif // USE_GENERIC_FILEDIALOG

//-----------------------------------------------------------------------------
//  wxFileData - a class to hold the file info for the wxFileCtrl
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxFileData
{
public:
    enum fileType
    {
        is_file  = 0x0000,
        is_dir   = 0x0001,
        is_link  = 0x0002,
        is_exe   = 0x0004,
        is_drive = 0x0008
    };

    // Full copy constructor
    wxFileData( const wxFileData& fileData );
    // Create a filedata from this information
    wxFileData( const wxString &filePath, const wxString &fileName,
                fileType type, int image_id );

    // (re)read the extra data about the file from the system
    void ReadData();

    // get the name of the file, dir, drive
    wxString GetFileName() const { return m_fileName; }
    // get the full path + name of the file, dir, path
    wxString GetFilePath() const { return m_filePath; }
    // Set the path + name and name of the item
    void SetNewName( const wxString &filePath, const wxString &fileName );

    // Get the size of the file in bytes
    long GetSize() const { return m_size; }
    // Get the type of file, either file extension or <DIR>, <LINK>, <DRIVE>
    wxString GetFileType() const;
    // get the last modification time
    wxDateTime GetDateTime() const { return m_dateTime; }
    // Get the time as a formatted string
    wxString GetModificationTime() const;
    // in UNIX get rwx for file, in MSW get attributes ARHS
    wxString GetPermissions() const { return m_permissions; }
    // Get the id of the image used in a wxImageList
    int GetImageId() const { return m_image; }

    bool IsFile() const  { return !IsDir() && !IsLink() && !IsDrive(); }
    bool IsDir() const   { return (m_type & is_dir  ) != 0; }
    bool IsLink() const  { return (m_type & is_link ) != 0; }
    bool IsExe() const   { return (m_type & is_exe  ) != 0; }
    bool IsDrive() const { return (m_type & is_drive) != 0; }

    // Get/Set the type of file, file/dir/drive/link
    int GetType() const { return m_type; }

    // the wxFileCtrl fields in report view
    enum fileListFieldType
    {
        FileList_Name,
        FileList_Size,
        FileList_Type,
        FileList_Time,
#if defined(__UNIX__) || defined(__WIN32__)
        FileList_Perm,
#endif // defined(__UNIX__) || defined(__WIN32__)
        FileList_Max
    };

    // Get the entry for report view of wxFileCtrl
    wxString GetEntry( fileListFieldType num ) const;

    // Get a string representation of the file info
    wxString GetHint() const;
    // initialize a wxListItem attributes
    void MakeItem( wxListItem &item );

private:
    wxString m_fileName;
    wxString   m_filePath;
    long     m_size;
    wxDateTime m_dateTime;
    wxString m_permissions;
    int      m_type;
    int        m_image;
};

//-----------------------------------------------------------------------------
//  wxFileCtrl
//-----------------------------------------------------------------------------

class WXDLLEXPORT wxFileCtrl : public wxListCtrl
{
public:
    wxFileCtrl();
    wxFileCtrl( wxWindow *win,
                wxWindowID id,
                const wxString &wild,
                bool showHidden,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = wxLC_LIST,
                const wxValidator &validator = wxDefaultValidator,
                const wxString &name = wxT("filelist") );
    virtual ~wxFileCtrl();

    virtual void ChangeToListMode();
    virtual void ChangeToReportMode();
    virtual void ChangeToSmallIconMode();
    virtual void ShowHidden( bool show = TRUE );
    bool GetShowHidden() const { return m_showHidden; }

    virtual long Add( wxFileData *fd, wxListItem &item );
    virtual void UpdateItem(const wxListItem &item);
    virtual void UpdateFiles();
    virtual void MakeDir();
    virtual void GoToParentDir();
    virtual void GoToHomeDir();
    virtual void GoToDir( const wxString &dir );
    virtual void SetWild( const wxString &wild );
    wxString GetWild() const { return m_wild; }
    wxString GetDir() const { return m_dirName; }

    void OnListDeleteItem( wxListEvent &event );
    void OnListEndLabelEdit( wxListEvent &event );
    void OnListColClick( wxListEvent &event );

    virtual void SortItems(wxFileData::fileListFieldType field, bool foward);
    bool GetSortDirection() const { return m_sort_foward; }
    wxFileData::fileListFieldType GetSortField() const { return m_sort_field; }

protected:
    void FreeItemData(const wxListItem& item);
    void FreeAllItemsData();

    wxString      m_dirName;
    bool          m_showHidden;
    wxString      m_wild;

    bool m_sort_foward;
    wxFileData::fileListFieldType m_sort_field;

private:
    DECLARE_DYNAMIC_CLASS(wxFileCtrl);
    DECLARE_EVENT_TABLE()
};

#endif // _WX_FILEDLGG_H_


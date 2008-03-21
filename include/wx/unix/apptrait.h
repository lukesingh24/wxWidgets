///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/apptrait.h
// Purpose:     standard implementations of wxAppTraits for Unix
// Author:      Vadim Zeitlin
// Modified by:
// Created:     23.06.2003
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_APPTRAIT_H_
#define _WX_UNIX_APPTRAIT_H_

// ----------------------------------------------------------------------------
// wxGUI/ConsoleAppTraits: must derive from wxAppTraits, not wxAppTraitsBase
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxConsoleAppTraits : public wxConsoleAppTraitsBase
{
public:
#if wxUSE_CONSOLE_EVENTLOOP
    virtual wxEventLoopBase *CreateEventLoop();
#endif // wxUSE_CONSOLE_EVENTLOOP
    virtual int WaitForChild(wxExecuteData& execData);
#if wxUSE_TIMER
    virtual wxTimerImpl *CreateTimerImpl(wxTimer *timer);
#endif
};

#if wxUSE_GUI

class WXDLLEXPORT wxGUIAppTraits : public wxGUIAppTraitsBase
{
public:
    virtual wxEventLoopBase *CreateEventLoop();
    virtual int WaitForChild(wxExecuteData& execData);
#if wxUSE_TIMER
    virtual wxTimerImpl *CreateTimerImpl(wxTimer *timer);
#endif
#if wxUSE_THREADS && defined(__WXGTK20__)
    virtual void MutexGuiEnter();
    virtual void MutexGuiLeave();
#endif

#if (defined(__WXMAC__) || defined(__WXCOCOA__)) && wxUSE_STDPATHS
    virtual wxStandardPathsBase& GetStandardPaths();
#endif
    virtual wxPortId GetToolkitVersion(int *majVer = NULL, int *minVer = NULL) const;

#if defined(__WXGTK__) && wxUSE_INTL
    virtual void SetLocale();
#endif // __WXGTK__

#ifdef __WXGTK20__
    virtual wxString GetDesktopEnvironment() const;
    virtual wxString GetStandardCmdLineOptions(wxArrayString& names,
                                               wxArrayString& desc) const;
#endif // __WXGTK20____

#if defined(__WXDEBUG__) && defined(__WXGTK20__)
    virtual bool ShowAssertDialog(const wxString& msg);
#endif

    // GTK+ and Motif integrate sockets directly in their main loop, the other
    // Unix ports do it at wxEventLoop level
    //
    // TODO: Should we use XtAddInput() for wxX11 too? Or, vice versa, if there
    //       is no advantage in doing this compared to the generic way
    //       currently used by wxX11, should we continue to use GTK/Motif-
    //       specific stuff?
#if wxUSE_SOCKETS && (defined(__WXGTK__) || defined(__WXMOTIF__))
    virtual GSocketManager *GetSocketManager();
#endif
};

#endif // wxUSE_GUI

#endif // _WX_UNIX_APPTRAIT_H_


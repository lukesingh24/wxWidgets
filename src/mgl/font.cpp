/////////////////////////////////////////////////////////////////////////////
// Name:        font.cpp
// Author:      Vaclav Slavik
// Id:          $Id$
// Copyright:   (c) 2001 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "font.h"
#endif

#include "wx/font.h"
#include "wx/fontutil.h"
#include "wx/cmndata.h"
#include "wx/utils.h"
#include "wx/log.h"
#include "wx/gdicmn.h"
#include "wx/tokenzr.h"
#include "wx/settings.h"

#include <strings.h>

// ----------------------------------------------------------------------------
// wxFontRefData
// ----------------------------------------------------------------------------

class wxFontRefData : public wxObjectRefData
{
public:
    wxFontRefData(int size = wxDEFAULT,
                  int family = wxDEFAULT,
                  int style = wxDEFAULT,
                  int weight = wxDEFAULT,
                  bool underlined = FALSE,
                  const wxString& faceName = wxEmptyString,
                  wxFontEncoding encoding = wxFONTENCODING_DEFAULT);
    wxFontRefData(const wxFontRefData& data);
    virtual ~wxFontRefData();

protected:
    // common part of all ctors
    void Init(int pointSize,
              int family,
              int style,
              int weight,
              bool underlined,
              const wxString& faceName,
              wxFontEncoding encoding);

private:
    int              m_pointSize;
    int              m_family,
                     m_style,
                     m_weight;
    bool             m_underlined;
    wxString         m_faceName;
    wxFontEncoding   m_encoding;

    wxMGLFontLibrary *m_library;
    bool              m_valid;

    friend class wxFont;
};

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxFontRefData
// ----------------------------------------------------------------------------

void wxFontRefData::Init(int pointSize,
                         int family,
                         int style,
                         int weight,
                         bool underlined,
                         const wxString& faceName,
                         wxFontEncoding encoding)
{
    if ( family == wxDEFAULT )
        m_family = wxSWISS;
    else
        m_family = family;

    m_faceName = faceName;

    if ( style == wxDEFAULT )
        m_style = wxNORMAL;
    else
        m_style = style;

    if ( weight == wxDEFAULT )
        m_weight = wxNORMAL;
    else
        m_weight = weight;

    if ( pointSize == wxDEFAULT )
        m_pointSize = 12;
    else
        m_pointSize = pointSize;

    m_underlined = underlined;
    m_encoding = encoding;

    m_library = NULL;
    m_valid = FALSE;
}

wxFontRefData::wxFontRefData(const wxFontRefData& data)
{
    Init(data.m_pointSize, data.m_family, data.m_style, data.m_weight,
         data.m_underlined, data.m_faceName, data.m_encoding);

    m_library = data.m_library;
    m_valid = data.m_valid;
    if ( m_library )
        m_library->IncRef();
}

wxFontRefData::wxFontRefData(int size, int family, int style,
                             int weight, bool underlined,
                             const wxString& faceName,
                             wxFontEncoding encoding)
{
    Init(size, family, style, weight, underlined, faceName, encoding);
}

wxFontRefData::~wxFontRefData()
{
    if ( m_library )
        m_library->DecRef();
}

// ----------------------------------------------------------------------------
// wxFont
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxFont, wxGDIObject)

void wxFont::Init()
{
    if (wxTheFontList)
        wxTheFontList->Append(this);
}

bool wxFont::Create(const wxNativeFontInfo& info)
{
    return Create(info.pointSize, info.family, info.style, info.weight,
                  info.underlined, info.faceName, info.encoding);
}

bool wxFont::Create(int pointSize,
                    int family,
                    int style,
                    int weight,
                    bool underlined,
                    const wxString& face,
                    wxFontEncoding encoding)
{
    m_refData = new wxFontRefData(pointSize, family, style, weight,
                                  underlined, face, encoding);
    return TRUE;
}

struct font_t *wxFont::GetMGLfont_t(float scale, bool antialiased)
{
    if ( !M_FONTDATA->m_valid )
    {
        wxMGLFontLibrary *old = M_FONTDATA->m_library;
        M_FONTDATA->m_library = wxTheFontsManager->GetFontLibrary(this);
        M_FONTDATA->m_library->IncRef();
        if ( old )
            old->DecRef();
    }

    wxMGLFontInstance *instance = 
        M_FONTDATA->m_library->GetFontInstance(this, scale, antialiased);

    return instance->GetMGLfont_t();
}

void wxFont::Unshare()
{
    if ( !m_refData )
    {
        m_refData = new wxFontRefData();
    }
    else
    {
        wxFontRefData* ref = new wxFontRefData(*(wxFontRefData*)m_refData);
        UnRef();
        m_refData = ref;
    }
}

wxFont::~wxFont()
{
    if (wxTheFontList)
        wxTheFontList->DeleteObject(this);
}

// ----------------------------------------------------------------------------
// accessors
// ----------------------------------------------------------------------------

int wxFont::GetPointSize() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid font") );

    return M_FONTDATA->m_pointSize;
}

wxString wxFont::GetFaceName() const
{
    wxCHECK_MSG( Ok(), wxT(""), wxT("invalid font") );

    return M_FONTDATA->m_faceName;
}

int wxFont::GetFamily() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid font") );

    return M_FONTDATA->m_family;
}

int wxFont::GetStyle() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid font") );

    return M_FONTDATA->m_style;
}

int wxFont::GetWeight() const
{
    wxCHECK_MSG( Ok(), 0, wxT("invalid font") );

    return M_FONTDATA->m_weight;
}

bool wxFont::GetUnderlined() const
{
    wxCHECK_MSG( Ok(), FALSE, wxT("invalid font") );

    return M_FONTDATA->m_underlined;
}


wxFontEncoding wxFont::GetEncoding() const
{
    wxCHECK_MSG( Ok(), wxFONTENCODING_DEFAULT, wxT("invalid font") );

    return M_FONTDATA->m_encoding;
}


// ----------------------------------------------------------------------------
// change font attributes
// ----------------------------------------------------------------------------

void wxFont::SetPointSize(int pointSize)
{
    Unshare();

    M_FONTDATA->m_pointSize = pointSize;
    M_FONTDATA->m_valid = FALSE;
}

void wxFont::SetFamily(int family)
{
    Unshare();

    M_FONTDATA->m_family = family;
    M_FONTDATA->m_valid = FALSE;
}

void wxFont::SetStyle(int style)
{
    Unshare();

    M_FONTDATA->m_style = style;
    M_FONTDATA->m_valid = FALSE;
}

void wxFont::SetWeight(int weight)
{
    Unshare();

    M_FONTDATA->m_weight = weight;
    M_FONTDATA->m_valid = FALSE;
}

void wxFont::SetFaceName(const wxString& faceName)
{
    Unshare();

    M_FONTDATA->m_faceName = faceName;
    M_FONTDATA->m_valid = FALSE;
}

void wxFont::SetUnderlined(bool underlined)
{
    Unshare();

    M_FONTDATA->m_underlined = underlined;
}

void wxFont::SetEncoding(wxFontEncoding encoding)
{
    Unshare();

    M_FONTDATA->m_encoding = encoding;
    M_FONTDATA->m_valid = FALSE;
}

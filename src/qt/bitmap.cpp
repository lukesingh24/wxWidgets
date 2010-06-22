/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/bitmap.cpp
// Author:      Peter Most, Javier Torres
// Id:          $Id$
// Copyright:   (c) Peter Most, Javier Torres
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/bitmap.h"
#include "wx/image.h"
#include "wx/qt/converter.h"
#include "wx/qt/utils.h"

#include <QtGui/QPixmap>
#include <QtGui/QBitmap>
#include <QtGui/QLabel>

static wxImage ConvertImage( QImage qtImage )
{
    bool hasAlpha = qtImage.hasAlphaChannel();
    
    int numPixels = qtImage.height() * qtImage.width();

    //Convert to ARGB32 for scanLine
    qtImage = qtImage.convertToFormat(QImage::Format_ARGB32);
    
    unsigned char *data = (unsigned char *)malloc(sizeof(char) * 3 * numPixels);
    unsigned char *startData = data;
    
    unsigned char *alpha = NULL;
    if (hasAlpha)
        alpha = (unsigned char *)malloc(sizeof(char) * numPixels);

    unsigned char *startAlpha = alpha;
    
    for (int y = 0; y < qtImage.height(); y++)
    {
        QRgb *line = (QRgb*)qtImage.scanLine(y);
        
        for (int x = 0; x < qtImage.width(); x++)
        {
            QRgb colour = line[x];
            
            data[0] = qRed(colour);
            data[1] = qGreen(colour);
            data[2] = qBlue(colour);
            
            if (hasAlpha)
            {
                alpha[0] = qAlpha(colour);
                alpha++;
            }
            data += 3;
        }
    }
    if (hasAlpha)
        return wxImage(wxQtConvertSize(qtImage.size()), startData, startAlpha);
    else
        return wxImage(wxQtConvertSize(qtImage.size()), startData);
}

static QImage ConvertImage( const wxImage &image )
{
    bool hasAlpha = image.HasAlpha();
    QImage qtImage(wxQtConvertSize(image.GetSize()), (hasAlpha ? QImage::Format_ARGB32 : QImage::Format_RGB32));
    
    unsigned char *data = image.GetData();
    unsigned char *alpha = hasAlpha ? image.GetAlpha() : NULL;
    QRgb colour;
    
    for (int y = 0; y < image.GetHeight(); y++)
    {
        for (int x = 0; x < image.GetWidth(); x++)
        {
            if (hasAlpha)
            {
                colour = alpha[0] << 24;
                alpha++;
            }
            else
                colour = 0;
            
            colour += (data[0] << 16) + (data[1] << 8) + data[2];
            qtImage.setPixel(x, y, colour);
            
            data += 3;
        }
    }
    return qtImage;
}

//-----------------------------------------------------------------------------
// wxBitmapRefData
//-----------------------------------------------------------------------------

class wxBitmapRefData: public wxGDIRefData
{
    public:
        wxBitmapRefData() : wxGDIRefData()
        {
            m_mask = NULL;
            m_qtPixmap = new QPixmap();
        }
        
        wxBitmapRefData( const wxBitmapRefData& data ) : wxGDIRefData()
        {
            m_mask = NULL;
            m_qtPixmap = data.m_qtPixmap;
        }
        
        wxBitmapRefData( int width, int height, int depth ) : wxGDIRefData()
        {
            m_mask = NULL;
            if (depth == 1) {
                m_qtPixmap = new QBitmap( width, height );
            } else {
                m_qtPixmap = new QPixmap( width, height );
            }
        }
        
        wxBitmapRefData( QPixmap pix ) : wxGDIRefData()
        {
            m_mask = NULL;
            m_qtPixmap = new QPixmap(pix);
        }

        QPixmap *m_qtPixmap;
        wxMask *m_mask;
};

//-----------------------------------------------------------------------------
// wxBitmap
//-----------------------------------------------------------------------------

#define M_PIXDATA (*((wxBitmapRefData *)m_refData)->m_qtPixmap)

void wxBitmap::InitStandardHandlers()
{
}

wxBitmap::wxBitmap()
{
}

wxBitmap::wxBitmap(QPixmap pix)
{
    m_refData = new wxBitmapRefData(pix);
}

wxBitmap::wxBitmap(const wxBitmap& bmp)
{
    Ref(bmp);
}

wxBitmap::wxBitmap(const char bits[], int width, int height, int depth )
{
    if (depth == 1) {
        m_refData = new wxBitmapRefData();
        ((wxBitmapRefData *)m_refData)->m_qtPixmap = new QBitmap(QBitmap::fromData(QSize(width, height), (const uchar*)bits));
    } else {
        wxMISSING_IMPLEMENTATION("wxBitmap(bits, width, height, depth constructor) for depth != 1");
        m_refData = new wxBitmapRefData();
    }
}

wxBitmap::wxBitmap(int width, int height, int depth)
{
    Create(width, height, depth);
}

wxBitmap::wxBitmap(const wxSize& sz, int depth )
{
    Create(sz, depth);
}

// wxBitmap::wxBitmap(const char* const* bits)
// {
// }

wxBitmap::wxBitmap(const wxString &filename, wxBitmapType type )
{
    LoadFile(filename, type);
}

wxBitmap::wxBitmap(const wxImage& image, int depth )
{
    Qt::ImageConversionFlags flags = 0;
    if (depth == 1)
        flags = Qt::MonoOnly;
    m_refData = new wxBitmapRefData(QPixmap::fromImage(ConvertImage(image), flags));
}


bool wxBitmap::Create(int width, int height, int depth )
{
    UnRef();
    m_refData = new wxBitmapRefData(width, height, depth);
    
    return true;
}

bool wxBitmap::Create(const wxSize& sz, int depth )
{
    return Create(sz.GetWidth(), sz.GetHeight(), depth);
}


int wxBitmap::GetHeight() const
{
    return M_PIXDATA.height();
}

int wxBitmap::GetWidth() const
{
    return M_PIXDATA.width();
}

int wxBitmap::GetDepth() const
{
    return M_PIXDATA.depth();
}


#if wxUSE_IMAGE
wxImage wxBitmap::ConvertToImage() const
{
    return ConvertImage(M_PIXDATA.toImage());
}

#endif // wxUSE_IMAGE

wxMask *wxBitmap::GetMask() const
{
    return ( ((wxBitmapRefData *)m_refData)->m_mask );
}

void wxBitmap::SetMask(wxMask *mask)
{
    wxMask *bitmapMask = ( ((wxBitmapRefData *)m_refData)->m_mask );

    if (bitmapMask)
        delete bitmapMask;

    bitmapMask = mask;
    M_PIXDATA.setMask( *mask->GetHandle() );
}


wxBitmap wxBitmap::GetSubBitmap(const wxRect& rect) const
{
    return wxBitmap(M_PIXDATA.copy(wxQtConvertRect(rect)));
}


bool wxBitmap::SaveFile(const wxString &name, wxBitmapType type,
              const wxPalette *palette ) const
{   
    #if wxUSE_IMAGE
    //Try to save using wx
    wxImage image = ConvertToImage();
    if (image.IsOk() && image.SaveFile(name, type))
        return true;
    #endif
    
    //Try to save using Qt
    const char* type_name = NULL;
    switch (type)
    {
        case wxBITMAP_TYPE_BMP:  type_name = "bmp";  break;
        case wxBITMAP_TYPE_ICO:  type_name = "ico";  break;
        case wxBITMAP_TYPE_JPEG: type_name = "jpeg"; break;
        case wxBITMAP_TYPE_PNG:  type_name = "png";  break;
        default: break;
    }
    return type_name &&
        M_PIXDATA.save(wxQtConvertString(name), type_name);
}

bool wxBitmap::LoadFile(const wxString &name, wxBitmapType type)
{
#if wxUSE_IMAGE
    //Try to load using wx
    wxImage image;
    if (image.LoadFile(name, type) && image.IsOk())
    {
        *this = wxBitmap(image);
        return true;
    }
    else
#endif
    {
        //Try to load using Qt
        AllocExclusive();
        
        //TODO: Use passed image type instead of auto-detection
        return M_PIXDATA.load(wxQtConvertString(name));
    }
}


#if wxUSE_PALETTE
wxPalette *wxBitmap::GetPalette() const
{
    wxMISSING_IMPLEMENTATION( "wxBitmap palettes" );
    return 0;
}

void wxBitmap::SetPalette(const wxPalette& palette)
{
    wxMISSING_IMPLEMENTATION( "wxBitmap palettes" );
}

#endif // wxUSE_PALETTE

// copies the contents and mask of the given (colour) icon to the bitmap
bool wxBitmap::CopyFromIcon(const wxIcon& icon)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );
    return 0;
}


// implementation:
void wxBitmap::SetHeight(int height)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );
}

void wxBitmap::SetWidth(int width)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );
}

void wxBitmap::SetDepth(int depth)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );
}


void *wxBitmap::GetRawData(wxPixelDataBase& data, int bpp)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );
    return 0;
}

void wxBitmap::UngetRawData(wxPixelDataBase& data)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );
}

QPixmap *wxBitmap::GetHandle() const
{
    return ((wxBitmapRefData *)m_refData)->m_qtPixmap;
}

wxGDIRefData *wxBitmap::CreateGDIRefData() const
{
    return new wxBitmapRefData;
}

wxGDIRefData *wxBitmap::CloneGDIRefData(const wxGDIRefData *data) const
{
    return new wxBitmapRefData(*(wxBitmapRefData *)data);
}

//-----------------------------------------------------------------------------
// wxMask
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS( wxMask, wxObject )

wxMask::wxMask()
{
    m_qtBitmap = NULL;
}

wxMask::wxMask(const wxMask &mask)
{
    m_qtBitmap = new QBitmap(*mask.GetHandle());
}

wxMask::wxMask(const wxBitmap& bitmap, const wxColour& colour)
{
    m_qtBitmap = NULL;
    Create(bitmap, colour);
}

wxMask::wxMask(const wxBitmap& bitmap, int paletteIndex)
{
    m_qtBitmap = NULL;
    Create(bitmap, paletteIndex);
}

wxMask::wxMask(const wxBitmap& bitmap)
{
    m_qtBitmap = NULL;
    Create(bitmap);
}

wxMask::~wxMask()
{
    if (m_qtBitmap)
        delete m_qtBitmap;
}

bool wxMask::Create(const wxBitmap& bitmap, const wxColour& colour)
{
    if (!bitmap.IsOk())
        return false;

    if (m_qtBitmap)
        delete m_qtBitmap;

    m_qtBitmap = new QBitmap(bitmap.GetHandle()->createMaskFromColor(colour.GetHandle()));
    return true;
}

bool wxMask::Create(const wxBitmap& bitmap, int paletteIndex)
{
    wxMISSING_IMPLEMENTATION( __FUNCTION__ );
    return false;
}

bool wxMask::Create(const wxBitmap& bitmap)
{
    //Only for mono bitmaps
    if (!bitmap.IsOk() || bitmap.GetDepth() != 1)
        return false;

    if (m_qtBitmap)
        delete m_qtBitmap;

    m_qtBitmap = new QBitmap(*bitmap.GetHandle());
    return true;
}

QBitmap *wxMask::GetHandle() const
{
    return m_qtBitmap;
}

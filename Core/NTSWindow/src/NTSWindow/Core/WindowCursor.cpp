//
// Created by TEUGUIA TADJUIDJE Rodolf Séderis on 4/12/2024 at 3:07:50 PM.
// Copyright (c) 2024 Rihen. All rights reserved.
//

#include "pch/ntspch.h"
#include "WindowCursor.h"
#include <NTSWindow/Core/NkentseuLogger.h>

#ifdef NKENTSEU_PLATFORM_WINDOWS
#include <Windows.h>
#elif defined(NKENTSEU_PLATFORM_LINUX)
#include <X11/Xcursor/Xcursor.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h> // xcursorint.h

#include <vector>

#include <cassert>
#include <cstdlib>
#include <mutex>
#endif

namespace nkentseu {
    using namespace maths;
#if defined(NKENTSEU_PLATFORM_LINUX)
std::recursive_mutex mutex;
Display* sharedDisplay = nullptr;
unsigned int         referenceCount = 0;

Display* OpenDisplay()
{
    const std::lock_guard lock(mutex);

    if (referenceCount == 0)
    {
        sharedDisplay = XOpenDisplay(nullptr);

        // Opening display failed: The best we can do at the moment is to output a meaningful error message
        // and cause an abnormal program termination
        if (!sharedDisplay)
        {
            Log_nts.Error("Failed to open X11 display; make sure the DISPLAY environment variable is set correctly");
        }
}

    ++referenceCount;
    return sharedDisplay;
    return 0;
}


////////////////////////////////////////////////////////////
void CloseDisplay(Display* display)
{
    const std::lock_guard lock(mutex);

    assert(display == sharedDisplay && "Display must match shared display");

    --referenceCount;
    if (referenceCount == 0)
        XCloseDisplay(display);
}
#endif


    WindowCursor::~WindowCursor() {
#if defined(NKENTSEU_PLATFORM_LINUX)
        CloseDisplay(m_Display);
#endif
    }

    bool WindowCursor::LoadFromPixels(const uint8* pixels, Vector2u size, Vector2u hotspot) {
        if ((pixels == nullptr) || (size.x == 0) || (size.y == 0)) {
            return false;
        }
        return LoadFromPixelsInternal(pixels, size, hotspot);
    }

    bool WindowCursor::LoadFromSystem(WindowCursor::Code type) {
        return LoadFromSystemInternal(type);
    }

#ifdef NKENTSEU_PLATFORM_WINDOWS
    // cursor
    bool WindowCursor::LoadFromPixelsInternal(const uint8* pixels, Vector2u size, Vector2u hotspot) {// Create the bitmap that will hold our color data
        auto bitmapHeader = BITMAPV5HEADER();
        bitmapHeader.bV5Size = sizeof(BITMAPV5HEADER);
        bitmapHeader.bV5Width = static_cast<LONG>(size.x);
        bitmapHeader.bV5Height = -static_cast<LONG>(size.y); // Negative indicates origin is in upper-left corner
        bitmapHeader.bV5Planes = 1;
        bitmapHeader.bV5BitCount = 32;
        bitmapHeader.bV5Compression = BI_BITFIELDS;
        bitmapHeader.bV5RedMask = 0x00ff0000;
        bitmapHeader.bV5GreenMask = 0x0000ff00;
        bitmapHeader.bV5BlueMask = 0x000000ff;
        bitmapHeader.bV5AlphaMask = 0xff000000;

        uint32* bitmapData = nullptr;

        HDC     screenDC = GetDC(nullptr);
        HBITMAP color = CreateDIBSection(screenDC,
            reinterpret_cast<const BITMAPINFO*>(&bitmapHeader),
            DIB_RGB_COLORS,
            reinterpret_cast<void**>(&bitmapData),
            nullptr,
            0);
        ReleaseDC(nullptr, screenDC);

        if (!color) {
            Log_nts.Error("Failed to create cursor color bitmap");
            return false;
        }

        // Fill our bitmap with the cursor color data
        // We'll have to swap the red and blue channels here
        uint32* bitmapOffset = bitmapData;
        for (uint32 remaining = size.x * size.y; remaining > 0; --remaining, pixels += 4) {
            *bitmapOffset++ = static_cast<uint32>((pixels[3] << 24) | (pixels[0] << 16) | (pixels[1] << 8) | pixels[2]);
        }

        // Create a dummy mask bitmap (it won't be used)
        HBITMAP mask = CreateBitmap(static_cast<int>(size.x), static_cast<int>(size.y), 1, 1, nullptr);

        if (!mask) {
            DeleteObject(color);
            Log_nts.Error("Failed to create cursor mask bitmap");
            return false;
        }

        // Create the structure that describes our cursor
        auto cursorInfo = ICONINFO();
        cursorInfo.fIcon = FALSE; // This is a cursor and not an icon
        cursorInfo.xHotspot = hotspot.x;
        cursorInfo.yHotspot = hotspot.y;
        cursorInfo.hbmColor = color;
        cursorInfo.hbmMask = mask;

        // Create the cursor
        m_Cursor = reinterpret_cast<HCURSOR>(CreateIconIndirect(&cursorInfo));
        m_SystemCursor = false;

        // The data has been copied into the cursor, so get rid of these
        DeleteObject(color);
        DeleteObject(mask);

        if (m_Cursor) {
            return true;
        }
        else {
            Log_nts.Error("Failed to create cursor from bitmap");
            return false;
        }
    }

    bool WindowCursor::LoadFromSystemInternal(WindowCursor::Code type) {
        Release();

        LPCTSTR shape = nullptr;

        // clang-format off
        switch (type) {
        case WindowCursor::Arrow:                  shape = IDC_ARROW;       break;
        case WindowCursor::ArrowWait:              shape = IDC_APPSTARTING; break;
        case WindowCursor::Wait:                   shape = IDC_WAIT;        break;
        case WindowCursor::Text:                   shape = IDC_IBEAM;       break;
        case WindowCursor::Hand:                   shape = IDC_HAND;        break;
        case WindowCursor::SizeHorizontal:         shape = IDC_SIZEWE;      break;
        case WindowCursor::SizeVertical:           shape = IDC_SIZENS;      break;
        case WindowCursor::SizeTopLeftBottomRight: shape = IDC_SIZENWSE;    break;
        case WindowCursor::SizeBottomLeftTopRight: shape = IDC_SIZENESW;    break;
        case WindowCursor::SizeLeft:               shape = IDC_SIZEWE;      break;
        case WindowCursor::SizeRight:              shape = IDC_SIZEWE;      break;
        case WindowCursor::SizeTop:                shape = IDC_SIZENS;      break;
        case WindowCursor::SizeBottom:             shape = IDC_SIZENS;      break;
        case WindowCursor::SizeTopLeft:            shape = IDC_SIZENWSE;    break;
        case WindowCursor::SizeBottomRight:        shape = IDC_SIZENWSE;    break;
        case WindowCursor::SizeBottomLeft:         shape = IDC_SIZENESW;    break;
        case WindowCursor::SizeTopRight:           shape = IDC_SIZENESW;    break;
        case WindowCursor::SizeAll:                shape = IDC_SIZEALL;     break;
        case WindowCursor::Cross:                  shape = IDC_CROSS;       break;
        case WindowCursor::Help:                   shape = IDC_HELP;        break;
        case WindowCursor::NotAllowed:             shape = IDC_NO;          break;
        }
        // clang-format on

        // Get the shared system cursor and make sure not to destroy it
        m_Cursor = LoadCursor(nullptr, shape);
        m_SystemCursor = true;

        if (m_Cursor) {
            return true;
        }
        else {
            Log_nts.Error("Could not create copy of a system cursor");
            return false;
        }
    }

    void WindowCursor::Release() {
        if (m_Cursor && !m_SystemCursor) {
            DestroyCursor(static_cast<HCURSOR>(m_Cursor));
            m_Cursor = nullptr;
        }
    }
#elif defined(NKENTSEU_PLATFORM_LINUX)
    template <typename T>
    struct XDeleter
    {
        void operator()(T* data) const
        {
            XFree(data);
        }
    };

    template <typename T> 
    using X11Ptr = std::unique_ptr<T, XDeleter<std::remove_all_extents_t<T>>>;

    template <>
    struct XDeleter<XcursorImage>
    {
        void operator()(XcursorImage* cursorImage) const
        {
            XcursorImageDestroy(cursorImage);
        }
    };

    WindowCursor::WindowCursor() : m_Display(OpenDisplay()) {
    }

    // cursor
    bool WindowCursor::LoadFromPixelsInternal(const uint8* pixels, nkentseu::Vector2u size, nkentseu::Vector2u hotspot) {
        Release();

        if (IsColorCursorSupported())
            return LoadFromPixelsARGB(pixels, size, hotspot);
        else
            return LoadFromPixelsMonochrome(pixels, size, hotspot);
            return true;
    }

    bool WindowCursor::LoadFromPixelsARGB(const std::uint8_t* pixels, Vector2u size, Vector2u hotspot) {
        // Create cursor image, convert from RGBA to ARGB.
        auto cursorImage = X11Ptr<XcursorImage>(XcursorImageCreate(static_cast<int>(size.x), static_cast<int>(size.y)));
        cursorImage->xhot = hotspot.x;
        cursorImage->yhot = hotspot.y;

        const std::size_t numPixels = static_cast<std::size_t>(size.x) * static_cast<std::size_t>(size.y);
        for (std::size_t pixelIndex = 0; pixelIndex < numPixels; ++pixelIndex)
        {
            cursorImage->pixels[pixelIndex] = static_cast<std::uint32_t>(
                pixels[pixelIndex * 4 + 2] + (pixels[pixelIndex * 4 + 1] << 8) + (pixels[pixelIndex * 4 + 0] << 16) +
                (pixels[pixelIndex * 4 + 3] << 24));
        }

        // Create the cursor.
        m_Cursor = XcursorImageLoadCursor(m_Display, cursorImage.get());

        // We assume everything went fine...
        return true;
    }

    bool WindowCursor::LoadFromPixelsMonochrome(const std::uint8_t* pixels, Vector2u size, Vector2u hotspot) {
        const std::size_t         packedWidth = (size.x + 7) / 8;
        const std::size_t         bytes = packedWidth * size.y;
        std::vector<std::uint8_t> mask(bytes, 0); // Defines which pixel is opaque (1) or transparent (0).
        std::vector<std::uint8_t> data(bytes, 0); // Defines which pixel is white (1) or black (0).

        for (std::size_t j = 0; j < size.y; ++j)
        {
            for (std::size_t i = 0; i < size.x; ++i)
            {
                const std::size_t pixelIndex = i + j * size.x;
                const std::size_t byteIndex = i / 8 + j * packedWidth;
                const std::size_t bitIndex = i % 8;

                // Turn on pixel that are not transparent
                const std::uint8_t opacity = pixels[pixelIndex * 4 + 3] > 0 ? 1 : 0;
                mask[byteIndex] |= static_cast<std::uint8_t>(opacity << bitIndex);

                // Choose between black/background & white/foreground color for each pixel,
                // based on the pixel color intensity: on average, if a channel is "active"
                // at 50%, the bit is white.
                const int intensity = (pixels[pixelIndex * 4 + 0] + pixels[pixelIndex * 4 + 1] + pixels[pixelIndex * 4 + 2]) / 3;
                const std::uint8_t bit = intensity > 128 ? 1 : 0;
                data[byteIndex] |= static_cast<std::uint8_t>(bit << bitIndex);
            }
        }

        const Pixmap maskPixmap = XCreateBitmapFromData(m_Display,
            XDefaultRootWindow(m_Display),
            reinterpret_cast<char*>(mask.data()),
            size.x,
            size.y);
        const Pixmap dataPixmap = XCreateBitmapFromData(m_Display,
            XDefaultRootWindow(m_Display),
            reinterpret_cast<char*>(data.data()),
            size.x,
            size.y);

        // Define the foreground color as white and the background as black.
        XColor fg;
        XColor bg;
        fg.red = 0xFFFF;
        fg.blue = 0xFFFF;
        fg.green = 0xFFFF;
        bg.red = 0x0000;
        bg.blue = 0x0000;
        bg.green = 0x0000;

        // Create the monochrome cursor.
        m_Cursor = XCreatePixmapCursor(m_Display, dataPixmap, maskPixmap, &fg, &bg, hotspot.x, hotspot.y);

        // Free the resources
        XFreePixmap(m_Display, dataPixmap);
        XFreePixmap(m_Display, maskPixmap);
        return true;
    }

    bool WindowCursor::LoadFromSystemInternal(nkentseu::WindowCursor::Code type) {
        Release();

        uint32 shape;

        // clang-format off
        switch (type)
        {
        default: return false;

        case WindowCursor::Arrow:           shape = XC_arrow;               break;
        case WindowCursor::Wait:            shape = XC_watch;               break;
        case WindowCursor::Text:            shape = XC_xterm;               break;
        case WindowCursor::Hand:            shape = XC_hand2;               break;
        case WindowCursor::SizeHorizontal:  shape = XC_sb_h_double_arrow;   break;
        case WindowCursor::SizeVertical:    shape = XC_sb_v_double_arrow;   break;
        case WindowCursor::SizeLeft:        shape = XC_left_side;           break;
        case WindowCursor::SizeRight:       shape = XC_right_side;          break;
        case WindowCursor::SizeTop:         shape = XC_top_side;            break;
        case WindowCursor::SizeBottom:      shape = XC_bottom_side;         break;
        case WindowCursor::SizeTopLeft:     shape = XC_top_left_corner;     break;
        case WindowCursor::SizeBottomRight: shape = XC_bottom_right_corner; break;
        case WindowCursor::SizeBottomLeft:  shape = XC_bottom_left_corner;  break;
        case WindowCursor::SizeTopRight:    shape = XC_top_right_corner;    break;
        case WindowCursor::SizeAll:         shape = XC_fleur;               break;
        case WindowCursor::Cross:           shape = XC_crosshair;           break;
        case WindowCursor::Help:            shape = XC_question_arrow;      break;
        case WindowCursor::NotAllowed:      shape = XC_X_cursor;            break;
        }
        // clang-format on

        m_Cursor = XCreateFontCursor(m_Display, shape);
        return true;
    }

    bool WindowCursor::IsColorCursorSupported()
    {
       return XcursorSupportsARGB(m_Display);
    }

    void WindowCursor::Release() {
        if (m_Cursor != None)
        {
            XFreeCursor(m_Display, m_Cursor);
            m_Cursor = None;
        }
    }

#endif
}    // namespace nkentseu
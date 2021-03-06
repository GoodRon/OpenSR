/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2014 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OpenSR/FTFont.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>

#include <OpenSR/libRanger.h>

#include "OpenSR/ResourceManager.h"
#include "OpenSR/Texture.h"

namespace Rangers
{
namespace
{
uint32_t blendARGB(uint32_t dst, uint32_t src)
{
    uint8_t r, g, b, a;

    uint8_t sa = (src >> 24) & 0xFF;
    uint8_t sr = (src >> 16) & 0xFF;
    uint8_t sg = (src >> 8) & 0xFF;
    uint8_t sb = src & 0xFF;

    uint8_t da = (dst >> 24) & 0xFF;
    uint8_t dr = (dst >> 16) & 0xFF;
    uint8_t dg = (dst >> 8) & 0xFF;
    uint8_t db = dst & 0xFF;

    a = sa + (da * (255 - sa)) / 255;

    if (a == 0)
        return 0;

    r = ((sr * sa) * 255 + dr * da * (255 - sa)) / a / 255;
    g = ((sg * sa) * 255 + dg * da * (255 - sa)) / a / 255;
    b = ((sb * sa) * 255 + db * da * (255 - sa)) / a / 255;

    return (a << 24) | (r << 16) | (g << 8) | b;
}

void drawGlyph(unsigned char *dest, int destwidth, int destheight, int x, int y, int w, int h, int pitch, unsigned char *data, bool antialiased)
{
    if (antialiased)
    {
        for (int i = 0; i < w * h; i++)
        {
            //TODO: Replace dirty hack
            if (((y + i / w) >= destheight) || ((y + i / w) < 0) || (x + i % w < 0) || (x + i % w >= destwidth))
                continue;

            dest[(y + i / w) * destwidth + x + i % w] |= data[i];
        }
    }
    else
    {
        for (int i = 0; i < w * h; i++)
        {
            //TODO: Replace dirty hack
            if (((y + i / w) >= destheight) || ((y + i / w) < 0) || (x + i % w < 0) || (x + i % w >= destwidth))
                continue;

            //t = (i / w) * ((w - 1) / 8 + 1) + i % w
            dest[(y + i / w) * destwidth + x + i % w] |= data[(i / w * pitch) + (i % w) / 8] & (1 << (7 - i % w)) ? 0xff : 0;
        }
    }
}

void drawGlyph(unsigned char *dest, int destwidth, int destheight, int x, int y, int w, int h, int pitch, unsigned char *data, unsigned int color, bool antialiased)
{
    if (antialiased)
    {
        for (int i = 0; i < w * h; i++)
        {
            //TODO: Replace dirty hack
            if (((y + i / w) >= destheight) || ((y + i / w) < 0) || (x + i % w < 0) || (x + i % w >= destwidth))
                continue;

            uint32_t bgColor = ((uint32_t*)dest)[((y + i / w) * destwidth + x + i % w)];
            ((uint32_t*)dest)[((y + i / w) * destwidth + x + i % w)] = blendARGB(bgColor, (data[i] << 24) | (color & 0xFFFFFF));
        }
    }
    else
    {
        for (int i = 0; i < w * h; i++)
        {
            //TODO: Replace dirty hack
            if (((y + i / w) >= destheight) || ((y + i / w) < 0) || (x + i % w < 0) || (x + i % w >= destwidth))
                continue;

            dest[((y + i / w) * destwidth + x + i % w) * 4 + 1] = (color & 0xff0000) >> 16;
            dest[((y + i / w) * destwidth + x + i % w) * 4 + 2] = (color & 0xff00) >> 8;
            dest[((y + i / w) * destwidth + x + i % w) * 4 + 3] = color & 0xff;
            dest[((y + i / w) * destwidth + x + i % w) * 4 + 0] = data[(i / w * pitch) + (i % w) / 8] & (1 << (7 - i % w)) ? 0xff : 0;
        }
    }

}
}
//TODO: Move this
extern FT_Library trueTypeLibrary;

//TODO: Error handling
FTFont::FTFont(const char* data, size_t dataSize, int size, bool antialiased): Font(),
    m_antialiased(antialiased)
{
    m_fontSize = size;
    m_fontData = boost::shared_array<char>(new char[dataSize]);
    memcpy(m_fontData.get(), data, dataSize);
    FT_New_Memory_Face(trueTypeLibrary, (FT_Byte *)m_fontData.get(), dataSize, 0, &m_fontFace);
    FT_Set_Pixel_Sizes(m_fontFace, 0, size);
}

FTFont::~FTFont()
{
    FT_Done_Face(m_fontFace);
}

int FTFont::calculateStringWidth(const std::wstring& str, int from, int length) const
{
    int width = 0;
    int x = 0;
    for (int i = from; i < from + length; i++)
    {
        if (str[i] == L'\n')
        {
            width = x > width ? x : width;
            x = 0;
            continue;
        }
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(m_fontFace, str[i]);
        FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
        //bearingY = bearingY < fontFace->glyph->metrics.horiBearingY >> 6 ? fontFace->glyph->metrics.horiBearingY >> 6 : bearingY;

        x += m_fontFace->glyph->advance.x >> 6;
    }
    width = x > width ? x : width;
    return width;
}

int FTFont::maxChars(const std::wstring& str, int from, int length, int width) const
{
    int x = 0;
    for (int i = from; i < from + length; i++)
    {
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(m_fontFace, str[i]);
        FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
        //bearingY = bearingY < fontFace->glyph->metrics.horiBearingY >> 6 ? fontFace->glyph->metrics.horiBearingY >> 6 : bearingY;

        x += m_fontFace->glyph->advance.x >> 6;
        if (x > width)
            return i - from;
    }
    return length;
}

boost::shared_ptr<Texture> FTFont::renderText(const std::wstring& t, int wrapWidth) const
{
    int x = 0;
    int lines = 1;
    int bearingY = 0;
    int width = 0;
    int i;

    std::wstring text = t;

    if ((wrapWidth) && (wrapWidth < (m_fontFace->max_advance_width >> 6)))
        return boost::shared_ptr<Texture>();


    int pos = 0;
    while ((pos = text.find('\t', pos)) >= 0)
    {
        text.replace(pos, 1, L"    ");
        pos += 4;
    }

    for (i = 0; i < text.length(); i++)
    {
        if (text.at(i) == '\n')
        {
            width = x > width ? x : width;
            x = 0;
            lines++;
            continue;
        }
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(m_fontFace, text.at(i));
        FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
        //bearingY = bearingY < fontFace->glyph->metrics.horiBearingY >> 6 ? fontFace->glyph->metrics.horiBearingY >> 6 : bearingY;

        x += m_fontFace->glyph->advance.x >> 6;

        if ((wrapWidth) && (x > wrapWidth))
        {
            int pos = 0;
            int lastnl = 0;
            while (1)
            {
                pos = text.find(L"\n", pos);
                if ((pos > i) || (pos == std::wstring::npos))
                    break;
                lastnl = pos;
                pos++;
            }


            int lastsp = lastnl;
            pos = lastnl;
            while (1)
            {
                pos = text.find(L" ", pos);
                if ((pos > i) || (pos == std::wstring::npos))
                    break;
                lastsp = pos;
                pos++;
            }

            if (lastsp == lastnl)
                text.insert(i - 1, L"-\n");
            else
                text.replace(lastsp, 1, L"\n");

            x = 0;
            lines = 1;
            width = 0;
            bearingY = 0;
            i = 0;
        }
    }
    width = x > width ? x : width;

    //FIXME: Baseline workaround.
    FT_UInt glyph_index;
    glyph_index = FT_Get_Char_Index(m_fontFace, 'f');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    glyph_index = FT_Get_Char_Index(m_fontFace, 'W');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    glyph_index = FT_Get_Char_Index(m_fontFace, 'A');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    //bearingY = (fontFace->size->metrics.ascending >> 6) - fontSize/10 -1;


    int fullHeight = lines * m_fontSize;
    int fullWidth;

    if (wrapWidth)
        fullWidth = wrapWidth;
    else
        fullWidth = width;

    x = 0;
    int line = 0;

    unsigned char *textureData = new unsigned char[fullWidth * fullHeight];
    memset(textureData, 0, fullWidth * fullHeight);

    for (i = 0; i < text.length(); i++)
    {
        if (text.at(i) == '\n')
        {
            x = 0;
            line++;
        }
        else
        {
            FT_UInt glyph_index;
            glyph_index = FT_Get_Char_Index(m_fontFace, text.at(i));
            FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
            if (m_antialiased)
                FT_Render_Glyph(m_fontFace->glyph, FT_RENDER_MODE_NORMAL);
            else
                FT_Render_Glyph(m_fontFace->glyph, FT_RENDER_MODE_MONO);

            drawGlyph(textureData, fullWidth, fullHeight,
                      x + m_fontFace->glyph->bitmap_left, bearingY - m_fontFace->glyph->bitmap_top + line * m_fontSize,
                      m_fontFace->glyph->bitmap.width, m_fontFace->glyph->bitmap.rows, m_fontFace->glyph->bitmap.pitch,
                      m_fontFace->glyph->bitmap.buffer, m_antialiased);

            x += m_fontFace->glyph->advance.x >> 6;
        }
    }
    Texture *texture = new Texture(fullWidth, fullHeight, TEXTURE_A8, textureData);
    delete[] textureData;
    return boost::shared_ptr<Texture>(texture);
}

boost::shared_ptr<Texture> FTFont::renderColoredText(const std::wstring& t, int defaultTextColor, int selectionTextColor, int wrapWidth) const
{
    int x = 0;
    int lines = 1;
    int bearingY = 0;
    int width = 0;
    int i;

    std::map<int, unsigned int> colorSelect;
    unsigned int currentColor = defaultTextColor;

    std::wstring text = t;

    if ((wrapWidth) && (wrapWidth < (m_fontFace->max_advance_width >> 6)))
        return boost::shared_ptr<Texture>();


    int pos = 0;
    while ((pos = text.find('\t', pos)) >= 0)
    {
        text.replace(pos, 1, L"    ");
        pos += 4;
    }
    pos = 0;
    //TODO: hex check
    while ((pos = text.find(L"\\c", pos)) >= 0)
    {
        if (text.length() < pos + 3)
            break;

        if (text[pos + 2] == L'R')
        {
            colorSelect[pos] = defaultTextColor;
            text.erase(pos, 3);
            continue;
        }
        if (text[pos + 2] == L'S')
        {
            colorSelect[pos] = selectionTextColor;
            text.erase(pos, 3);
            continue;
        }
        if (text.length() < pos + 8)
            break;

        std::wstring colorstring = text.substr(pos + 2, 6);
        text.erase(pos, 8);
        std::wistringstream ss(colorstring);
        int color;
        ss >> std::hex >> color;
        colorSelect[pos] = color;
    }

    for (i = 0; i < text.length(); i++)
    {
        if (text.at(i) == '\n')
        {
            width = x > width ? x : width;
            x = 0;
            lines++;
            continue;
        }
        FT_UInt glyph_index;
        glyph_index = FT_Get_Char_Index(m_fontFace, text.at(i));
        FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
        //bearingY = bearingY < fontFace->glyph->metrics.horiBearingY >> 6 ? fontFace->glyph->metrics.horiBearingY >> 6 : bearingY;

        x += m_fontFace->glyph->advance.x >> 6;

        if ((wrapWidth) && (x > wrapWidth))
        {
            int pos = 0;
            int lastnl = 0;
            while (1)
            {
                pos = text.find(L"\n", pos);
                if ((pos > i) || (pos == std::wstring::npos))
                    break;
                lastnl = pos;
                pos++;
            }


            int lastsp = lastnl;
            pos = lastnl;
            while (1)
            {
                pos = text.find(L" ", pos);
                if ((pos > i) || (pos == std::wstring::npos))
                    break;
                lastsp = pos;
                pos++;
            }

            if (lastsp == lastnl)
                text.insert(i - 1, L"-\n");
            else
                text.replace(lastsp, 1, L"\n");

            x = 0;
            lines = 1;
            width = 0;
            bearingY = 0;
            i = 0;
        }
    }
    width = x > width ? x : width;

    //FIXME: Baseline workaround.
    FT_UInt glyph_index;
    glyph_index = FT_Get_Char_Index(m_fontFace, 'f');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    glyph_index = FT_Get_Char_Index(m_fontFace, 'W');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    glyph_index = FT_Get_Char_Index(m_fontFace, 'A');
    FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
    bearingY = bearingY > m_fontFace->glyph->metrics.horiBearingY >> 6 ? bearingY : m_fontFace->glyph->metrics.horiBearingY >> 6;

    //bearingY = (fontFace->size->metrics.ascending >> 6) - fontSize/10 -1;


    int fullHeight = lines * m_fontSize;
    int fullWidth;

    if (wrapWidth)
        fullWidth = wrapWidth + 1;
    else
        fullWidth = width + 1;

    x = 0;
    int line = 0;

    unsigned char *textureData = new unsigned char[fullWidth * fullHeight * 4];
    memset(textureData, 0, fullWidth * fullHeight * 4);

    for (i = 0; i < text.length(); i++)
    {
        std::map<int, unsigned int>::const_iterator it;
        if ((it = colorSelect.find(i)) != colorSelect.end())
            currentColor = it->second;
        if (text.at(i) == '\n')
        {
            x = 0;
            line++;
        }
        else
        {
            FT_UInt glyph_index;
            glyph_index = FT_Get_Char_Index(m_fontFace, text.at(i));
            FT_Load_Glyph(m_fontFace, glyph_index, FT_LOAD_DEFAULT);
            if (m_antialiased)
                FT_Render_Glyph(m_fontFace->glyph, FT_RENDER_MODE_NORMAL);
            else
                FT_Render_Glyph(m_fontFace->glyph, FT_RENDER_MODE_MONO);

            drawGlyph(textureData, fullWidth, fullHeight,
                      x + m_fontFace->glyph->bitmap_left, bearingY - m_fontFace->glyph->bitmap_top + line * m_fontSize,
                      m_fontFace->glyph->bitmap.width, m_fontFace->glyph->bitmap.rows, m_fontFace->glyph->bitmap.pitch,
                      m_fontFace->glyph->bitmap.buffer, currentColor, m_antialiased);

            x += m_fontFace->glyph->advance.x >> 6;
        }
    }
    Texture *texture = new Texture(fullWidth, fullHeight, TEXTURE_B8G8R8A8, textureData);
    delete[] textureData;
    return boost::shared_ptr<Texture>(texture);
}

bool FTFont::antialiased() const
{
    return m_antialiased;
}
}

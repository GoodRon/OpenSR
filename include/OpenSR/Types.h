/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2012 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_TYPES_H
#define RANGERS_TYPES_H

#include "config.h"

#include <string>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace Rangers
{
class Texture;
class TextureRegionDescriptor;

struct Vector
{
    Vector();
    Vector(float x, float y);

    float x, y;
};

struct Vertex
{
    float x, y;
    float u, v;
};

struct Rect
{
    Rect();
    Rect(float x1, float y1, float x2, float y2);
    float x, y;
    float width, height;
    Rect& operator+=(const Rect& other);
    bool contains(const Vector& v);
    bool valid() const;
};

//! Struct that handles region of texture.
struct TextureRegion
{
    //! Constructs texture region.
    TextureRegion(boost::shared_ptr<Texture> texture, int x, int y, int width, int height);
    TextureRegion(const TextureRegionDescriptor& desc);
    TextureRegion(boost::shared_ptr<Texture> texture);
    TextureRegion();
    boost::shared_ptr<Texture> texture;
    float u1, v1, u2, v2;
};

struct NinePatchDescriptor
{
    int rows;
    int columns;
    std::vector<TextureRegionDescriptor> regions;
    std::vector<int> sizeableRows;
    std::vector<int> sizeableColumns;
};

struct FontDescriptor
{
    std::wstring path;
    int size;
};

struct TextureRegionDescriptor
{
    std::wstring texture;
    int x, y, width, height;
};

Rect operator+(const Rect& r1, const Rect& r2);
}

#endif

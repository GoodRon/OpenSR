/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2012 Kosyak <ObKo@mail.ru>

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

#include "Item.h"

namespace Rangers
{
namespace World
{
uint32_t Item::cost() const
{
    return m_cost;
}

bool Item::deserialize(std::istream& stream)
{
    return WorldObject::deserialize(stream);
}

std::wstring Item::name() const
{
    return m_name;
}

bool Item::serialize(std::ostream& stream) const
{
    return WorldObject::serialize(stream);
}

uint32_t Item::size() const
{
    return m_size;
}

uint32_t Item::type() const
{
    return 0x4b74e518;
}
}
}

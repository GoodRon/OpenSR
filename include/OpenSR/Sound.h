/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2011 - 2013 Kosyak <ObKo@mail.ru>

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

#ifndef RANGERS_SOUND_H
#define RANGERS_SOUND_H

#include "OpenSR/config.h"

#include <string>
#include <boost/shared_ptr.hpp>

struct Mix_Chunk;
namespace Rangers
{
class RANGERS_ENGINE_API Sound
{
public:
    Sound();
    Sound(const std::string& path);

    void play();

private:
    boost::shared_ptr<Mix_Chunk> m_chunk;
};
}

#endif // RANGERS_SOUND_H

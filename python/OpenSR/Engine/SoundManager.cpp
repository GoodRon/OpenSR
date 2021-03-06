/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

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

#include <OpenSR/SoundManager.h>
#include <OpenSR/Sound.h>
#include <boost/python.hpp>

namespace Rangers
{
namespace Python
{
void exportSoundManager()
{
    using namespace boost::python;
    class_<SoundManager, boost::noncopyable>("SoundManager", no_init)
    .def("playMusic", &SoundManager::playMusic)
    .def("loadSound", &SoundManager::loadSound)
    .def("instance", &SoundManager::instance, return_value_policy<reference_existing_object>())
    .staticmethod("instance");
}
}
}

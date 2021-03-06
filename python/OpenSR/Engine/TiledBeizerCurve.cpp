/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 - 2014 Kosyak <ObKo@mail.ru>

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

#include <boost/python.hpp>
#include <OpenSR/TiledBeizerCurve.h>
#include <OpenSR/Texture.h>

#include "OpenSR/python/ObjectWrap.h"

namespace Rangers
{
namespace Python
{
struct TiledBeizerCurveWrap: TiledBeizerCurve, boost::python::wrapper<TiledBeizerCurve>
{
    TiledBeizerCurveWrap(): TiledBeizerCurve()
    {
    }

    TiledBeizerCurveWrap(boost::shared_ptr<Texture> texture)
        : TiledBeizerCurve(texture)
    {
    }

    TiledBeizerCurveWrap(const std::string& texture)
        : TiledBeizerCurve(texture)
    {
    }

    RANGERS_PYTHON_WRAP_OBJECT(TiledBeizerCurve)
};

void exportTiledBeizerCurve()
{
    using namespace boost::python;

    class_<TiledBeizerCurveWrap, bases<Object>, boost::shared_ptr<TiledBeizerCurveWrap>, boost::noncopyable> c("TiledBeizerCurve", init<boost::shared_ptr<Texture> >());
    c.def(init<const std::string&>())
    .def(init<>())
    .add_property("curve", &TiledBeizerCurve::curve, &TiledBeizerCurve::setCurve);
    RANGERS_PYTHON_WRAP_OBJECT_DEF(TiledBeizerCurve, TiledBeizerCurveWrap, c);
    register_ptr_to_python<boost::shared_ptr<TiledBeizerCurve> >();
}
}
}

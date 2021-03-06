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
#include <OpenSR/python/WidgetWrap.h>

#include <OpenSR/World/InfoWidget.h>
#include <OpenSR/World/SkinManager.h>

namespace Rangers
{
namespace World
{
namespace Python
{
struct InfoWidgetWrap : InfoWidget, boost::python::wrapper<InfoWidget>
{
    InfoWidgetWrap(boost::shared_ptr<InfoWidgetStyle> style)
        : InfoWidget(style)
    {
    }
    RANGERS_PYTHON_WRAP_WIDGET(InfoWidget)
};

void exportInfoWidget()
{
    using namespace boost::python;

    class_<InfoWidgetWrap, bases<Widget>, boost::shared_ptr<InfoWidgetWrap>, boost::noncopyable> c("InfoWidget", init<boost::shared_ptr<InfoWidgetStyle> >());
    c.def("clear", &InfoWidget::clear)
    .def("show", &InfoWidget::show)
    .def("addContentProvider", &InfoWidget::addContentProvider);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(InfoWidget, InfoWidgetWrap, c);
    register_ptr_to_python<boost::shared_ptr<InfoWidget> >();
}
}
}
}
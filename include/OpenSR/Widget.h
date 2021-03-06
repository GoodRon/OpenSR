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

#ifndef RANGERS_WIDGET_H
#define RANGERS_WIDGET_H

#include "OpenSR/Object.h"

#include <list>

namespace Rangers
{
class WidgetPrivate;
class ActionListener;
class Action;

class RANGERS_ENGINE_API Widget: public Object
{
    RANGERS_DECLARE_PRIVATE(Widget)
public:
    Widget();
    Widget(float w, float h);

    virtual ~Widget();

    void mouseMove(float x, float y);


    //! Is widget contains point. Used in mouse tracking
    virtual bool containsPoint(const Vector &p) const;
    //! Widget's bounding rect, including children
    virtual Rect boundingRect() const;

    virtual void mouseMove(const Vector &p);

    virtual int minWidth() const;
    virtual int minHeight() const;
    virtual int preferredWidth() const;
    virtual int preferredHeight() const;
    virtual int maxWidth() const;
    virtual int maxHeight() const;
    virtual void focus();
    virtual void unFocus();

    int width() const;
    int height() const;
    bool isFocused() const;
    bool isVisible() const;

    void setWidth(int width);
    void setHeight(int height);
    void setGeometry(int width, int height);
    void setVisible(bool visible);

    void addWidget(boost::shared_ptr<Widget> w);
    void removeWidget(boost::shared_ptr<Widget> w);
    std::list<boost::shared_ptr<Widget> > childWidgets() const;

    void addListener(boost::shared_ptr<ActionListener> listener);
    void removeListener(boost::shared_ptr<ActionListener> listener);

    void action(const Action& action);

protected:
    Widget(WidgetPrivate &p);

    virtual bool prepareDraw() const;

    RANGERS_DISABLE_COPY(Widget)
};
}

#endif // WIDGET_H

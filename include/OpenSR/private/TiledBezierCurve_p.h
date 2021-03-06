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

#ifndef RANGERS_TILED_BEZIER_CURVE_P_H
#define RANGERS_TILED_BEZIER_CURVE_P_H

#include "TiledPolyline_p.h"

namespace Rangers
{
class TiledBezierCurvePrivate: public TiledPolylinePrivate
{
    RANGERS_DECLARE_PUBLIC(TiledBezierCurve)
public:
    TiledBezierCurvePrivate();

    void findPoints(float t0, float t1, std::list<Vector>& points, std::list<Vector>::iterator& i);
    Vector calcBezierPoint(float t, Vector& d);

    BezierCurve curve;
};
}

#endif
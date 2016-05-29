//
// Created by Nagash on 09/05/16.
//

#ifndef DISPLAYIMAGE_POINT_H
#define DISPLAYIMAGE_POINT_H

#include "imProc.h"

namespace imProc
{

    template <class T>
    class Point
    {
    private:
        T _y;
        T _x;


    public:

        // y, x ---> row, col
        Point(const T& y, const T& x) :  _y(y), _x(x) {}


        inline const int &y() const { return _y; }
        inline const int &x() const { return _x; }

        inline void y(const T& new_y) { _y = new_y; }
        inline void x(const T& new_x) { _x = new_x; }

    };
}

#endif //DISPLAYIMAGE_POINT_H

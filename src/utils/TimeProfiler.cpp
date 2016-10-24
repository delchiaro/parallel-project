//
// Created by Nagash on 18/05/16.
//

#include "TimeProfiler.h"

std::ostream &operator<<(std::ostream &os, TimeProfiler const &m)  {
    //return os << m.getSec() << "s\t" << m.getUsec() << "uS";
    return os << m.getSec() << "s\t" << m.getMsec() << "mS\t" << m.getUsec() << "uS";

}
#include "astrocalc.h"

#include <QtMath>

namespace AstroCalc
{

double deg2rad(double deg)
{
    return M_PI * deg / 180;
}

} // namespace AstroCalc

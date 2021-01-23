#include "astrocalc.h"

#include <QtMath>

namespace AstroCalc
{

double deg2rad(double deg)
{
    return M_PI * deg / 180;
}

double julianDay(const QDateTime &datetime)
{
    // Convert to UTC so that we remove all timezone data we do not need
    const QDateTime utc = datetime.toUTC();
    const QDate date = utc.date();
    const QTime time = utc.time();

    // Get Julian Day from the date
    double jd = date.toJulianDay();

    int h(time.hour());
    int min(time.minute());
    int s(time.second());

    // Compute adjustment for time of the day
    double T = h / 24.0 + min / 1440.0 + s / 86400.0;

    // Remove 0.5 because Julian Days change at noon UTC
    T -= 0.5;
    return jd + T;
}

double julianDay2000(const QDateTime &datetime)
{
    return julianDay(datetime) - 2451545.0;
}

} // namespace AstroCalc

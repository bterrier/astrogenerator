#ifndef ASTROCALC_H
#define ASTROCALC_H

#include <QDateTime>

namespace AstroCalc
{

double deg2rad(double deg);

double julianDay(const QDateTime &datetime);
inline double julianDay(const QDate &date, const QTime &time)
{
    return julianDay(QDateTime(date, time));
}

double julianDay2000(const QDateTime &datetime);
inline double julianDay2000(const QDate &date, const QTime &time)
{
    return julianDay2000(QDateTime(date, time));
}

} // namespace AstroCalc

#endif // ASTROCALC_H

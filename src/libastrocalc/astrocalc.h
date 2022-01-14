#ifndef ASTROCALC_H
#define ASTROCALC_H

#include <QDateTime>

namespace AstroCalc
{

struct DMS {
public:
	static DMS fromDegrees(double deg);
	[[nodiscard]] QString toString() const;

private:
	int sign;
	int d;
	int minutes;
	double seconds;
};

QString degreeToDms(double val);

double deg2rad(double deg);
double rad2deg(double rad);

double toZero360(double deg);

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

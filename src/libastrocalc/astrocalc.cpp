#include "astrocalc.h"

#include <QtMath>

namespace AstroCalc
{

double deg2rad(double deg)
{
	return M_PI * deg / 180;
}

double rad2deg(double rad)
{
	return 180 * rad / M_PI;
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

double toZero360(double deg)
{
	double result = std::fmod(deg, 360.0);

	if (result < 0)
		result += 360.0;

	return result;
}

QString degreeToDms(double val)
{
	return DMS::fromDegrees(val).toString();
}

DMS DMS::fromDegrees(double deg)
{
	DMS result;

	if (deg < 0.0) {
		result.sign = -1;
		deg *= -1;
	} else {
		result.sign = 1;
	}

	long double i = 0.0;
	long double dec = std::modf((long double)(deg), &i);
	result.d = i;
	long double m = 60.0 * dec;

	dec = std::modf(m, &i);

	result.minutes = i;
	result.seconds = 60.0 * dec;
	result.seconds = std::round(1000.0 * result.seconds) / 1000.0;

	if (result.seconds == 60) {
		result.seconds = 0;
		result.minutes += 1;

		if (result.minutes == 60) {
			result.minutes = 0;
			result.d += 1;
		}
	}

	return result;
}

QString DMS::toString() const
{
	return QStringLiteral("%1%2° %3' %4\"").arg(sign > 0 ? "" : "-").arg(d).arg(minutes).arg(seconds);
}

} // namespace AstroCalc

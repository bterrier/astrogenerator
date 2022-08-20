#ifndef HAUTEURMAXTEMPS_H
#define HAUTEURMAXTEMPS_H

#include <QDateTime>

class HauteurMaxTemps
{
public:
	HauteurMaxTemps(const QDateTime &temps, double hauteurMax);

	[[nodiscard]] QDateTime temps() const
	{
		return m_temps;
	}

	[[nodiscard]] constexpr double hauteurMax() const noexcept
	{
		return m_hauteurMax;
	}

	void setDateTime(const QDateTime &date);
	void setHauteurMax(double hauteur);

private:
	QDateTime m_temps;
	double m_hauteurMax;
};

#endif // HAUTEURMAXTEMPS_H

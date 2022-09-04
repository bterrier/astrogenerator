#include <QtTest>

#include "astrocalc.h"

class tst_AstroCalc : public QObject
{
	Q_OBJECT

public:
	tst_AstroCalc();
	~tst_AstroCalc();

private slots:
	void test_dms();
	void test_deg2rad();
	void test_rad2deg();
	void test_julian();
	void test_julian2000();
	void test_angleNormalization();
};

tst_AstroCalc::tst_AstroCalc()
{
}

tst_AstroCalc::~tst_AstroCalc()
{
}

void tst_AstroCalc::test_dms()
{
	using AstroCalc::DMS;

	QCOMPARE("0° 0' 0\"", DMS::fromDegrees(0).toString());
	QCOMPARE("0° 30' 0\"", DMS::fromDegrees(0.5).toString());
	QCOMPARE("-0° 30' 0\"", DMS::fromDegrees(-0.5).toString());
	QCOMPARE("-42° 30' 0\"", DMS::fromDegrees(-42.5).toString());
	QCOMPARE("424° 12' 0\"", DMS::fromDegrees(424.2).toString());
	QCOMPARE("45° 25' 12\"", DMS::fromDegrees(45.42).toString());
}

void tst_AstroCalc::test_deg2rad()
{
	QCOMPARE(AstroCalc::deg2rad(0), 0);
	QCOMPARE(AstroCalc::deg2rad(180), M_PI);
	QCOMPARE(AstroCalc::deg2rad(90), M_PI_2);
}

void tst_AstroCalc::test_rad2deg()
{
	QCOMPARE(AstroCalc::rad2deg(0), 0);
	QCOMPARE(AstroCalc::rad2deg(M_PI), 180);
	QCOMPARE(AstroCalc::rad2deg(M_PI_2), 90);
}

void tst_AstroCalc::test_julian()
{
	QDateTime dt;

	dt = QDateTime::fromString("1582-10-15T00:00:00Z", Qt::ISODate);
	QCOMPARE(AstroCalc::julianDay(dt), 2299160.5);

	// J2000 epoch
	// Note that the actual epoch is "2000-01-01T11:58:55.816" TT
	// But we do not handle TT
	dt = QDateTime::fromString("2000-01-01T12:00:00Z", Qt::ISODate);
	QCOMPARE(AstroCalc::julianDay(dt), 2451545.0);
}

void tst_AstroCalc::test_julian2000()
{
	QDateTime dt;

	// J2000 epoch
	// Note that the actual epoch is "2000-01-01T11:58:55.816" TT
	// But we do not handle TT
	dt = QDateTime::fromString("2000-01-01T12:00:00Z", Qt::ISODate);
	QCOMPARE(AstroCalc::julianDay2000(dt), 0);
}

void tst_AstroCalc::test_angleNormalization()
{
	QCOMPARE(AstroCalc::toZero360(0.0), 0.0);
	QCOMPARE(AstroCalc::toZero360(-360.0), 0.0);
	QCOMPARE(AstroCalc::toZero360(360.0), 0.0);
	QCOMPARE(AstroCalc::toZero360(720.0), 0.0);
	QCOMPARE(AstroCalc::toZero360(180.0), 180.0);
	QCOMPARE(AstroCalc::toZero360(730.0), 10.0);
	QCOMPARE(AstroCalc::toZero360(402.42), 42.42);
	QCOMPARE(AstroCalc::toZero360(-3.5), 356.5);
}

QTEST_APPLESS_MAIN(tst_AstroCalc)

#include "tst_astrocalc.moc"

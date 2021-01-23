#include <QtTest>

#include "astrocalc.h"

class tst_AstroCalc : public QObject
{
    Q_OBJECT

public:
    tst_AstroCalc();
    ~tst_AstroCalc();

private slots:
    void test_case1();
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

void tst_AstroCalc::test_case1()
{
    QCOMPARE(AstroCalc::deg2rad(0), 0);
    QCOMPARE(AstroCalc::deg2rad(180), M_PI);
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

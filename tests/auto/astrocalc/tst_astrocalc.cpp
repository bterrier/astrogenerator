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

QTEST_APPLESS_MAIN(tst_AstroCalc)

#include "tst_astrocalc.moc"

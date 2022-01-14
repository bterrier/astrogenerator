#ifndef COMPTEREBOURS_H
#define COMPTEREBOURS_H

#include <QLCDNumber>

class CompteRebours : public QWidget
{
	Q_OBJECT
public:
	CompteRebours();
	QString secToMinSec(int sec) const;

signals:
	void fin();

public slots:
	void enlever();
	void commencer(int secondes = 10);

private:
	QTimer *timer;
	QLCDNumber *m_lcd;
	int secondeRestant;
};

#endif // COMPTEREBOURS_H

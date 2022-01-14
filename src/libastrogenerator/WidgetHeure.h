#ifndef WIDGETHEURE_H
#define WIDGETHEURE_H

#include <QLCDNumber>
#include <QTimer>

class WidgetHeure : public QLCDNumber
{
	Q_OBJECT

public:
	explicit WidgetHeure(QWidget *parent = nullptr);

private slots:
	void refresh();

private:
	QTimer m_timer;
};

#endif // WIDGETHEURE_H

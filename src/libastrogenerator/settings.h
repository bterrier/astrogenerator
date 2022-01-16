#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

#include "eyepiece.h"

class Settings : public QObject
{
	Q_OBJECT
public:
	static Settings &instance();

	[[nodiscard]] QList<Eyepiece> eyepieces() const;

signals:
private:
	explicit Settings(QObject *parent = nullptr);
	static Settings *s_instance;
	QSettings m_settings;
};

#endif // SETTINGS_H

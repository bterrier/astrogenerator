#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

#include "eyepiece.h"

struct Notes {
	int amasGlobulaire;
	int amasNebuleuse;
	int nebuleusePlanetaire;
	int nebuleuseReflection;
	int etoiletripe;
	int etoiledouble;
	int galaxy;
	int amasOuvert;
};

class Settings : public QObject
{
	Q_OBJECT
public:
	static Settings &instance();

	[[nodiscard]] QList<Eyepiece> eyepieces() const;
	[[nodiscard]] QString city() const;
	[[nodiscard]] QString country() const;
	[[nodiscard]] int difficulty() const;
	void setDifficulty(int level);
	[[nodiscard]] QString telescopName() const;

	void setLatitude(double value);
	void setLongitude(double value);

	[[nodiscard]] Notes notes() const;

signals:
private:
	explicit Settings(QObject *parent = nullptr);
	static Settings *s_instance;
	QSettings m_settings;
};

#endif // SETTINGS_H

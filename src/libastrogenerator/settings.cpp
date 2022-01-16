#include "settings.h"

#include "Constantes.h"

Settings *Settings::s_instance = nullptr;

Settings &Settings::instance()
{
	if (!s_instance)
		s_instance = new Settings();

	return *s_instance;
}

Settings::Settings(QObject *parent) :
    QObject{parent},
    m_settings{NOM_EQUIPE, NOM_PROGRAMME}
{
}

QList<Eyepiece> Settings::eyepieces() const
{
	const auto values = m_settings.value("oculaires", OCULAIREES_DEFAUT).toString().split("|");

	QList<Eyepiece> result;
	result.reserve(values.size());

	for (const auto &val : values) {
		result.append(Eyepiece{val.toInt()});
	}

	return result;
}

QString Settings::city() const
{
	return m_settings.value("localisation/ville", VILLE_DEFAUT).toString();
}

QString Settings::country() const
{
	return m_settings.value("localisation/pays", PAYS_DEFAUT).toString();
}

int Settings::difficulty() const
{
	return m_settings.value("niveau", 0).toInt();
}

void Settings::setDifficulty(int level)
{
	m_settings.setValue("niveau", level);
}

QString Settings::telescopName() const
{
	return m_settings.value("telescope/nom", TELESCOPE_DEFAUT).toString();
}

void Settings::setLatitude(double value)
{
	m_settings.setValue("latitude", value);
}

void Settings::setLongitude(double value)
{
	m_settings.setValue("longitude", value);
}

Notes Settings::notes() const
{
	return {
		m_settings.value("generateur/amasGlobulaire", NOTE_AMAS_GLOBULAIRE).toInt(),
		m_settings.value("generateur/amasNebuleuse", NOTE_AMAS_NEBULEUSE).toInt(),
		m_settings.value("generateur/nebuleusePlanetaire", NOTE_NEBULEUSE_PLANETAIRE).toInt(),
		m_settings.value("generateur/nebuleuseReflection", NOTE_NEBULEUSE_REFLECTION).toInt(),
		m_settings.value("generateur/etoileTriple", NOTE_ETOILE_TRIPLE).toInt(),
		m_settings.value("generateur/etoileDouble", NOTE_ETOILE_DOUBLE).toInt(),
		m_settings.value("generateur/galaxie", NOTE_GALAXIE).toInt(),
		m_settings.value("generateur/amasOuvert", NOTE_AMAS_OUVERT).toInt(),
	};
}

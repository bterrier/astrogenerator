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

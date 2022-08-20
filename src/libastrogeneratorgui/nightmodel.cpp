#include "nightmodel.h"

NightModel::NightModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

QVariant NightModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
		case Name:
			return tr("Name");
		case Reference:
			return tr("Reference");
		case RightAscension:
			return tr("Right ascension");
		case Declination:
			return tr("Declination");
		case Magn:
			return tr("Magn");
		case Constellation:
			return tr("Name");
		case Difficulty:
			return tr("Difficulty");
		case Interest:
			return tr("Interest");
		case Time:
			return tr("Time");
		}
	}

	return QVariant();
}

int NightModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	if (m_night == nullptr)
		return 0;

	return m_night->getPlanning().count();
}

int NightModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	return ColumnCount;
}

QVariant NightModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || m_night == nullptr)
		return QVariant();

	const auto planning = m_night->getPlanning();

	const int row = index.row();
	const int column = index.column();

	if (role == Qt::DisplayRole) {
		auto obj = planning.at(row);
		switch (column) {
		case Name:
			return obj->nomComplet();
		case Reference:
			return obj->ref();
		case RightAscension:
			return obj->ascdr();
		case Declination:
			return obj->declinaison();
		case Magn:
			return obj->magnitude();
		case Constellation:
			return obj->constellation();
		case Difficulty:
			return obj->difficulte(true);
		case Interest:
			return obj->interet(true);
		case Time:
			return obj->getDebut().toLocalTime().time().toString(tr("hh:mm"));
		}
	}

	return QVariant();
}

void NightModel::setNight(Soiree *newNight)
{
	if (newNight != m_night) {
		beginResetModel();
		m_night = newNight;
		doReload();
		endResetModel();
	}
}

void NightModel::reload()
{
	beginResetModel();
	doReload();
	endResetModel();
}

void NightModel::doReload() {}

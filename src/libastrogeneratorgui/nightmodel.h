#ifndef NIGHTMODEL_H
#define NIGHTMODEL_H

#include <QAbstractTableModel>

#include "Soiree.h"

class NightModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum Column
	{
		Name,
		Reference,
		RightAscension,
		Declination,
		Magn,
		Constellation,
		Difficulty,
		Interest,
		Time,
		ColumnCount,
	};

	explicit NightModel(QObject *parent = nullptr);

	// Header:
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	void setNight(Soiree *newNight);

public slots:
	void reload();
private slots:
	void doReload();

private:
	Soiree *m_night = nullptr;
};

#endif // NIGHTMODEL_H

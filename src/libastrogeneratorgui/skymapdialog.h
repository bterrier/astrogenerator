#ifndef SKYMAPDIALOG_H
#define SKYMAPDIALOG_H

#include "Carteciel.h"
#include <QDialog>

namespace Ui
{
class SkyMapDialog;
}

class SkyMapDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SkyMapDialog(Carteciel *map, QWidget *parent = nullptr);
	~SkyMapDialog() override;

private slots:
	void saveImage();

private:
	Ui::SkyMapDialog *ui;
	Carteciel *m_map;
};

#endif // SKYMAPDIALOG_H

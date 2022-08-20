#ifndef NIGHTPRINTER_H
#define NIGHTPRINTER_H

#include <QPrinter>

#include "Soiree.h"

class NightPrinter : public QObject
{
	Q_OBJECT
public:
	NightPrinter() = delete;
	static void toPDF(Soiree *night);
	static bool paintPdf(Soiree *night, QPrinter *printer);
};

#endif // NIGHTPRINTER_H

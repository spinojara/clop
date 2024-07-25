#include "CRAIICursor.h"
#include <QtWidgets/QApplication>

CRAIICursor::CRAIICursor(const QCursor &cursor)
{
 QApplication::setOverrideCursor(cursor);
}

CRAIICursor::~CRAIICursor()
{
 QApplication::restoreOverrideCursor();
}

#ifndef CPLOTWIDGET_H
#define CPLOTWIDGET_H

#include <QtWidgets/QWidget>
#include <QtGui/QColor>

#include "COutcome.h"

class MainWindow;

class CPlotWidget : public QWidget
{
Q_OBJECT

    const MainWindow *mw;
    QColor backgroundColor;
    QColor outcomeColor[COutcome::AllOutcomes];

public:
    explicit CPlotWidget(QWidget *parent = 0);
    void setMainWindow(const MainWindow *mwNew) {mw = mwNew;}

public slots:
    void storePointSize(int state);
    void storeSampleWeight(int state);
    void storeMergeReplications(int state);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent( QMouseEvent *event);
};

#endif // CPLOTWIDGET_H

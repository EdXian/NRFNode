#ifndef PLOTTHREAD_H
#define PLOTTHREAD_H
#include <QThread>
#include <QObject>
#include <qcustomplot/qcustomplot.h>
class plotthread : public QObject
 {
     Q_OBJECT
     //QThread plthread;

 public:
     plotthread();
     void setPlotter(QCustomPlot* plot){
        Plot = plot;
     }
     uint32_t count =0;
     void configPlotter();
     void addGraph();
 public slots:
     void update_plot();

 signals:
     void resultReady(const QString &result);

 private:
     QCustomPlot* Plot;
 };


#endif // PLOTTHREAD_H

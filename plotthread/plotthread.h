#ifndef PLOTTHREAD_H
#define PLOTTHREAD_H
#include <QThread>
#include <QObject>
#include <qcustomplot/qcustomplot.h>
#include "bleservices/thingy_service.h"
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

     void resizeRange();


     void update_plot();
 public :
     void onAccDataReceieved(Points3D data);

 signals:
     void resultReady(const QString &result);

 private:
     QCustomPlot* Plot;
 };


#endif // PLOTTHREAD_H

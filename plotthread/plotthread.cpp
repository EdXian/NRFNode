#include "plotthread.h"
#include "math.h"
plotthread::plotthread()
{

}


void plotthread::addGraph(){
    Plot->addGraph();
}

void plotthread::configPlotter(){

     Plot->addGraph();
 //    ui->plotwidget->graph(0).
     Plot->addGraph();
     Plot->addGraph();
     Plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
     Plot->xAxis->setLabel("x");
     Plot->xAxis->setLabel("y");
     Plot->graph(0)->setPen(QPen(QColor(255, 0, 0)));
     Plot->graph(1)->setPen(QPen(QColor(0, 255, 0)));
     Plot->graph(2)->setPen(QPen(QColor(0, 0, 255)));

     Plot->graph(0)->setName("x");
     Plot->graph(1)->setName("y");
     Plot->graph(2)->setName("z");
     Plot->legend->setVisible(true);
     // set axes ranges, so we see all data:
     Plot->yAxis->setRange(-3000, 3000);
     Plot->xAxis->setRange(0, 2000);

}
void plotthread::update_plot(){
     count++;
    Plot->graph(0)->addData(count, cos(2*count*3.14159/30));
    if(count > 1000){
         Plot->graph(0)->data()->removeBefore(count-1000);

         bool ok=true;
         QCPRange range = Plot->graph(0)->getValueRange(ok);
         Plot->yAxis->setRange(range);
         Plot->xAxis->setRange(count+500,count-1000);

    }

    Plot->replot();
}

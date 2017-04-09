#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optimization.h"
#include <vector>

#define customPlot ui->customPlot

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    customPlot->setMinimumWidth(this->width());
    customPlot->setMinimumHeight(this->height()-100);

    Optimization optObj;
    int n = 150;
    double start = 0, end = 1;
    QVector<double> x(n);
    QVector<double> y = optObj.getFuncData(x, start, end, n);

    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(start, end);
    customPlot->yAxis->setRange(0, 1);
    customPlot->replot();

    QVector<double> args;
    QVector<QPair<double, double>> iterations;
    QPair<double, double> min = optObj.bruteforceMethod(iterations, start, end, 10000000, 0.0005);

    for (int i = 0; i < iterations.size(); i++)
    {
        drawLine(iterations[i], i, i == iterations.size()-1);
    }
}

void MainWindow::drawLine(QPair<double, double> iteration, int iterNum, bool isFinal)
{
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setColor(isFinal ? Qt::green : Qt::red);

    QCPItemLine *min1 = new QCPItemLine(customPlot);
    min1->setPen(pen);
    min1->start->setCoords(iteration.first, 0);
    min1->end->setCoords(iteration.first, iteration.second);

    QCPItemLine *min2 = new QCPItemLine(customPlot);
    min2->setPen(pen);
    float shiftY = 0.1;
    min2->start->setCoords(iteration.first, iteration.second);
    min2->end->setCoords(iteration.first, iteration.second+shiftY);

    QCPItemText *textLabel = new QCPItemText(customPlot);
    textLabel->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    textLabel->position->setType(QCPItemPosition::ptPlotCoords);
    textLabel->position->setCoords(iteration.first, iteration.second+shiftY);
    textLabel->setText(QString::number(iteration.first));

    QCPItemText *textLabel2 = new QCPItemText(customPlot);
    textLabel2->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
    textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
    textLabel2->position->setCoords(iteration.first+shiftY/10, iteration.second/2);
    textLabel2->setText(QString::number(iterNum));
}

MainWindow::~MainWindow()
{
    delete ui;
}

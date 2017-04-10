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

    Optimization optObj;
    const int n = 150;
    const double startX = -5, endX = 5, startY = -2, endY = 2;
    const double eps = 0.01;

    createGraph(optObj, n, eps, startX, endX, startY, endY);

    const int iterCount = 10000000;
    const double r = 2;
    QVector<QPair<double, double>> iterations;
//    QPair<double, double> min = optObj.bruteforceMethod(iterations, startX, endX, iterCount, eps);
//    QPair<double, double> min = optObj.PiavskiiMethod(iterations, startX, endX, iterCount, eps, r);
    QPair<double, double> min = optObj.StronginMethod(iterations, startX, endX, iterCount, eps, r);

    showIterations(iterations, min, startY);
}

void MainWindow::createGraph(const Optimization& optObj, int n, double eps, double startX, double endX, double startY, double endY)
{
    customPlot->setMinimumWidth(this->width());
    customPlot->setMinimumHeight(this->height()-100);

    QVector<double> x(n);
    QVector<double> y = optObj.getFuncData(x, startX, endX, n);

    // create graph and assign data to it:
    customPlot->addGraph();
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    customPlot->graph(0)->setPen(pen);
    customPlot->graph(0)->setData(x, y);

    // give the axes some labels:
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(startX, endX);
    customPlot->yAxis->setRange(startY, endY);

    // set ticks count
    const int maxTicksCount = 50;
    int ticksCountNeeded = (endX - startX) / eps;

    customPlot->xAxis->ticker()->setTickCount(ticksCountNeeded <= maxTicksCount ? ticksCountNeeded : maxTicksCount);
    customPlot->xAxis->setTickLabelRotation(40);

    customPlot->replot();
}

void MainWindow::showIterations(const QVector<QPair<double, double>>& iterations, const QPair<double, double>& min, double startY)
{
    for (int i = 0; i < iterations.size(); i++)
    {
        drawLine(iterations[i], startY, i, iterations[i].first == min.first, false, false);
    }
}

void MainWindow::drawLine(QPair<double, double> iteration, double startY, int iterNum, bool isFinal, bool showIter, bool showX)
{
    QPen pen;
    pen.setStyle(Qt::DashLine);
    pen.setColor(isFinal ? Qt::green : Qt::red);
    const int finalWidth = 2;
    if (isFinal)
        pen.setWidth(finalWidth);

    QCPItemLine *min1 = new QCPItemLine(customPlot);
    min1->setPen(pen);
    min1->start->setCoords(iteration.first, startY);
    min1->end->setCoords(iteration.first, iteration.second);

    QCPItemLine *min2 = new QCPItemLine(customPlot);
    min2->setPen(pen);
    const float shiftY = 0.1;
    min2->start->setCoords(iteration.first, iteration.second);
    min2->end->setCoords(iteration.first, iteration.second+shiftY);

    if (showX)
    {
        QCPItemText *textLabel = new QCPItemText(customPlot);
        textLabel->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        textLabel->position->setType(QCPItemPosition::ptPlotCoords);
        textLabel->position->setCoords(iteration.first, iteration.second+shiftY);
        textLabel->setText(QString::number(iteration.first));
    }

    if (showIter)
    {
        QCPItemText *textLabel2 = new QCPItemText(customPlot);
        textLabel2->setPositionAlignment(Qt::AlignBottom|Qt::AlignHCenter);
        textLabel2->position->setType(QCPItemPosition::ptPlotCoords);
        textLabel2->position->setCoords(iteration.first+shiftY/10, iteration.second/2);
        textLabel2->setText(QString::number(iterNum));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

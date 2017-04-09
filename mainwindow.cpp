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
    double start = -1, end = 1;
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
}

MainWindow::~MainWindow()
{
    delete ui;
}
